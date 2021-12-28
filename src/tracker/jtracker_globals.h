#ifndef JTRACKER_GLOBALS_H
#define JTRACKER_GLOBALS_H

#include<iostream>
#include<tracker/jtracker_theme.h>
namespace jtracker {

float get_track_line_height();

struct tracker_data
{
    size_t seq_nbr = 1;
    size_t number_of_tracks = 1;
    size_t number_of_lines = 8;
    size_t grid_step = 0;

    float track_line_height = get_track_line_height();

    constexpr static const size_t default_column_number = 8;
};

extern tracker_data data;

}


#endif // JTRACKER_GLOBALS_H
