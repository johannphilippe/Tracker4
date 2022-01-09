#include<widgets/track_cell.h>

////////////////////////////////////////////////////////////////
// Tracker cells dynamic background
////////////////////////////////////////////////////////////////
cell_background::cell_background(float width, color inactive_color)
    : tracker<>()
    , _width(width)
    , is_active(false)
    , inactive(inactive_color)
    , active(inactive_color.opacity(1))
    , current_color(std::make_shared<color_animation>(inactive))
{}

void cell_background::draw(context const& ctx)
{
    ctx.canvas.fill_color(*current_color);
    ctx.canvas.fill_round_rect(ctx.bounds, 4);
    ctx.canvas.fill();

    ctx.canvas.line_width( is_in_grid ? 1.5 : 0.5);
    ctx.canvas.stroke_color( is_in_grid ? colors::bisque : colors::navajo_white);
    ctx.canvas.stroke_round_rect(ctx.bounds, 4);
    ctx.canvas.stroke();
}

void cell_background::set_selection(bool b) {
    if(b) select();
    else unselect();
}
void cell_background::select() {
    set_color(active);
    is_active = true;
}

void cell_background::unselect() {
    set_color(inactive);
    is_active = false;
}

void cell_background::set_color(color c)
{
    current_color->set_color_target(c);
}

void cell_background::set_color_no_redraw(color c)
{
    current_color->set_color_target(c);
}

bool cell_background::wants_focus() const {return false;}

////////////////////////////////////////////////////////////////
// Tracker cells
////////////////////////////////////////////////////////////////
void cell_background::set_inactive_color(color _inactive)
{
    inactive = _inactive;
    active = inactive.opacity(1);
    if(inactive == get_theme().panel_color)
        active = colors::goldenrod.opacity(0.5);
    *current_color = inactive;
}


track_cell::track_cell( float width, color inactive_color) :
    array_composite<2, layer_element>(
        layer(
        scroller(
            hsize(100 , // size of textbox inside scroller
              align_middle(
              margin({1,1,1,1},link(text_box)))),
            no_scrollbars | no_vscroll
            ),
        link(background)
        )
    )
  , _width(width)
{
    background.set_inactive_color(inactive_color);
}

view_limits track_cell::limits(basic_context const&) const
{
    const float line_height = text_box.get_font().line_height() + 5;
    return {{_width, line_height},
        {_width, line_height}};
}

bool track_cell::click(context const& ctx, mouse_button btn)
{
    on_click(ctx, btn);
    return text_box.click(ctx, btn);
}

bool track_cell::text(const context &ctx, text_info info)
{
    bool res = text_box.text(ctx, info);
    ctx.view.refresh(text_box);
    return res;
}

void track_cell::set_text(std::string_view s) {text_box.set_text(s);}
std::u32string_view track_cell::get_text()  {return text_box.get_text();}
