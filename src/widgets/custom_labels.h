#ifndef CUSTOM_LABELS_H
#define CUSTOM_LABELS_H

#include<elements.hpp>
#include<tracker/jtracker_globals.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

// Labels with fixed size (fixed in template parameter, as a number of possible characters)
// Since it is based on the width of a character, it should be accurate for monospace fonts.
template<size_t Size>
class fixed_size_base : public default_label
{
public:
    view_limits limits(const basic_context &ctx) const override
    {
        point size = measure_text(ctx.canvas, "9" ,get_font());
        size.x *= Size;
        return { { size.x, size.y }, { size.x, size.y } };
    }
};

template<size_t Size>
using basic_fixed_size = basic_label_base<fixed_size_base<Size>>;

template<size_t Size>
using fixed_size_label = label_gen<basic_fixed_size<Size>>;

template<size_t Size>
inline auto make_fixed_size_label_with_background(std::string text, const color back_color)
{
    return share(layer(fixed_size_label<Size>(text), rbox(back_color, 4)));
}

template<size_t Size>
class fixed_size_label_with_background : public array_composite<2, layer_element>
{

public:
    fixed_size_label_with_background(std::string txt, const color back_color) :
     array_composite<2, layer_element>(
         layer(
             fixed_size_label<Size>(txt),
             rbox(back_color, 4)
             )
         )
    {}

    view_limits limits(basic_context const& ctx) const override
    {
        point size = measure_text(ctx.canvas, "9", jtracker::theme.cell_box_font);
        size.x *= Size;
        return {{ size.x, jtracker::data.track_line_height}, { size.x, jtracker::data.track_line_height}};
    }

};

// Labels expandable in horizontal axis
class expandable_label_base : public default_label
{

public:
    view_limits limits(const basic_context &ctx) const override
    {
        point size = measure_text(ctx.canvas, get_text() ,get_font());
        return {{size.x, size.y}, {full_extent, size.y}};
    }
};

using basic_expandable = basic_label_base<expandable_label_base>;
using expandable_label = label_gen<basic_expandable>;

using basic_expandable_with_font = label_with_font<expandable_label_base>;
using expandable_label_with_font = label_gen<basic_expandable_with_font>;

/*
// Idem with default font
class custom_font_label : public default_label
{
public:
    custom_font_label(font_descr font_d) : default_label(), descr(font_d), font(font_d)
    {
    }
      virtual font_type       get_font() const
    {
        return descr;
    }
      virtual float           get_font_size() const
    {
        return descr._size;
    }
      virtual float           get_default_font_size() const
    {
        return get_font_size();
    }
private:
    font_descr descr;
    font font;
};
*/

#endif // CUSTOM_LABELS_H
