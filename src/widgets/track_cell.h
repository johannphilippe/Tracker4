#ifndef TRACK_CELL_H
#define TRACK_CELL_H

#include<iostream>
#include<elements.hpp>
#include<utilities/paint_utilities.h>

#include<tracker/jtracker.h>
#include<models/track_event.h>

using namespace cycfi::elements;
using namespace std::chrono_literals;

class custom_text_box : public basic_input_box
{
public:
    custom_text_box() : basic_input_box()
    {
    }


private:
};

class cell_background : public tracker<>
{
public:
    constexpr static const size_t frame_steps = 15;

    cell_background(std::string text_ = "", float width = 40,
                    color inactive_color = get_theme().panel_color) :
        tracker<>(),
        _width(width),
        is_active(false),
        inactive(inactive_color),
        active(inactive_color.opacity(1)),
        current_color(inactive)
    {
    }

    void draw(context const& ctx) override
    {
        ctx.canvas.fill_color(current_color);
        ctx.canvas.fill_round_rect(ctx.bounds, 4);
        ctx.canvas.fill();
        ctx.canvas.line_width(0.5);
        ctx.canvas.stroke_color(colors::navajo_white);
        ctx.canvas.stroke_round_rect(ctx.bounds, 4);
        ctx.canvas.stroke();
    }

    void set_selection(bool b) {
        if(b) select();
        else unselect();
    }
    void select() {
        set_color(active);
        is_active = true;
    }

    void unselect() {
        set_color(inactive);
        is_active = false;
    }

    void set_color(color c)
    {
        current_color.set_color_target(c, frame_steps);
        //animate(v);
    }

    void set_color_no_redraw(color c)
    {
        current_color.set_color_target(c, frame_steps);
    }


    bool wants_focus() const override {return false;}

    void set_inactive_color(color _inactive)
    {
        inactive = _inactive;
        active = inactive.opacity(1);
        if(inactive == get_theme().panel_color)
            active = colors::goldenrod.opacity(0.5);
        current_color = inactive;
    }

    float _width;
    bool is_active;
    color inactive, active;
    jtracker::color_utilities::interpolated_color current_color;
};


// The actual cell for tracker track
class track_cell :
        public array_composite<2, layer_element>
{
public:
    track_cell( float width = 60, color inactive_color = get_theme().panel_color) :
        array_composite<2, layer_element>(
            layer(
                scroller(
                    hsize(100,
                          align_middle(
                          margin({1,1,1,1},link(text_box)))),
                    no_scrollbars | no_vscroll
                    ),
                link(background)
                )
            ),
        _width(width)
    {
        background.set_inactive_color(inactive_color);
        text_box.on_text = [&](std::string_view v)
        {
            std::cout << "on text : " << v << std::endl;
        };
    }

    virtual view_limits limits(basic_context const&) const override
    {
        const float line_height = text_box.get_font().line_height() + 5;
        return {{_width, line_height},
            {_width, line_height}};
    }

    bool click(context const& ctx, mouse_button btn) override
    {
        text_box.click(ctx, btn);
        on_click(ctx, btn);
        return true;
    }


    void set_text(std::string_view s) {text_box.set_text(s);}
    std::u32string_view get_text()  {return text_box.get_text();}

    using click_function = std::function<bool(context const& ctx, mouse_button btn)>;
    click_function on_click = [](context const &, mouse_button){ return false; };

    float _width;
    //basic_input_box text_box;
    basic_input_box text_box;
    cell_background background;
};

#endif // TRACK_CELL_H
