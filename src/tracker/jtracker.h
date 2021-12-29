#ifndef JTRACKER_H
#define JTRACKER_H

#include<elements.hpp>
#include<iostream>
#include<tracker/jtracker_theme.h>
#include<animations/generic_animation.h>

using namespace cycfi::elements;

namespace jtracker {

// Main window background color
extern cycfi::artist::color bkd_color;
extern box_element background;

// App singleton
class tracker_app : public app
{

public:
    static tracker_app *get_instance(int argc = 0, char *argv[] = nullptr);

    window _win;
    view _view;

    // Animation utilities
    //color_animator _color_animator;
    //static color_animator& get_color_animator();
    animator_pool _animator_pool;
    static animator_pool& get_animator_pool();

    void refresh()
    {
        if(tracker_app::app_ == nullptr) return;
        _view.refresh();
    }

    void refresh(element_ptr ptr)
    {
        if(tracker_app::app_ == nullptr) return;
        _view.refresh(*ptr);
    }

private:
    tracker_app(int argc, char *argv[]);
    tracker_app& operator= (const tracker_app&) = delete;
    tracker_app (const tracker_app&) = delete;

    static tracker_app *app_;
};

inline auto make_base_layer()
{
    rect pad(0, 0, 1920, 1080);
}

// to reduce verbosity
inline tracker_app *get_app() {return tracker_app::get_instance();}

}

#endif // JTRACKER_H
