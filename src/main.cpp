/*=============================================================================
   Copyright (c) 2021 Johann Philippe

   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/
#include <elements.hpp>
#include<tracker/jtracker.h>
#include<widgets/tracker/track_view.h>
#include<widgets/tracker/gen_view.h>
#include<widgets/tracker/tracker_menus.h>


using namespace cycfi::elements;
using namespace cycfi::artist;

using namespace jtracker;

inline auto make_page(std::string text)
{
    return layer(align_center_middle(basic_text_box(text)), frame{});
    //return layer(align_center_middle(label(text).font_size(100)), frame{});
}

int main(int argc, char* argv[])
{
    tracker_app *app = jtracker::tracker_app::get_instance(argc, argv);
    // All those widgets must be global, so moved to an extern instance or a singleton
    // That will let other parts of the programm access their states when necesssary

    gen_view gens;
    track_view tv;
    tv.t_set.add_track(track_type::sniper_track);

    constexpr const rect main_pad = {5, 5, 5, 5};

    auto tracker_tab = margin(main_pad,
                              layer(
                                  align_left_top(
                                      link(tv)
                                      )
                                  )
                              );

    auto editor_tab = margin(main_pad,
                             gens
                             );

    auto tabs = vnotebook(
               app->_view,
               deck(
                    tracker_tab,
                    editor_tab,
                    make_page("code editor")
                   ),
               tab("Tracker"),
               tab("GEN editor"),
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
