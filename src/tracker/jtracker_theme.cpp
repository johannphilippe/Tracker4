#include<tracker/jtracker_theme.h>

namespace jtracker {
    constexpr const tracker_theme tracker_theme::dark_theme()
    {
        tracker_theme t;
        /***********************
        * COLORS
        ***********************/
        t.app_background_color 						=   rgba(35, 35, 37, 255);

        //t.clear_buttons 							= 	colors::orchid;
        t.clear_buttons_color 						= 	colors::pale_violet_red;
        t.store_buttons_color 						= 	colors::blue_violet;
        t.info_buttons_color 						= 	colors::light_steel_blue;

        //t.spin_borders_color 						=   rgba(28, 30, 34, 192);
        t.spin_left_arrow_color 	 				= 	colors::light_salmon;
        t.spin_right_arrow_color	 				= 	colors::light_sky_blue;
        t.spin_sleeping_arrow_color					= 	colors::dark_slate_blue;
        t.spin_borders_color 		 				=	colors::dark_slate_blue;
        t.spin_text_color 		 					= 	colors::antique_white;

        // Tracks colors

        t.track_background_color					= 	rgba(28, 30, 34, 255);
        t.track_label_index_color					=	colors::grey[20].opacity(0.5);
        t.track_label_index_hot_color				=	colors::grey[50].opacity(0.5);
        t.sniper_track_background_color				= 	rgba(15, 15, 35, 190);  //rgb(15, 15, 40);
        t.sniper_track_lines_color					= 	colors::wheat;


        // Radios
        t.rectangle_radio_inactive_color			=   colors::dark_violet;
        t.rectangle_radio_active_color				= 	colors::medium_violet_red;

        // Events

        t.none_event_color							=   rgba(28, 30, 34, 192);
        t.csound_event_color						=	colors::midnight_blue;
        t.lua_rt_event_color						=	colors::dark_green;
        t.chuck_rt_color							=	colors::dark_slate_blue;
        t.antescofo_rt_color						=	colors::dark_orchid;
        t.audio_sample_color						=	colors::firebrick;

        /***********************
        * FONTS
        ***********************/
        t.cell_box_font						=	font_descr{ "Open Sans", 14.0 };
        t.sniper_track_font					=	font_descr{ "Roboto Light", 12.0 };
        t.main_spin_font					=	font_descr{ "Roboto Light", 14.0};

        return t;
    }

    constexpr const tracker_theme tracker_theme::light_theme()
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

const tracker_theme theme = tracker_theme::dark_theme();

}
