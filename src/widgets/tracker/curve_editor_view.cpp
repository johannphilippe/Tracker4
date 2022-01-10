#include "curve_editor_view.h"
#include<widgets/common_widgets.h>

namespace jtracker {

curve_editor_radios::curve_editor_radios()
    : linear(squared_radio_button("Linear"))
    , log_exp(squared_radio_button("Logarithmic/Exponential"))
    , spline(squared_radio_button("Spline"))
    , quadbezier(squared_radio_button("Quadratic Bezier"))
    , cubbezier(squared_radio_button("Cubic Bezier"))
{}

curve_editor_radio_group::curve_editor_radio_group()
    : array_composite<3, layer_element>(
          group("Curve mode",
                top_margin(25,
                           margin({10,10,10,10},
                                  htile(
                                      link(linear),
                                      hspacer(3),
                                      link(log_exp),
                                      hspacer(3),
                                      link(spline),
                                      hspacer(3),
                                      link(cubbezier),
                                      hspacer(3),
                                      link(quadbezier)
                                      )
                                  )
                           )
                    )
          )
{}

void curve_editor_radio_group_holder::set_radio_selection(curve_mode c)
{
    radios.linear.select(false);
    radios.log_exp.select(false);
    radios.cubbezier.select(false);
    radios.spline.select(false);
    radios.quadbezier.select(false);
    switch (c) {
    case curve_mode::linear:
    {
        radios.linear.select(true);
        break;
    }
    case curve_mode::log_exp:
    {
        radios.log_exp.select(true);
        break;
    }
    case curve_mode::cubic_bezier:
    {
        radios.cubbezier.select(true);
        break;
    }
    case curve_mode::cubic_spline:
    {
        radios.spline.select(true);
        break;
    }
    case curve_mode::quadratic_bezier:
    {
        radios.quadbezier.select(true);
        break;
    }
    default: break;
    }
}


void curve_editor_view::make_info_popup()
{
    constexpr static const char * help =  "Curve Editor \n"
                       "Click on the canvas and plot samples to draw 2D curves. \n"
                       "There are several modes (displayed at the bottom). "
                       "Some require 2 samples to work (linear, log_exp), while some require 3 (cubic spline, quadbezier) or 4 (cubbezier). \n"
                       "To curve the log_exp mode you can alt or shift click + drag up / down or simply scroll. \n"
                       "Alt+click or shift+click on a sample removes it.\n"
                       "Double lick on a sample will snap it to the closest grid node. \n";
    auto on_ok = [&](){
    };

    tracker_app *instance = tracker_app::get_instance();
    auto popup = message_box1(instance->_view, help, icons::info, on_ok, "OK", point(500, 150) );
    instance->_view.add(popup);
}


auto curve_editor_view::make_mode_buttons()
{
    radios.linear.select(true);
    radios.spline.on_click = [&](bool b) {
        mode_selection(b, curve_mode::cubic_spline);
       };

    radios.log_exp.on_click = [&](bool b) {
        mode_selection(b, curve_mode::log_exp);
    };

    radios.cubbezier.on_click = [&](bool b) {
        mode_selection(b, curve_mode::cubic_bezier);
    };

    radios.quadbezier.on_click = [&](bool b) {
        mode_selection(b, curve_mode::quadratic_bezier);
    };

    radios.linear.on_click = [&](bool b) {
        mode_selection(b, curve_mode::linear);
    };
    get_app()->_view.refresh();
    return link(radios);
}

inline auto curve_editor_view::make_header()
{
    auto hb = icon_button(icons::info, 1, jtracker::theme.info_buttons_color);
    hb.on_click = [&](bool b){
        if(b) {
            make_info_popup();
        }
    };
    auto help_button_tip = tooltip(hb, make_tip("Help"));

    auto compare_button = toggle_icon_button(icons::exchange, 1.0);
    compare_button.on_click = [&](bool )
    {

    };
    auto compare_button_tip = tooltip(compare_button, make_tip("Enable compare curves"));

    size_spinbox compare_spin;


    return htile(
                help_button_tip,
                hspacer(5),
                compare_button_tip,
                hspacer(5),
                hmax_size(200, compare_spin),
                limit({{400, 0}, {full_extent, 0}}, box(colors::black.opacity(0)))
                );
}

void curve_editor_view::mode_selection(bool b, curve_mode m)
{
    if(!b) return;
    editor.get_controller().samples.mode = m;
    tracker_app::get_instance()->_view.refresh();
}

curve_editor_view::curve_editor_view()
    : curve_editor_radio_group_holder()
    , array_composite<3, vtile_element>(
        vtile(
              vtile(
                  vspacer(5),
                  make_header(),
                  vspacer(10)
                  ),
              link(editor),
              make_mode_buttons()
              )
          )
{}

}

