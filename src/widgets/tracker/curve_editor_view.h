#ifndef CURVE_EDITOR_VIEW_H
#define CURVE_EDITOR_VIEW_H

#include<tracker/jtracker.h>
#include<elements.hpp>
#include<widgets/curve_editor.h>
#include<widgets/custom_radio.h>

using namespace cycfi::elements;

namespace jtracker {


template<typename ... T>
auto make_custom_radio(std::string name, T&& ... elements)
{
    return group(name, top_margin(25, margin({10, 10, 10, 10},
                                             htile(share(std::forward<T>(elements))...))));
}


class curve_editor_view : public vtile_composite
{
public:
    curve_editor_view() : vtile_composite()
    {
        push_back(share(hstretch(1, make_help_button())));
        push_back(share(editor));
        push_back(share(hstretch(1, make_custom_radio("Curve Mode", linear, log_exp, quadbezier, cubbezier, spline_mode))));
    }

    layered_button make_help_button()
    {
        auto hb = icon_button(icons::info, 2.0, colors::light_steel_blue);
        hb.on_click = [&](bool b){
          if(b) {
              make_info_popup();
          }
        };
        layered_button help = hb;
        return  help;
    }

    void make_info_popup()
    {
        std::string help(  "Curve Editor \n"
                           "Click on the canvas and plot samples to draw 2D curves. \n"
                           "There are several modes (displayed at the bottom). "
                           "Some require 2 samples to work (linear, log_exp), while some require 3 (cubic spline, quadbezier) or 4 (cubbezier). \n"
                           "To curve the log_exp mode you can alt or shift click + drag up / down or simply scroll. \n"
                           "Alt+click or shift+click on a sample removes it.\n");
        auto on_ok = [&](){

        };

        tracker_app *instance = tracker_app::get_instance();

        auto popup = message_box1(instance->_view, help, icons::info, on_ok, "OK", point(500, 150) );
        instance->_view.add(popup);
    }

    void make_mode_buttons(curve_editor &editor)
    {
       tracker_app *instance = tracker_app::get_instance();

       spline_mode = squared_radio_button("Spline");
       log_exp = squared_radio_button("log_exp");
       quadbezier = squared_radio_button("quad bezier");
       cubbezier = squared_radio_button("cubbezier");
       linear = squared_radio_button("linear");

       linear.select(true);
       spline_mode.on_click = [&](bool b) {
           if(!b) return;
           editor.value(curve_editor_controller(curve_mode::cubic_spline));
           instance->_view.refresh();
       };
       log_exp.on_click = [&](bool b) {
           if(!b) return;
           editor.value(curve_editor_controller(curve_mode::log_exp));
           instance->_view.refresh();
       };
       cubbezier.on_click = [&](bool b) {
           if(!b) return;
           editor.value(curve_editor_controller(curve_mode::cubic_bezier));
           instance->_view.refresh();
       };
       quadbezier.on_click = [&](bool b) {
           if(!b) return;
           editor.value(curve_editor_controller(curve_mode::quadratic_bezier));
           instance->_view.refresh();
       };
       linear.on_click = [&](bool b) {
           if(!b) return;
           editor.value(curve_editor_controller(curve_mode::linear));
           instance->_view.refresh();
       };


       //return group("curve mode", top_margin(25, margin({10,10,10,10}, htile(
       //            linear, hspacer(3), log_exp, hspacer(3), spline_mode, hspacer(3),  cubbezier,  hspacer(3), quadbezier
       //            ))));
    }

private:
    curve_editor editor;
    basic_choice<proxy<squared_radio_button_element, basic_button>> spline_mode, log_exp, quadbezier, cubbezier, linear;

};

}

#endif // CURVE_EDITOR_VIEW_H
