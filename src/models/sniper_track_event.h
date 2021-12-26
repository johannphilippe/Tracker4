#ifndef SNIPER_TRACK_EVENT_DATA_H
#define SNIPER_TRACK_EVENT_DATA_H

#include<models/track_event.h>
#include<elements.hpp>
using namespace cycfi::elements;

struct sniper_track_event
{
    rect bounds;
    track_event_type type;
    bool selected = false;
};

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