#ifndef PAINT_UTILITIES_H
#define PAINT_UTILITIES_H

#include<elements.hpp>
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
    color_difference(float r_, float g_, float b_, float a_) :
        r(r_), g(g_), b(b_), a(a_)
    {}
    float r, g, b, a;
};

class interpolated_color : public color
{
public:
    interpolated_color(color c) : color(c)
    {}
    interpolated_color(color from_, color to_, size_t steps_) :
        color(from_),
        count(0),
        steps(steps_),
        to(to_)
    {
        diff();
    }

    void set_color_target(color c, size_t steps_ = 30)
    {
        to = c;
        count = 0;
        diff();
        steps = steps_;
    }

    void diff()
    {
        difference = color_difference(
                    to.red - red, to.green - green,
                    to.blue - blue, to.alpha - alpha
                    );
    }

    bool interpolate()
    {
        if(count >= steps) return true;
        this->red += (float(difference.r) / float(steps) );
        this->green += (difference.g / steps);
        this->blue += (difference.b / steps);
        this->alpha += (difference.a / steps);
        count++;
        return false;
    }

    bool compare()
    {
        if(this->red == to.red && this->green == to.green &&
                this->blue == to.blue && this->alpha == to.alpha)
            return true;
        return false;
    }

    size_t count, steps;
    color to;
    color_difference difference;
};




}

}
#endif // PAINT_UTILITIES_H
