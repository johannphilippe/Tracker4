#ifndef TRACK_CELL_H
#define TRACK_CELL_H

#include<iostream>
#include<elements.hpp>
#include<utilities/paint_utilities.h>

#include<tracker/jtracker.h>

using namespace cycfi::elements;
using namespace std::chrono_literals;

class custom_text_box : public basic_input_box
{
public:
    custom_text_box() : basic_input_box()
    {}


    void begin_focus() override
    {
        basic_input_box::begin_focus();
    }
};

class cell_background : public tracker<>
{
public:
    constexpr static const size_t frame_steps = 15;
    constexpr static auto fps = 30ms;

    cell_background(std::string text_ = "", float width = 40) :
        tracker<>(),
        _width(width),
        is_active(false),
        active(colors::dark_orange.opacity(0.5)),
        inactive(get_theme().panel_color),
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

    void set_selection(bool b, view &v) {
        if(b) select(v);
        else unselect(v);
    }
    void select(view &v) {
        set_color(colors::dark_orange.opacity(0.5), v);
        is_active = true;
    }
    void unselect(view &v) {
        set_color(get_theme().panel_color, v);
        is_active = false;
    }

    void set_color(color c, view &v)
    {
        current_color.set_color_target(c, frame_steps);
        animate(v);
    }

    void set_color_no_redraw(color c)
    {
        current_color.set_color_target(c, frame_steps);
    }


    void animate(view &view)
    {
        if(current_color.interpolate()) return;
        view.refresh(*this);
        view.post(fps, [&, this]() {
            animate(view);
        });
    }

    bool wants_focus() const override {return false;}


    float _width;
    bool is_active;
    color active, inactive;
    jtracker::color_utilities::interpolated_color current_color;
};


// The actual cell for tracker track
class track_cell :
        public array_composite<2, layer_element>
{
public:
    track_cell( float width = 60) :
        array_composite<2, layer_element>(
            layer(
                scroller(
                    hsize(16384,
                          align_middle(
                          margin({1,1,1,1},link(text_box)))),
                    no_scrollbars | no_vscroll
                    ),
                link(background)
                )
            ),
        _width(width)
    {
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
        on_focus(true);
        return true;
    }


    void set_text(std::string_view s) {text_box.set_text(s);}
    std::u32string_view get_text()  {return text_box.get_text();}

    using click_function = std::function<bool(context const& ctx, mouse_button btn)>;
    click_function on_click = [](context const &, mouse_button){ return false; };

    std::function<void(bool)> on_focus =[](bool){};

    float _width;
    basic_input_box text_box;
    cell_background background;
};

#endif // TRACK_CELL_H
