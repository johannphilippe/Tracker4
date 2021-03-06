#ifndef JTRACKER_THEME_H
#define JTRACKER_THEME_H

#include<elements.hpp>
using namespace cycfi::elements;
using namespace cycfi::artist;

namespace jtracker {


struct tracker_theme
{
    constexpr static const tracker_theme dark_theme();
    constexpr static const tracker_theme light_theme();

    /***********************
     * COLORS
    ***********************/

    // Background
    color app_background_color;

    // Buttons
    color clear_buttons_color;
    color store_buttons_color;
    color info_buttons_color;

    // Spinboxes
    color spin_background_color;
    color spin_right_arrow_color;
    color spin_left_arrow_color;
    color spin_sleeping_arrow_color;
    color spin_borders_color;
    color spin_text_color;

    // Tracks
    color track_label_index_color;
    color track_label_index_hot_color;
    color track_background_color;
    color sniper_track_background_color;
    color sniper_track_lines_color;

    // Radios
    color rectangle_radio_inactive_color;
    color rectangle_radio_active_color;

    // events
    color none_event_color;
    color csound_event_color;
    color lua_rt_event_color;
    color chuck_rt_color;
    color antescofo_rt_color;
    color audio_sample_color;

    /***********************
     * FONTS
    ***********************/

    font_descr cell_box_font;
    font_descr sniper_track_font;
    font_descr main_spin_font;
};

// Probably needs to be mutable (not const or constexpr)
extern const tracker_theme theme;

}

#endif // JTRACKER_THEME_H
