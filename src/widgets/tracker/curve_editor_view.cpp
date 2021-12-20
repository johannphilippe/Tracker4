#include "curve_editor_view.h"

namespace jtracker {

curve_editor_view::curve_editor_view() :
    array_composite<3, vtile_element>(vtile(make_help_button(), link(editor), make_mode_buttons()))
{
}

void curve_editor_view::make_info_popup()
{
    std::string help(  "Curve Editor \n"
                       "Click on the canvas and plot samples to draw 2D curves. \n"
                       "There are several modes (displayed at the bottom). "
                       "Some require 2 samples to work (linear, log_exp), while some require 3 (cubic spline, quadbezier) or 4 (cubbezier). \n"
                       "To curve the log_exp mode you can alt or shift click + drag up / down or simply scroll. \n"
                       "Alt+click or shift+click on a sample removes it.\n"
                       "Double lick on a sample will snap it to the closest grid node. \n");
    auto on_ok = [&](){
    };

    tracker_app *instance = tracker_app::get_instance();
    auto popup = message_box1(instance->_view, help, icons::info, on_ok, "OK", point(500, 150) );
    instance->_view.add(popup);
}



}

