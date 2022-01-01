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

    void draw(const context &ctx) override;
    view_limits limits(const basic_context &ctx) const override;

    std::string _text;
};

inline auto squared_radio_button(std::string text)
{
    return choice(squared_radio_button_element{text});
}


#endif // CUSTOM_RADIO_H
