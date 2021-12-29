#ifndef SNIPER_TRACK_EVENT_DATA_H
#define SNIPER_TRACK_EVENT_DATA_H

#include<models/track_event.h>
#include<elements.hpp>
#include<utilities/paint_utilities.h>
using namespace cycfi::elements;
using namespace jtracker::color_utilities;

struct sniper_track_event
{
    rect bounds;
    track_event_type type;
    bool selected = false;
    std::shared_ptr<color_animation> current_color;
};

extern sniper_track_event sniper_null_event;

struct sniper_track_drag_resize
{
    enum class mode
    {
        none = 0,
        top = 1,
        bottom = 2
    };

    mode resize_mode;
    size_t index;
};

#endif // SNIPER_TRACK_EVENT_DATA_H
