#ifndef TRACK_EVENT_H
#define TRACK_EVENT_H

#include<elements.hpp>
#include<tracker/jtracker_theme.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

enum class track_event_type
{
    none = 0,
    csound_score = 1,
    lua_rt_process = 2,
    chuck_rt_process = 3,
    antescofo_rt_process = 4,
    audio_sample = 5
};

inline color get_event_color(track_event_type t)
{
    color c;
    const float op = 0.3;
    switch(t)
    {
    case track_event_type::none:
    {
        c = jtracker::theme.none_event_color;
        //c = get_theme().panel_color;
        break;
    }
    case track_event_type::csound_score:
    {
        c = jtracker::theme.csound_event_color.opacity(op);
        break;
    }
    case track_event_type::lua_rt_process:
    {
        c = jtracker::theme.lua_rt_event_color.opacity(op);
        break;
    }
    case track_event_type::chuck_rt_process:
    {
        c = jtracker::theme.chuck_rt_color.opacity(op);
        break;
    }
    case track_event_type::antescofo_rt_process:
    {
        c = jtracker::theme.antescofo_rt_color.opacity(op);
        break;
    }
    case track_event_type::audio_sample:
    {
        c = jtracker::theme.audio_sample_color.opacity(op);
        break;
    }
    default:
        c = get_theme().panel_color;
        break;
    }

    return c;
}

#endif // TRACK_EVENT_H
