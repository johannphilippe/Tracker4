#include<tracker/jtracker_globals.h>
#include<models/sniper_track_event.h>

namespace jtracker {

float get_track_line_height()
{
    font f (jtracker::theme.cell_box_font);
    auto m = f.metrics();
    return (m.ascent + m.descent + m.leading + 5);
}

// At the first opening of the tracker, it creates necessary config files if necessary
tracker_data::tracker_data()
{
    // Audio config file
    if(!fs::exists(filesystem.tracker_audio_config))
    {
        std::string audio_config_ = audio_config.to_json().dump(4);
        jtracker::string::write_file_as_string(filesystem.tracker_audio_config,
                                               audio_config_);
    } else // read it
    {
        json js = json::parse( jtracker::string::read_file_as_string(filesystem.tracker_audio_config));
        audio_config = tracker_audio_config_data::from_json(js);
    }


    // Gen data file
    if(!fs::exists(filesystem.tracker_gen_data))
    {

        gen_data = json();
        std::string to_write = gen_data.dump(4);
        jtracker::string::write_file_as_string(filesystem.tracker_gen_data, to_write);
    } else
    {
        gen_data = json::parse(jtracker::string::read_file_as_string(filesystem.tracker_gen_data));
    }

}

tracker_data data;

}

sniper_track_event sniper_null_event = {};
