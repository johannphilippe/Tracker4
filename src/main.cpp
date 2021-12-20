/*=============================================================================
   Copyright (c) 2021 Johann Philippe

   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/
#include <elements.hpp>
#include<tracker/jtracker.h>
#include<widgets/tracker/tab_view.h>
#include<widgets/tracker/track.h>
#include<widgets/tracker/track_view.h>

#include<widgets/tracker/curve_editor_view.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

inline auto make_page(std::string text)
{
    return layer(align_center_middle(label(text).font_size(100)), frame{});
}

int main(int argc, char* argv[])
{
   jtracker::tracker_app *app = jtracker::tracker_app::get_instance(argc, argv);
   jtracker::curve_editor_view editor;
   jtracker::track t(10, 8);

   jtracker::track_set ts;
   ts.add_track(track_type::tracker_track);
   ts.add_track(track_type::tracker_track);
   ts.add_track(track_type::tracker_track);
   ts.add_track(track_type::tracker_track);

   auto b = button("test");
   b.on_click = [&](bool)
   {
     app->_view.layout();
     app->_view.refresh();
   };

   auto tabs = vnotebook(
               app->_view,
               deck(
                        //layer(align_left_top(t)),
                        layer(align_left_top(link(ts))),
                        editor,
                        make_page("code editor")
                   ),
               tab("Tracker"),
               tab("Curve editor"),
               tab("Code editor")

               );

   app->_view.content(
               vtile(
               tabs, b),
               jtracker::background
               );
   std::cout << "app address " << app << std::endl;
   app->run();
   return 0;
}
