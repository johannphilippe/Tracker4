#ifndef JTRACKER_H
#define JTRACKER_H

#include<elements.hpp>

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

private:
    tracker_app(int argc, char *argv[]);
    static tracker_app *app_;
};



}

#endif // JTRACKER_H
