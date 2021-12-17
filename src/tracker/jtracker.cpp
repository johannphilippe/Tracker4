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
    _win(name()), _view(_win)
{
    std::cout << "singleton constructed" << std::endl;
    _win.on_close = [this](){
        this->stop();
    };
}

tracker_app* tracker_app::app_ = nullptr;

// Main window background color
cycfi::artist::color bkd_color = cycfi::artist::rgba(35, 35, 37, 255);
cycfi::elements::box_element background = box(bkd_color);

}
