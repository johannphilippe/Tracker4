#include "tab_view.h"

namespace jtracker {
tab_view::tab_view() :
    array_composite<2, vtile_element>( vnotebook(
                                          jtracker::tracker_app::get_instance()->_view,
                                                deck(
                                                    make_page("dream"),
                                                    make_page("volupté")
                                                    ),
                                                tab("dream"),
                                                tab("volupté")
                                                )
                                      )
{

}
}
