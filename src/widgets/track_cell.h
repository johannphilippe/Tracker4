#ifndef TRACK_CELL_H
#define TRACK_CELL_H

#include<iostream>
#include<elements.hpp>
#include<utilities/paint_utilities.h>

#include<models/track_event.h>
#include<tracker/jtracker_theme.h>

using namespace cycfi::elements;
using namespace std::chrono_literals;
using namespace jtracker::color_utilities;

class cell_text_box : public basic_input_box
{
public:
    cell_text_box()
        : basic_input_box("" , jtracker::theme.cell_box_font ) {}
};

class cell_background : public tracker<>
{
public:

    cell_background(float width = 40,
                    color inactive_color = get_event_color(track_event_type::none));

    void draw(context const& ctx) override;

    void set_selection(bool b);
    void select();
    void unselect();
    void set_color(color c);
    void set_color_no_redraw(color c);
    bool wants_focus() const override;
    void set_inactive_color(color _inactive);

    float _width;
    bool is_active;
    color inactive, active;
    std::shared_ptr<color_animation> current_color;
    bool is_in_grid = false;
};


// The actual cell for tracker track
class track_cell :
        public array_composite<2, layer_element>
{
public:
    track_cell( float width = 60, color inactive_color = get_theme().panel_color);

    virtual view_limits limits(basic_context const&) const override;
    bool click(context const& ctx, mouse_button btn) override;

    void set_text(std::string_view s);
    std::u32string_view get_text();

    bool text(context const &ctx, text_info info) override;

    using click_function = std::function<void(context const& ctx, mouse_button btn)>;
    click_function on_click = [](context const &, mouse_button){ return false; };

    float _width;
    //basic_input_box text_box;
    cell_text_box text_box;
    cell_background background;
};

#endif // TRACK_CELL_H
