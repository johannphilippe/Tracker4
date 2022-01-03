/*=============================================================================
   Copyright (c) 2021 Johann Philippe

   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/
#include <elements.hpp>
#include<tracker/jtracker.h>
#include<widgets/tracker/track_view.h>
#include<widgets/tracker/curve_editor_view.h>
#include<widgets/tracker/tracker_menus.h>


#include<widgets/expandable.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

using namespace jtracker;

inline auto make_page(std::string text)
{
    return layer(align_center_middle(label(text).font_size(100)), frame{});
}

int main(int argc, char* argv[])
{
    tracker_app *app = jtracker::tracker_app::get_instance(argc, argv);
    // All those widgets must be global, so moved to an extern instance or a singleton
    // That will let other parts of the programm access their states when necesssary
    curve_editor_view editor;
    track_view tv;
    tv.t_set.add_track(track_type::sniper_track);

    auto tabs = vnotebook(
               app->_view,
               deck(
                        margin({5, 5, 5, 5},
                               layer(
                                   align_left_top(
                                       link(tv) )
                                   )
                               ),
                        editor,
                        make_page("code editor")
                   ),
               tab("Tracker"),
               tab("Curve editor"),
               tab("Code editor")
               );

    app->_view.content(
               vtile(
                   make_tracker_menus(),
                   tabs
                   ),
               jtracker::background
               );
    app->run();
    return 0;
}
