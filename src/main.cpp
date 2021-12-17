/*=============================================================================
   Copyright (c) 2021 Johann Philippe

   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/
#include <elements.hpp>
#include<tracker/jtracker.h>

#include<widgets/tracker/curve_editor_view.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

int main(int argc, char* argv[])
{

    jtracker::curve_editor_view editor;
   jtracker::tracker_app *app = jtracker::tracker_app::get_instance(argc, argv);
   app->_view.content(
               margin({10,10,10,10},
                      hstretch(2,
                               vtile(
                                   editor
                                   )
                               )
                      ),
               jtracker::background
               );
   app->run();
   return 0;
}
