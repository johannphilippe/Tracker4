#ifndef TRACK_H
#define TRACK_H

#include<elements.hpp>
#include<tracker/jtracker.h>
#include<iostream>

#include<widgets/track_content.h>
using namespace cycfi::elements;

namespace jtracker {

class tracker_track :
        public array_composite<2, vtile_element>
{
public:
    array_composite<2, vtile_element> make_buttons_tile(size_t track_index);

    tracker_track(size_t track_index = 0);

    bool click(context const& ctx, mouse_button btn) override;
    void end_focus() override;

    track_content<track_line> t_content;
};
}

#endif // TRACK_H
