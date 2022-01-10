#ifndef GEN_VIEW_H
#define GEN_VIEW_H
#include<elements.hpp>
#include<widgets/tracker/curve_editor_view.h>
using namespace cycfi::elements;
namespace jtracker {

////////////////////////////////////////////////////////////
// Gen types
///////////////////////////////////////////////////////////
enum gen_type
{
    curve = 0,
    waveform = 1,
    raw_data = 2,
};

inline json get_gen_data(gen_type t, json &data)
{
    json j;
    j["type"] = static_cast<int>(t);
    j["gen_data"] = data;
    return j;
}

////////////////////////////////////////////////////////////
// Gen tabs
///////////////////////////////////////////////////////////
class gen_tabs : public array_composite<2, vtile_element>
{
public:
    gen_tabs();
    curve_editor_view curve_editor;
};

struct gen_tabs_holder
{
    gen_tabs tabs;
};

////////////////////////////////////////////////////////////
// Control bar
///////////////////////////////////////////////////////////
class gen_control_bar_components
{
public:
    gen_control_bar_components();
    size_spinbox selector;
    layered_button store_button, to_clipboard_button, clear_button;
};

////////////////////////////////////////////////////////////
// Main GEN View
///////////////////////////////////////////////////////////
class gen_view : public gen_tabs_holder,
        public gen_control_bar_components,
        public array_composite<3, vtile_element>
{
public:
    gen_view();

    gen_type current_gen_type = gen_type::curve;

    void store_current_gen();
    void recall_gen(size_t index);
};


}
#endif // GEN_VIEW_H
