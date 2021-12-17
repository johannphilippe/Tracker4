#ifndef CUSTOM_RADIO_H
#define CUSTOM_RADIO_H

#include<elements.hpp>
using namespace cycfi::elements;

struct squared_radio_button_element : element, basic_receiver<button_state>
{

    squared_radio_button_element(std::string text)
     : _text(std::move(text))
    {}
    const color background = colors::light_salmon;
    const color hilite_color = colors::light_coral;

    void draw(const context &ctx) override
    {
      auto  state = value();
      auto  value = state.value;
      auto  hilite = state.hilite;
      auto  tracking = state.tracking;
      if(value)
      {
          ctx.canvas.fill_color(background.opacity(0.7));
      } else if(tracking) {
          ctx.canvas.fill_color(background);
      } else if(hilite) {
          ctx.canvas.fill_color(hilite_color);
      } else {
          ctx.canvas.fill_color(background.opacity(0.5));
      }

      ctx.canvas.add_rect(ctx.bounds);
      ctx.canvas.fill();
      rect        box = ctx.bounds.move(15, 0);
      float cx = box.left + 10;
      float cy = ctx.bounds.top + (ctx.bounds.height() / 2);
      ctx.canvas.text_align(ctx.canvas.left | ctx.canvas.middle);

      ctx.canvas.fill_color(colors::ivory);
      ctx.canvas.fill_text(_text.c_str(), point{ cx, cy });
    }

    view_limits limits(const basic_context &ctx) const override
    {
        auto t_size = measure_text(ctx.canvas, _text, get_theme().text_box_font);
        view_limits limits;
        limits.max = point(full_extent, t_size.y);
        return limits;
    }


    std::string _text;
};

inline auto squared_radio_button(std::string text)
{
    return choice(squared_radio_button_element{text});
}


#endif // CUSTOM_RADIO_H
