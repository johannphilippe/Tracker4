#ifndef CURVE_EDITOR_VIEW_H
#define CURVE_EDITOR_VIEW_H

#include<elements.hpp>
#include<widgets/curve_editor.h>
#include<widgets/custom_radio.h>
#include<tracker/jtracker.h>

using namespace cycfi::elements;

namespace jtracker {

class curve_editor_view  : public array_composite<3, vtile_element> //: public vtile_composite
{
public:
    curve_editor_view();

    inline auto make_help_button()
    {
        auto hb = icon_button(icons::info, 2.0, colors::light_steel_blue);
        hb.on_click = [&](bool b){
            if(b) {
                make_info_popup();
            }
        };
        layered_button help = hb;
        return  align_center(htile(help));

    }

    void mode_selection(bool b, curve_mode m)
    {
        if(!b) return;
        editor.get_controller().mode = m;
        tracker_app::get_instance()->_view.refresh();
    }

    void make_info_popup();
    inline auto make_mode_buttons()
    {

        auto spline_mode = squared_radio_button("Spline");
        auto log_exp = squared_radio_button("log_exp");
        auto quadbezier = squared_radio_button("quad bezier");
        auto cubbezier = squared_radio_button("cubbezier");
        auto linear = squared_radio_button("linear");


        linear.select(true);
        spline_mode.on_click = [&](bool b) {
            mode_selection(b, curve_mode::cubic_spline);
           };

        log_exp.on_click = [&](bool b) {
            mode_selection(b, curve_mode::log_exp);
        };

        cubbezier.on_click = [&](bool b) {
            mode_selection(b, curve_mode::cubic_bezier);
        };

        quadbezier.on_click = [&](bool b) {
            mode_selection(b, curve_mode::quadratic_bezier);
        };

        linear.on_click = [&](bool b) {
            mode_selection(b, curve_mode::linear);
        };

        return group("curve mode",
                     top_margin(25,
                                margin({10,10,10,10},
                                       htile(
                                           linear,
                                           hspacer(3),
                                           log_exp,
                                           hspacer(3),
                                           spline_mode,
                                           hspacer(3),
                                           cubbezier,
                                           hspacer(3),
                                           quadbezier
                                           )
                                       )
                                )
                     );
    }

private:
    curve_editor editor;
};

}

#endif // CURVE_EDITOR_VIEW_H
