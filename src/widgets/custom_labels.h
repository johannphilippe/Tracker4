#ifndef CUSTOM_LABELS_H
#define CUSTOM_LABELS_H

#include<elements.hpp>
using namespace cycfi::elements;
using namespace cycfi::artist;

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


#endif // CUSTOM_LABELS_H
