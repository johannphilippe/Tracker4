#ifndef JTRACKER_GLOBALS_H
#define JTRACKER_GLOBALS_H

#include<iostream>
#include<tracker/jtracker_theme.h>
#include<chrono>
#include<tracker/filesystem.h>
#include<tracker/config_data.h>
#include<utilities/string_utilities.h>
#include<animations/fps_model.h>
#include<tracker/filesystem.h>

using namespace std::chrono;
using namespace std::chrono_literals;
namespace jtracker {

float get_track_line_height();


//////////////////////////////////////////////////////////////////////
/// Tracker global data
//////////////////////////////////////////////////////////////////////
struct tracker_data
{
    tracker_data();

    // Tracker Data
    size_t seq_nbr = 1;
    size_t number_of_tracks = 1;
    size_t number_of_lines = 8;
    size_t grid_step = 0;

    float track_line_height = get_track_line_height();

    constexpr static const size_t default_column_number = 8;

    fps_value app_fps;
    tracker_filesystem filesystem;

    // Data for persistence
    tracker_audio_config_data audio_config;
    json gen_data;
};

extern tracker_data data;

inline void save_gen_data()
{
    std::string to_write = data.gen_data.dump(4);
    jtracker::string::write_file_as_string(data.filesystem.tracker_gen_data , to_write);
}

inline void save_audio_config()
{
    std::string to_write = data.audio_config.to_json().dump(4);
    jtracker::string::write_file_as_string(data.filesystem.tracker_audio_config, to_write);
}

}


#endif // JTRACKER_GLOBALS_H
