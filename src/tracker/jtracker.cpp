#include<tracker/jtracker.h>
#include<elements.hpp>
#include<iostream>

namespace jtracker {


tracker_app *tracker_app::get_instance(int argc, char *argv[])
{
    if(tracker_app::app_ == nullptr)
        tracker_app::app_ = new tracker_app(argc, argv);
    return tracker_app::app_;
}

tracker_app::tracker_app(int argc, char *argv[]) :
    app(argc, argv, "Curve Editor", "com.johannphilippe.curve_editor"),
    _win(name(), window::style::standard, rect(0,0,1920,1080)), _view(_win)
{
    _win.on_close = [this](){
        this->stop();
    };

}

cell_animator& tracker_app::get_cell_animation()
{
    return get_instance()->_cell_animation;
}

tracker_app* tracker_app::app_ = nullptr;

// Main window background color
cycfi::elements::box_element background = box(jtracker::theme.app_background_color);


}
