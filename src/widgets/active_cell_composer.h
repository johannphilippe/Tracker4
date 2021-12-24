#ifndef ACTIVE_CELL_COMPOSER_H
#define ACTIVE_CELL_COMPOSER_H

#include<elements.hpp>
#include<vector>
using namespace cycfi::elements;

template<typename T>
class active_cell_composer : public cell_composer
{

public:

    using cell_ptr = std::shared_ptr<T>;
    using composer_function = std::function<std::shared_ptr<T>(size_t)>;

    active_cell_composer(size_t length, composer_function c) : cell_composer(),
        _size(length), _composer(c)
    {
        _rows.resize(_size, nullptr);
    }

    std::size_t size() const override
    {
        return _size;
    }

    void resize(size_t size)
    {
        _rows.resize(_size);
        _size = size;
    }

    element_ptr compose(std::size_t index) override
    {
        if(_rows[index] == nullptr)
        {
            _rows[index] = _composer(index);
        }
        return _rows[index];
    }

    limits width_limits(const basic_context &ctx) const override
    {
        auto lim =  _rows[0]->limits(ctx);
        return {lim.min.x, lim.max.x};
    }

    float line_height(std::size_t index, const basic_context &ctx) const override
    {
        auto lim = const_cast<active_cell_composer*>(this)->compose(0)->limits(ctx);
        return lim.max.y;
    }

    size_t _size;
    std::vector< cell_ptr > _rows;

    composer_function _composer;
};


#endif // ACTIVE_CELL_COMPOSER_H
