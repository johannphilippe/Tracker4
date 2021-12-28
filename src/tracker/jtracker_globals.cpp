#include<tracker/jtracker_globals.h>
#include<models/sniper_track_event.h>

namespace jtracker {


float get_track_line_height()
{
    font f (jtracker::theme.cell_box_font);
    auto m = f.metrics();
    return (m.ascent + m.descent + m.leading + 5);
}

tracker_data data;

}

sniper_track_event sniper_null_event = {};
