#ifndef JTRACKER_GLOBALS_H
#define JTRACKER_GLOBALS_H

#include<iostream>
#include<tracker/jtracker_theme.h>
#include<chrono>
using namespace std::chrono;
using namespace std::chrono_literals;
namespace jtracker {

float get_track_line_height();

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

struct tracker_data
{
    size_t seq_nbr = 1;
    size_t number_of_tracks = 1;
    size_t number_of_lines = 8;
    size_t grid_step = 0;

    float track_line_height = get_track_line_height();

    constexpr static const size_t default_column_number = 8;

    fps_value app_fps;
};

extern tracker_data data;

}


#endif // JTRACKER_GLOBALS_H
