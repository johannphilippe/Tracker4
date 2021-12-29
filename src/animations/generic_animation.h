#ifndef GENERIC_ANIMATION_H
#define GENERIC_ANIMATION_H

#include<elements.hpp>
#include<functional>
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
    bool animate()
    {
        if(_cnt >= 1.0 ) return true;
        _animate(_val);
        _increment();
        return false;
    }

    void _set_duration(float dur_in_s)
    {
        _step = (1.0f / dur_in_s / jtracker::data.app_fps.get_fps());
        _number_of_steps = dur_in_s * jtracker::data.app_fps.get_fps();
        _reset();
    }

protected:

    void _reset()
    {
        _val = _cnt = 0.0f;
    }

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

    virtual void _increment()
    {
        _val = _cnt += _step;
    }
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
    virtual void _increment() override
    {
        _cnt += _step;
        _val = log_exp_point<float>(0.0f, 1.0f, curve_definition, _cnt * float(curve_definition), _curve_factor );
    }

    size_t curve_definition = jtracker::data.app_fps.get_fps();
    float _curve_factor;
};


/*
 *  When you need to manage a lot of animations, but you wan't them all to keep in sync with an FPS, use this tool.
 *
*/
class animator_pool : public std::vector<std::shared_ptr<animation_base>>
{
public:

    void animate(view &v)
    {
        if(_is_animating || (size() == 0 ))  return;
        _is_animating = true;
        _animate(v);
    }

private:
    void _animate(view &v)
    {
        if(size() == 0)
        {
            _is_animating = false;
            return;
        }

        for(size_t i = 0; i < size();)
        {
            if(data()[i]->animate())
                erase(begin() + i);
            else
                ++i;
        }
        v.refresh();
        v.post(jtracker::data.app_fps.get_frame_duration(), [&](){_animate(v);});
    }
    bool _is_animating = false;
};

#endif // GENERIC_ANIMATION_H
