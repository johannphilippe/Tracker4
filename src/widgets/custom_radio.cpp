#include<widgets/custom_radio.h>

void squared_radio_button_element::draw(const context &ctx)
{
    auto  state = value();
    auto  value = state.value;
    auto  hilite = state.hilite;
    auto  tracking = state.tracking;
    if(value)
    {
      ctx.canvas.fill_color(background.opacity(0.8));
    } else if(tracking) {
      ctx.canvas.fill_color(background);
    } else if(hilite) {
      ctx.canvas.fill_color(hilite_color);
    } else {
      ctx.canvas.fill_color(background.opacity(0.5));
    }

    ctx.canvas.add_round_rect(ctx.bounds, 6);
    ctx.canvas.fill();
    rect        box = ctx.bounds.move(15, 0);
    float cx = box.left + 10;
    float cy = ctx.bounds.top + (ctx.bounds.height() / 2);
    ctx.canvas.text_align(ctx.canvas.left | ctx.canvas.middle);

    ctx.canvas.fill_color(colors::ivory);
    ctx.canvas.fill_text(_text.c_str(), point{ cx, cy });
}

view_limits squared_radio_button_element::limits(const basic_context &ctx) const
{
    auto t_size = measure_text(ctx.canvas, _text, get_theme().text_box_font);
    view_limits limits;
    limits.max = point(full_extent, t_size.y * 1.5);
    return limits;
}

