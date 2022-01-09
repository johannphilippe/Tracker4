#include<dialogs/options_dialog.h>

auto option_item::make_view()
{
    auto tog_enable = plain_toggle_icon_button( icons::cancel, icons::ok, 1.0);
    tog_enable.value(enabled);
    tog_enable.on_click = [&](bool b)
    {
        enabled = b;
    };

    auto tb = input_box("Write a Csound option");
    tb.second->set_text(option);
    tb.second->on_text = [&](std::string_view v)
    {
      option = v;
    };
    return htile(
                (tb.first),
                (tog_enable)
           );
}

option_item::option_item(std::string name, bool enable) :
    options_dialog_click_callback(),
    audio_cs_option(name, enable),
    array_composite<2, htile_element>(make_view())
{}

void cs_option_list::add_item(std::string s, bool enabled)
{
    auto item = std::make_shared<option_item>(s, enabled);
    auto full_item = share(basic_menu_item( right_margin(15, hold(item) )));
    full_item->on_click = [&, item](){
        for(size_t i = 0; i < items.size(); i++)
        {
            if(item.get() == items[i].get())
            {
                _selected = i;
                break;
            }
        }
    };

    items.push_back(item);
    this->push_back(full_item);
}

void cs_option_list::add_item() {add_item("", true);}

void cs_option_list::remove_selected()
{
    if(_selected == -1) return;
    items.erase(items.begin() + _selected);
    erase(begin() + _selected);
    _selected = -1;
}

void cs_option_list::pop()
{
    if(size() == 0) return;
    if(_selected != -1 && _selected == static_cast<int>(items.size()) - 1)
        _selected = -1;
    items.pop_back();
    vtile_composite::pop_back();
}

auto audio_options_view::make_view()
{
    auto plus_but = plain_icon_button(icons::plus, 1.0);
    auto minus_but = plain_icon_button(icons::minus, 1.0);
    auto rem_but = plain_icon_button(icons::cancel, 1.0);

    plus_but.on_click = [&](bool)
    {
        options_list.add_item();
        jtracker::get_app()->_view.layout();
        jtracker::get_app()->_view.refresh();
    };
    minus_but.on_click = [&](bool)
    {
        if(options_list.size() == 0) return;
        options_list.pop();
        jtracker::get_app()->_view.layout();
        jtracker::get_app()->_view.refresh();
    };
    rem_but.on_click = [&](bool)
    {
        if(options_list.size() == 0 ) return;
        options_list.remove_selected();
        jtracker::get_app()->_view.layout();
        jtracker::get_app()->_view.refresh();
    };

    return (margin({15, 10, 15, 10}, vtile(
            simple_heading(link(sample_rate_spin), "Sample rate"),
            vspacer(5),
            simple_heading(link(ksmps_spin), "Ksmps - audio vector size"),
            vspacer(5),
            simple_heading(link(dbfs_spin), "0dbfs"),
            vspacer(5),
            htile(
            simple_heading(link(inchnls_spin), "Input channels"),
            vspacer(5),
            simple_heading(link(nchnls_spin), "Output channels")
              ),
            vspacer(5),
            simple_heading(
                  vtile(
                  align_center(
                      htile(
                      align_left(rem_but),
                      align_middle(minus_but),
                      align_right(plus_but)
                      )),
                  layer(
                      limit({{60, 150},{full_extent, full_extent}}, vscroller(
                      link(options_list)
                      )),
                      rbox(jtracker::theme.app_background_color.level(0.65), 6)
                      )
                  ), "Csound options")
            )))	;
}



options_dialog* options_dialog::instance = nullptr;

options_dialog* options_dialog::get_instance()
{
    if(instance != nullptr)
        delete instance;
    instance = new options_dialog();
    return instance;
}

auto options_dialog::make_options_tab()
{

    // first construct object
    audio_options = std::make_shared<audio_options_view>();

    auto tabs = hnotebook(
           jtracker::get_app()->_view,
           deck(
                layer(align_center_middle(audio_options->make_view())),
                layer(align_center_middle(label("Options2").font_size(100)), frame{}),
                layer(align_center_middle(label("Options3").font_size(100)), frame{})
               ),
           tab("Audio Options"),
           tab("Options2"),
           tab("Options3")
           );
    return tabs;
}

void options_dialog::update_config()
{
    // audio actualize
    jtracker::data.audio_config.sample_rate = audio_options->sample_rate_spin.get_value();
    jtracker::data.audio_config.ksmps = audio_options->ksmps_spin.get_value();
    jtracker::data.audio_config.dbfs = audio_options->dbfs_spin.get_value();
    jtracker::data.audio_config.i_nchnls = audio_options->inchnls_spin.get_value();
    jtracker::data.audio_config.nchnls = audio_options->nchnls_spin.get_value();
    jtracker::data.audio_config.additional_options.clear();
    for(auto & it : audio_options->options_list.items)
    {
        jtracker::data.audio_config.additional_options.push_back( audio_cs_option(it->option, it->enabled) );
    }
}

void options_dialog::make_content()
{
    _name = "Options";
    auto close_but = button("Close");
    close_but.on_click = [&](bool)
    {
      close();
    };

    auto ok_button = button("Validate");
    ok_button.on_click = [&](bool)
    {
        // save before
        jtracker::save_audio_config();
        update_config();
        close();
    };

    content_ptr = share(
            margin({10, 10, 10, 10},
                vtile(
                make_options_tab(),
                htile(ok_button,close_but)
                )
            ));
}
