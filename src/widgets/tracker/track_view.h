#ifndef TRACK_VIEW_H
#define TRACK_VIEW_H

#include<elements.hpp>
#include<vector>
#include<memory>
#include<widgets/tracker/track.h>
#include<widgets/track_base.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

namespace jtracker {

class sniper_track : public vtile_composite, public track_base
{
public:
    sniper_track() : vtile_composite(), track_base(track_type::sniper_track) {}
};


class track_set : public htile_composite
{
public:
    track_set() : htile_composite()
    {
    }

    inline auto make_separator()
    {
        return hspacer(15);
    }

    void add_track(track_type t = track_type::tracker_track)
    {
        switch (t) {
        case track_type::tracker_track:
        {
            std::shared_ptr<track> t_ptr(new track(8, 8));
            tracks.push_back(t_ptr);
            push_back(share(htile(link(*t_ptr))));
            push_back(share(make_separator()));

        break;
        }
        case track_type::sniper_track:
        {
            std::shared_ptr<sniper_track> t_ptr(new sniper_track());
            tracks.push_back(t_ptr);
            push_back(share(htile(link(*t_ptr))));
            push_back(share(make_separator()));
            break;
        }
        }
    }

    void pop_track()
    {
        if(size() > 0)
        {
            // needs to remove hspacer too
            erase(end() - 2, end());
            tracks.pop_back();
        }
    }

    void remove_track(size_t index)
    {
        if(index >= tracks.size()) return;
        erase(begin() + (index * 2), begin() + (index * 2 ) + 1);
        tracks.erase(tracks.begin() + index);
    }

    std::vector<std::shared_ptr<track_base>> tracks;
};

}
#endif // TRACK_VIEW_H
