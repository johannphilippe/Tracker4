#ifndef TRACK_BASE_H
#define TRACK_BASE_H


enum class track_type {tracker_track, sniper_track};

template<track_type T>
class track_base
{
public:
    track_base() : type(T)
    {}
    track_type type;

};

#endif // TRACK_BASE_H
