#ifndef DYNAMIC_SIZE_CELL_COMPOSER_H
#define DYNAMIC_SIZE_CELL_COMPOSER_H

#include<elements.hpp>
using namespace cycfi::elements;

struct resize_condition
{
    bool update_width = true;
    bool update_height = true;
};

class dynamic_vertical_cell_composer : public cell_composer
{

public:

    using cell_ptr = element_ptr;
    using composer_function = std::function<element_ptr(size_t)>;

    dynamic_vertical_cell_composer(size_t length, composer_function c, resize_condition *cond) :
        cell_composer(),
        _size(length),
        _composer(c),
        resize_conditions(cond)
    {
    }

    std::size_t size() const override
    {
        return _size;
    }

    void resize(size_t size)
    {
        _size = size;
    }

    element_ptr compose(std::size_t index) override
    {
        return _composer(index);
    }

    limits secondary_axis_limits(const basic_context &ctx) const override
    {
        if( resize_conditions->update_width || _secondary_axis_size.min == -1)
            update_width(ctx);
        return _secondary_axis_size;
    }

    float main_axis_size(std::size_t index, const basic_context &ctx) const override
    {
        if(  resize_conditions->update_height || _main_axis_size == -1)
            update_height(ctx);
        return _main_axis_size;
    }


    void update_height(basic_context const& ctx) const
    {
        auto lim = const_cast<dynamic_vertical_cell_composer*>(this)->compose(0)->limits(ctx);
        _main_axis_size = lim.min.y;
        resize_conditions->update_height = false;
    }

    void update_width(basic_context const& ctx) const
    {
        auto lim = const_cast<dynamic_vertical_cell_composer*>(this)->compose(0)->limits(ctx);
        _secondary_axis_size.min = lim.min.x;
        _secondary_axis_size.max = lim.max.x;
        resize_conditions->update_width = false;

    }

    size_t _size;
    composer_function _composer;

    mutable limits _secondary_axis_size = {-1, full_extent};
    mutable float _main_axis_size = -1;
    resize_condition *resize_conditions;
};

inline auto make_resizable_composer( size_t length, std::function<element_ptr(size_t)> f, resize_condition *cond)
{
    return share(dynamic_vertical_cell_composer(length, f, cond));
}

#endif // DYNAMIC_SIZE_CELL_COMPOSER_H
