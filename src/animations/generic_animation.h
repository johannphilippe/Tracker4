#ifndef GENERIC_ANIMATION_H
#define GENERIC_ANIMATION_H

#include<elements.hpp>
#include<functional>
#include<animations/fps_model.h>
#include<utilities/maths_utilities.h>
#include<tracker/jtracker_globals.h>
using namespace cycfi::elements;
using namespace jtracker::maths;

enum class animation_curve_type
{
    linear,
    logarithmic,
    exponential
};

// Simple linear animation
class animation_base
{
public:
    animation_base(float duration_in_s)
        : _step(1.0f / duration_in_s / jtracker::data.app_fps.get_fps())
        , _number_of_steps(duration_in_s * jtracker::data.app_fps.get_fps())
    {}

    // if returs true, ends the animation
    bool animate();

    void _set_duration(float dur_in_s);

protected:

    void _reset();

    // this count is linear
    float _cnt = 0.0f;
    // this one is based on curve_type
    float _val = 0.0f;
    // this one is the value of the step on a base 1
    float _step = 0.1f;
    // total count of steps to cross during animation
    size_t _number_of_steps = 1;


private:
    // Method to override
    // will be called with a value between 0 and 1 (cnt) where 0 is start, and 1 the end of animation
    virtual void _animate(float  cnt) = 0;

    virtual void _increment();
};

class animation_log_exp : public animation_base
{

public:
    animation_log_exp(float dur_in_s, float curve_factor = 0.0)
        : animation_base(dur_in_s)
        , _curve_factor(limit_number<float>(-10, 10, curve_factor) )
    {}

protected:

private:
    virtual void _increment() override;

    size_t curve_definition = jtracker::data.app_fps.get_fps();
    float _curve_factor;
};


/*
 *  When you need to manage a lot of animations, but you wan't them all to keep in sync with an FPS, use this tool.
*/
class animator_pool : public std::vector<std::shared_ptr<animation_base>>
{
public:

    void animate(view &v);

private:
    void _animate(view &v);
    bool _is_animating = false;
};

#endif // GENERIC_ANIMATION_H
