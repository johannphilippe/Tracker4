#ifndef ACTIVE_CELL_COMPOSER_H
#define ACTIVE_CELL_COMPOSER_H

#include<elements.hpp>
#include<vector>
using namespace cycfi::elements;

class active_cell_composer : public cell_composer
{

public:

    using cell_ptr = element_ptr;
    using composer_function = std::function<element_ptr(size_t)>;

    active_cell_composer(size_t length, composer_function c) : cell_composer(),
        _size(length), _composer(c)
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

    limits width_limits(const basic_context &ctx) const override
    {
        auto lim = const_cast<active_cell_composer*>(this)->compose(0)->limits(ctx);
        return {lim.min.x, lim.max.x};
    }

    float line_height(std::size_t index, const basic_context &ctx) const override
    {
        auto lim = const_cast<active_cell_composer*>(this)->compose(0)->limits(ctx);
        return lim.max.y;
    }


    size_t _size;

    composer_function _composer;
};


#endif // ACTIVE_CELL_COMPOSER_H
