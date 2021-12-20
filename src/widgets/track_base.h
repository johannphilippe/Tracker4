#ifndef TRACK_BASE_H
#define TRACK_BASE_H


enum class track_type {tracker_track, sniper_track};

class track_base
{
public:
    track_base(track_type t = track_type::tracker_track) : type(t)
    {}
    track_type type;
};

#endif // TRACK_BASE_H
