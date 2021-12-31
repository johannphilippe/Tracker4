#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H
#include<elements.hpp>
#include<tracker/jtracker.h>
#include<dialogs/elements_dialog_base.h>
#include<widgets/spinbox.h>
using namespace cycfi::elements;

class additional_option_item :
        public array_composite<2, htile_element>,
        public audio_cs_option
{
public:
    auto make_view()
    {
        auto tog_enable = toggle_icon_button(icons::ok, icons::cancel, 1.0);
        tog_enable.on_click = [&](bool b)
        {
            enabled = b;
        };

        auto in_box = input_box();
        in_box.second->on_text = [&](std::string_view v)
        {
            option = v;
        };


        auto tb = share(basic_input_box());
        tb->on_text = [&](std::string_view v)
        {
          option = v;
        };

        return htile(

                   //in_box.first,
                    hold(tb),
                   //basic_input_box(),
                   tog_enable
                   );
    }

    additional_option_item(std::string name, bool enable) :
        array_composite<2, htile_element>(make_view()),
        audio_cs_option(name, enable)
    {}

    basic_input_box text_box;
};

class audio_options_view
{
public:
    audio_options_view()
        : sample_rate_spin(spinbox_list_controller<size_t>(
                               jtracker::data.audio_config.sample_rate, std::vector<size_t>(audio_rate_list, audio_rate_list + audio_rate_list_size )
                               )
                           )
        , ksmps_spin(spin_controller<size_t>(1, 22'050, jtracker::data.audio_config.ksmps, 1))
        , dbfs_spin(spin_controller<size_t>(1, 64'000, jtracker::data.audio_config.dbfs, 1))
        , inchnls_spin(spin_controller<int>(0, std::numeric_limits<int>::max(), jtracker::data.audio_config.i_nchnls, 1))
        , nchnls_spin(spin_controller<int>(0, std::numeric_limits<int>::max(), jtracker::data.audio_config.nchnls, 1))
    {
        for(auto & it : jtracker::data.audio_config.additional_options)
        {
            additional_options_list.push_back(share(additional_option_item(it.option, it.enabled)));
            additional_options_list_tile.push_back(additional_options_list.back());
            jtracker::get_app()->_view.layout();
        }
    }

    auto make_view()
    {
        std::cout << "make view : " << std::endl;
        auto plus_but = icon_button(icons::plus, 1.0);
        plus_but.on_click = [&](bool)
        {
            additional_options_list.push_back(std::make_shared<additional_option_item>("", true));
            additional_options_list_tile.push_back(share(right_margin(15, layer(link(*additional_options_list.back()), rbox(jtracker::theme.app_background_color.level(1.3)))  )));
            jtracker::get_app()->_view.layout();
            //jtracker::get_app()->_view.refresh();
        };

        return margin({0, 10, 0, 10}, vtile(
                    simple_heading(link(sample_rate_spin), "Sample rate"),
                    vspacer(5),
                    simple_heading(link(ksmps_spin), "Ksmps - audio vector size"),
                    vspacer(5),
                    simple_heading(link(dbfs_spin), "0dbfs"),
                    vspacer(5),
                    simple_heading(link(inchnls_spin), "Input channels"),
                    vspacer(5),
                    simple_heading(link(nchnls_spin), "Output channels"),
                    vspacer(5),
                    simple_heading(
                              vtile(
                                  htile(
                                      align_center_middle(plus_but)
                                      ),
                                  layer(
                                      limit({{40, 100},{full_extent, full_extent}}, vscroller(
                                          link(additional_options_list_tile)
                                          )),
                                      rbox(jtracker::theme.app_background_color.level(0.55), 6)
                                      )
                                  ), "Additional options")
                    ));
    }

    spinbox_list<size_t> sample_rate_spin;
    spinbox<size_t> ksmps_spin, dbfs_spin;
    spinbox<int> inchnls_spin, nchnls_spin;
    vtile_composite additional_options_list_tile;
    std::vector<std::shared_ptr<additional_option_item>> additional_options_list;
};

class options_dialog : public elements_dialog_base
{
public:
    static options_dialog* get_instance() {
        if(instance != nullptr)
            delete instance;
        instance = new options_dialog();
        return instance;
    }



    auto make_options_tab()
    {
        // first construct object
        audio_options = std::make_shared<audio_options_view>();
        jtracker::data.audio_config.print_data();

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

    void update_config()
    {
        std::cout << "updating config " << std::endl;
        jtracker::data.audio_config.print_data();
        std::cout << "spin sample rate " << audio_options->sample_rate_spin.get_value() << std::endl;
        // audio actualize
        jtracker::data.audio_config.sample_rate = audio_options->sample_rate_spin.get_value();
        jtracker::data.audio_config.ksmps = audio_options->ksmps_spin.get_value();
        jtracker::data.audio_config.dbfs = audio_options->dbfs_spin.get_value();
        jtracker::data.audio_config.i_nchnls = audio_options->inchnls_spin.get_value();
        jtracker::data.audio_config.nchnls = audio_options->nchnls_spin.get_value();

        jtracker::data.audio_config.additional_options.clear();
        for(auto & it : audio_options->additional_options_list)
        {
            jtracker::data.audio_config.additional_options.push_back( audio_cs_option(it->option, it->enabled) );
        }
        jtracker::data.audio_config.print_data();
    }

    void make_content() override
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

private:
    options_dialog() {};
    options_dialog& operator= (const options_dialog&) = delete;
    options_dialog(const options_dialog& ) = delete;
    static options_dialog *instance;

    // members
    std::shared_ptr<audio_options_view> audio_options;

};

options_dialog* options_dialog::instance = nullptr;



#endif // OPTIONS_DIALOG_H
