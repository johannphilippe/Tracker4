#ifndef PAINT_UTILITIES_H
#define PAINT_UTILITIES_H

#include<elements.hpp>
#include<tracker/jtracker_globals.h>
#include<animations/generic_animation.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

namespace jtracker {

namespace color_utilities {

inline color invert_color(color c)
{
    return color(1.0 - c.red, 1.0 - c.green, 1.0 - c.blue, c.alpha);
}

struct color_difference
{
    color_difference() = default;
    color_difference(color &a, color &b)
        : r(b.red - a.red)
        , g(b.green - a.green)
        , b(b.blue - a.blue)
        , a(b.alpha - a.alpha)
    {}
    color_difference(float r_, float g_, float b_, float a_) :
        r(r_), g(g_), b(b_), a(a_)
    {}
    float r, g, b, a;
};


class color_animation : public animation_base, public color
{
public:
    color_animation(color from, float dur_in_s = 0.5)
        : animation_base(dur_in_s), color(from), _from(from)
    {
    }

    void set_color_target(color t, float dur_in_s = 0.5)
    {
        _from = *this;
        _target = t;
        calculate_difference();
        _set_duration(dur_in_s);
    }

protected:

    void _animate(float cnt) {
        red = _from.red + (_diff.r * cnt );
        green = _from.green + (_diff.g * cnt);
        blue = _from.blue + (_diff.b * cnt );
        alpha = _from.alpha + (_diff.a * cnt);
    }


    void interpolate()
    {
    }

    void calculate_difference()
    {
        _diff = color_difference(*this, _target);
    }

    color _target;
    color _from;
    color_difference _diff;
};


}

}
#endif // PAINT_UTILITIES_H
