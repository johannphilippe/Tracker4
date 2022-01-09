#ifndef TRACK_BASE_H
#define TRACK_BASE_H

#include<variant>
#include<widgets/tracker/sniper_track.h>
#include<widgets/tracker/tracker_track.h>
/////////////////////////////////////////////////////////////
// Definitions for tracks
/////////////////////////////////////////////////////////////
namespace jtracker {
    using tracker_track_ptr = std::shared_ptr<tracker_track>;
    using sniper_track_ptr = std::shared_ptr<sniper_track>;

    using track_t = std::variant<tracker_track_ptr, sniper_track_ptr>;
    enum class track_type {tracker_track, sniper_track};
}

#endif // TRACK_BASE_H
