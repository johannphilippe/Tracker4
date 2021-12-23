#ifndef SNIPER_TRACK_H
#define SNIPER_TRACK_H

#include<elements.hpp>
#include<widgets/track_base.h>
using namespace cycfi::elements;

class sniper_track :
        public track_base<track_type::sniper_track>,
        public vtile_composite
{
public:
    sniper_track() : vtile_composite()
    {
        push_back(share(button("hello")));
    }
};

#endif // SNIPER_TRACK_H
