#include "gen_view.h"
#include<tracker/jtracker.h>
#include<tracker/jtracker_globals.h>

namespace jtracker {

gen_tabs::gen_tabs()
    : array_composite<2, vtile_element>(
            vnotebook(
              jtracker::get_app()->_view,
              deck(
                  layer(align_left_top(link(curve_editor))),
                  layer(align_left_top(label("gen 1"))),
                  layer(align_left_top(label("wavefom")))
                  ),
              tab("Curve editor"),
              tab("Gen 1"),
              tab("Waveform")
              )
          )
{}

gen_control_bar_components::gen_control_bar_components()
    : store_button(icon_button(icons::floppy, 1.0, theme.store_buttons_color))
    , to_clipboard_button(icon_button(icons::docs, 1.0, theme.info_buttons_color))
    , clear_button(icon_button(icons::trash, 1.0, theme.clear_buttons_color))
{}

gen_view::gen_view()
    : gen_tabs_holder()
    , gen_control_bar_components()
    , array_composite<3, vtile_element>(
            vtile(
              pane("GENS",
                htile(
                  simple_heading( hmax_size(200, link(this->selector)) , "GEN index"),
                  hspacer(5),
                  simple_heading(
                           htile(
                               tooltip( link(this->store_button), make_tip("Store GEN")),
                               hspacer(5),
                               tooltip( link(this->clear_button), make_tip("Clear GEN")),
                               hspacer(5),
                               tooltip( link(this->to_clipboard_button), make_tip("Copy to clipboard"))
                               ),
                           "Controls"),
                  limit({{200, 0}, {full_extent, 0}}, box(colors::red.opacity(0)))
                  )),
                vspacer(10),
                layer(align_left_top(this->tabs))
              )
          )
{
    selector.on_change = [&](size_t v)
    {
        recall_gen(v);
    };


    store_button.on_click = [&](bool)
    {
        store_current_gen();
    };

    clear_button.on_click = [&](bool)
    {
        tabs.curve_editor.editor.clear();
        get_app()->_view.refresh(tabs.curve_editor.editor);

    };

    to_clipboard_button.on_click = [&](bool)
    {
    };

    recall_gen(1);
}

void gen_view::store_current_gen()
{
    switch (current_gen_type) {
    case gen_type::curve:
    {
        auto &crv = tabs.curve_editor.editor.get_curve();
        if(crv.size() > 0)
        {
            json curve_data = crv.to_json();
            json j = get_gen_data(gen_type::curve, curve_data);
            jtracker::data.gen_data[std::to_string(selector.get_value())] = j;
            save_gen_data();
        }
        break;
    }
    case gen_type::raw_data:
    {

        break;
    }
    case gen_type::waveform:
    {
        break;
    }
    default: break;
    }


}

void gen_view::recall_gen(size_t index)
{
    std::string s_idx = std::to_string(index);
   if(!jtracker::data.gen_data.contains(s_idx))
   {
       // Clear all if no data
       tabs.curve_editor.editor.clear();
       tabs.curve_editor.editor.get_controller().set_mode(curve_mode::linear);
       tabs.curve_editor.set_radio_selection(curve_mode::linear);
       jtracker::get_app()->_view.refresh(tabs.curve_editor);
       return;
   }
   json current = jtracker::data.gen_data[s_idx];
   current_gen_type = static_cast<gen_type>(current["type"].get<int>());
   switch (current_gen_type) {
    case gen_type::curve:
    {
        struct curve c(current["gen_data"]);
        tabs.curve_editor.editor.set_curve(c);
        tabs.curve_editor.set_radio_selection(c.mode);
        jtracker::get_app()->_view.refresh(tabs.curve_editor);
        break;
    }
    case gen_type::raw_data:
    {

        break;
    }
    case gen_type::waveform:
    {
        break;
    }
    default: break;
   }

}


}
