#ifndef CUSTOM_RADIO_H
#define CUSTOM_RADIO_H

#include<elements.hpp>
#include<tracker/jtracker_theme.h>
using namespace cycfi::elements;

struct squared_radio_button_element : element, basic_receiver<button_state>
{
    squared_radio_button_element(std::string text)
     : _text(std::move(text))
    {}

    const color background = jtracker::theme.rectangle_radio_inactive_color;
    const color hilite_color = jtracker::theme.rectangle_radio_active_color;

    void draw(const context &ctx) override;
    view_limits limits(const basic_context &ctx) const override;

    std::string _text;
};

inline auto squared_radio_button(std::string text)
{
    return choice(squared_radio_button_element{text});
}


struct expanded_radio_button_element : radio_button_element
{
    expanded_radio_button_element(std::string text)
        : radio_button_element{text}
    {}

    view_limits limits(basic_context const& ctx) const override
    {

        view_limits l = radio_button_element::limits(ctx);
        l.max.x = full_extent;
        return l;
    }
};

inline auto expanded_radio_button(std::string text)
{
   return choice(expanded_radio_button_element{ text });
}



#endif // CUSTOM_RADIO_H
