#ifndef TRACK_VIEW_H
#define TRACK_VIEW_H

#include<elements.hpp>
#include<vector>
#include<memory>
#include<variant>
#include<widgets/tracker/tracker_track.h>
#include<widgets/tracker/sniper_track.h>
#include<widgets/track_base.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

namespace jtracker {

using tracker_track_ptr = std::shared_ptr<tracker_track>;
using sniper_track_ptr = std::shared_ptr<sniper_track>;

class track_set : public htile_composite
{
public:
    track_set() : htile_composite()
    {}

    inline auto make_separator()
    {
        return hspacer(15);
    }

    void add_track(track_type t = track_type::tracker_track)
    {
        switch (t) {
        case track_type::tracker_track:
        {
            tracker_track_ptr t_ptr(new tracker_track(8, 8));
            std::variant<tracker_track_ptr, sniper_track_ptr > v;
            v = t_ptr;
            tracks.push_back(v);
            push_back(share(htile(link(*t_ptr), make_separator())));

        break;
        }
        case track_type::sniper_track:
        {
            sniper_track_ptr t_ptr(new sniper_track());
            std::variant<tracker_track_ptr, sniper_track_ptr> v;
            v = t_ptr;
            tracks.push_back(v);
            push_back(share(htile(link(*t_ptr), make_separator())));
            break;
        }
        }
    }

    void pop_track()
    {
        if(size() > 0)
        {
            pop_back();
            tracks.pop_back();
        }
    }

    void remove_track(size_t index)
    {
        if(index >= tracks.size()) return;
        erase(begin() + index, begin() + index + 1);
        tracks.erase(tracks.begin() + index, tracks.begin() + index + 1);
    }


    std::vector<std::variant<tracker_track_ptr, sniper_track_ptr>> tracks;
};

}
#endif // TRACK_VIEW_H
