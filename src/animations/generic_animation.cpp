#include<animations/generic_animation.h>

bool animation_base::animate()
{
    if(_cnt > 1.0 ) return true;
    _animate(_val);
    _increment();
    return false;
}

void animation_base::_set_duration(float dur_in_s)
{
    _step = (1.0f / dur_in_s / jtracker::data.app_fps.get_fps());
    _number_of_steps = dur_in_s * jtracker::data.app_fps.get_fps();
    _reset();
}

void animation_base::_reset()
{
    _val = _cnt = 0.0f;
}

void animation_base::_increment()
{
    _val = _cnt += _step;
}

void animation_log_exp::_increment()
{
    _cnt += _step;
    _val = log_exp_point<float>(0.0f, 1.0f, curve_definition, _cnt * float(curve_definition), _curve_factor );
}

void animator_pool::animate(view &v)
{
    if(_is_animating || (size() == 0 ))  return;
    _is_animating = true;
    _animate(v);
}

void animator_pool::_animate(view &v)
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
