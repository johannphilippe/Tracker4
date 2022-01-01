#ifndef FPS_MODEL_H
#define FPS_MODEL_H

#include<elements.hpp>
#include<chrono>
using namespace cycfi::elements;
using namespace std::chrono;
using namespace std::chrono_literals;
// FPS Value used in main global data
class fps_value
{
public:
    fps_value(size_t fps = 30)
        : _fps(fps)
        , _frame_duration( 1000ms / _fps )
    {}

    size_t get_fps() {return _fps;}
    milliseconds get_frame_duration() {return _frame_duration;}

    void set_fps(size_t fps)
    {
        _fps = fps;
        _frame_duration = 1000ms /  _fps;
    }
private:
    size_t _fps;
    milliseconds _frame_duration;
};

#endif // FPS_MODEL_H
