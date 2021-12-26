#ifndef JTRACKER_THEME_H
#define JTRACKER_THEME_H

#include<elements.hpp>
using namespace cycfi::elements;
using namespace cycfi::artist;

namespace jtracker {


struct tracker_theme
{
    constexpr static const tracker_theme dark_theme()
    {
        tracker_theme t;
        /***********************
        * COLORS
        ***********************/
        t.app_background_color 				=   rgba(35, 35, 37, 255);

        //t.clear_buttons 					= 	colors::orchid;
        t.clear_buttons_color 				= 	colors::pale_violet_red;

        //t.spin_borders_color 				=   rgba(28, 30, 34, 192);
        t.spin_left_arrow_color 	 		= 	colors::light_salmon;
        t.spin_right_arrow_color	 		= 	colors::light_sky_blue;
        t.spin_sleeping_arrow_color			= 	colors::dark_slate_blue;
        t.spin_borders_color 		 		=	colors::dark_slate_blue;
        t.spin_text_color 		 			= 	colors::antique_white;

        t.track_background_color			= 	rgba(28, 30, 34, 255);
        t.sniper_track_background_color		= 	rgb(15, 15, 40);
        t.sniper_track_lines_color			= 	colors::wheat;

        /***********************
        * FONTS
        ***********************/
        t.cell_box_font						=	font_descr{ "Open Sans", 14.0 };
        t.sniper_track_font					=	font_descr{ "Roboto Light", 12.0 };

        return t;
    }

    constexpr static const tracker_theme light_theme()
    {
        tracker_theme t;
        /***********************
        * COLORS
        ***********************/
        t.app_background_color				=	colors::navajo_white;

        t.clear_buttons_color				=	colors::pale_violet_red;

        t.spin_borders_color				=	rgba(120, 140, 150, 192);
        t.spin_left_arrow_color				=	colors::light_salmon;
        t.spin_right_arrow_color			=	colors::sky_blue;
        t.spin_sleeping_arrow_color			=	colors::light_slate_blue;
        t.spin_borders_color				= 	colors::light_slate_blue;
        t.spin_text_color					=	colors::navy_blue;

        t.track_background_color			=	colors::corn_silk;
        t.sniper_track_background_color		= 	colors::salmon;
        t.sniper_track_lines_color			= 	colors::wheat;

        /***********************
        * FONTS
        ***********************/

        t.cell_box_font						= font_descr{ "Open Sans", 14.0 };
        return t;
    }

    /***********************
     * COLORS
    ***********************/

    // Background
    color app_background_color;

    // Buttons
    color clear_buttons_color;

    // Spinboxes
    color spin_background_color;
    color spin_right_arrow_color;
    color spin_left_arrow_color;
    color spin_sleeping_arrow_color;
    color spin_borders_color;
    color spin_text_color;

    // Tracks
    color track_background_color;
    color sniper_track_background_color;
    color sniper_track_lines_color;

    /***********************
     * FONTS
    ***********************/

    font_descr cell_box_font;
    font_descr sniper_track_font;
};

// Probably needs to be mutable (not const or constexpr)
static const tracker_theme theme = tracker_theme::dark_theme();

}

#endif // JTRACKER_THEME_H