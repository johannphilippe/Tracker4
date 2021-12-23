#include "spinbox.h"

template<typename T>
view_limits spinbox<T>::limits(basic_context const& ctx) const
{
    point p = measure_text(ctx.canvas, "1", theme().label_font);
    p.y *= 1.35;
    return {{40, p.y}, {full_extent, p.y}};
}

template<typename T>
void spinbox<T>::draw(context const& ctx)
{
    const float mult = 1.35f;
    auto size = ctx.bounds.size();
    auto pos = ctx.bounds.top_left();
    auto& thm = get_theme();
    float font_size = thm.icon_font._size * mult;

    left_arrow_bounds = ctx.bounds;
    left_arrow_bounds.width( size.x / 4.0f) ;
    right_arrow_bounds = ctx.bounds;
    right_arrow_bounds.left =  pos.x + size.x - (size.x / 4.0f);

    color c = (focused == 1) ? colors::light_salmon.opacity( ((clicked) ? 1.0 : 0.75) ) : colors::dark_slate_blue;
    draw_icon(ctx.canvas, left_arrow_bounds, icons::left_dir, font_size, c);
    c = (focused == 2) ? colors::light_blue.opacity( ((clicked) ? 1.0 : 0.75)  ) : colors::dark_slate_blue;
    draw_icon(ctx.canvas, right_arrow_bounds, icons::right_dir, font_size, c) ;

    const point p = measure_text(ctx.canvas, _text, get_theme().label_font);
    const point middle(pos.x + (size.x / 2.0f), pos.y + (size.y / 2.0f) );
    const point t_point(middle.x - (p.x / 2.0f), ( middle.y + (p.y / 4.0f)) );

    ctx.canvas.stroke_color(colors::antique_white);
    ctx.canvas.stroke_text(_text,t_point );

    ctx.canvas.stroke_color(colors::dark_slate_blue);
    ctx.canvas.stroke_round_rect(ctx.bounds, 8.0f);
    ctx.canvas.stroke();
}

template<typename T>
bool spinbox<T>::click(context const& ctx, mouse_button btn)
{

    if(btn.down){
        clicked = true;
        switch (focused) {
        case 1:
        {
            decrement();
            ctx.view.refresh(*this);
            break;
        }
        case 2:
        {
            increment();
            ctx.view.refresh(*this);
            break;
        }
        default:
        {
            click_pos = btn.pos;
            break;
        }
        }
    } else // btn up
    {
        clicked = false;
        ctx.view.refresh(*this);
    }
    return true;
}

template<typename T>
bool spinbox<T>::cursor(context const &ctx, point p, cursor_tracking)
{
    int f = focused;
    if(left_arrow_bounds.includes(p)) {
        focused = 1;
    } else if(right_arrow_bounds.includes(p)) {
        focused = 2;
    } else {
        focused = -1;
    }
    if(f != focused) ctx.view.refresh(*this);
    return true;
}

template<typename T>
bool spinbox<T>::scroll(context const&ctx, point dir, point )
{
    if(dir.y < 0) {
        decrement();
        ctx.view.refresh(*this);
    }
    else if(dir.y > 0) {
        increment();
        ctx.view.refresh(*this);
    }
    return true;
}

template<typename T>
void spinbox<T>::drag(const context &ctx, mouse_button btn)
{
    const float step = 5.0f;
    if(focused != -1) return;
    float diff = btn.pos.y - click_pos.y;
    if(diff > step)
    {
        click_pos = btn.pos;
        decrement();
        ctx.view.refresh(*this);
    } else if(diff < -step)
    {
        click_pos = btn.pos;
        increment();
        ctx.view.refresh(*this);
    }
}