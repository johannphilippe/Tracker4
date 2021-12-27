#include<tracker/jtracker_globals.h>
#include<models/sniper_track_event.h>

namespace jtracker {


float get_track_line_height()
{
    font f (jtracker::theme.cell_box_font);
    auto m = f.metrics();
    return (m.ascent + m.descent + m.leading + 5);
}

tracker_data data
{
            .seq_nbr = 1,
            .number_of_tracks = 1,
            .number_of_lines = 16,
            .grid_step = 0,
            .track_line_height = get_track_line_height()
};



}

/*
sniper_track_event sniper_null_event = {
    .bounds = {0, 0, 0, 0},
    .type = track_event_type::none,
    .selected = false
};
*/
sniper_track_event sniper_null_event = {};
