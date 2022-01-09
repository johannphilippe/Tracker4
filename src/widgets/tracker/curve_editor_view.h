#ifndef CURVE_EDITOR_VIEW_H
#define CURVE_EDITOR_VIEW_H

#include<elements.hpp>
#include<widgets/curve_editor.h>
#include<widgets/custom_radio.h>
#include<tracker/jtracker.h>
#include<widgets/spinbox.h>

using namespace cycfi::elements;

namespace jtracker {

///////////////////////////////////////////////////////////
// Curve editor radio bar
///////////////////////////////////////////////////////////

using radio_selector_t = basic_choice<proxy<squared_radio_button_element, basic_button>>;
struct curve_editor_radio_button : radio_selector_t
{
    //curve_editor_radio_button();
    curve_editor_radio_button(radio_selector_t t)
        : radio_selector_t(std::forward<proxy<squared_radio_button_element, basic_button>>(t))
    {}
};

struct curve_editor_radios
{
    curve_editor_radios();
    curve_editor_radio_button linear, log_exp, spline, quadbezier, cubbezier;

};

struct curve_editor_radio_group:
        curve_editor_radios
        , array_composite<3, layer_element>
{
    curve_editor_radio_group();
};

struct curve_editor_radio_group_holder
{
    curve_editor_radio_group radios;
    void set_radio_selection(curve_mode c);
};

///////////////////////////////////////////////////////////
// Curve editor view
///////////////////////////////////////////////////////////
class curve_editor_view
        : public curve_editor_radio_group_holder
        , public array_composite<3, vtile_element>
{
public:
    curve_editor_view();

    auto make_header();
    void mode_selection(bool b, curve_mode m);
    void make_info_popup();
    auto make_mode_buttons();

    curve_editor editor;
};

}

#endif // CURVE_EDITOR_VIEW_H
