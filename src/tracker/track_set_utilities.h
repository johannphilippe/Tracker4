#ifndef TRACK_SET_UTILITIES_H
#define TRACK_SET_UTILITIES_H

#include<variant>
#include<elements.hpp>
#include<widgets/tracker/tracker_track.h>
#include<widgets/tracker/sniper_track.h>
#include<tracker/track_base.h>

using namespace cycfi::elements;

namespace jtracker {


namespace track_set_utilities {
    void set_text_to_selected_cells(std::vector<track_t> &t, std::string_view v);

};

}


#endif // TRACK_SET_UTILITIES_H
