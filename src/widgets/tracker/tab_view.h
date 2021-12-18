#ifndef TAB_VIEW_H
#define TAB_VIEW_H

#include<elements.hpp>
#include<tracker/jtracker.h>

using namespace cycfi::elements;

namespace jtracker {
class tab_view : array_composite<2, vtile_element>
{

public:
    tab_view();

    inline auto make_page(std::string text)
    {
        return layer(align_center_middle(label(text).font_size(100)), frame{});
    }

};
}
#endif // TAB_VIEW_H
