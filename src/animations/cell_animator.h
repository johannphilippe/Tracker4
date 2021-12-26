#ifndef CELL_ANIMATOR_H
#define CELL_ANIMATOR_H

#include<elements.hpp>
#include<vector>
#include<memory>
#include<iostream>
#include<widgets/track_cell.h>

using namespace cycfi::elements;
using namespace std::chrono_literals;

class cell_animator : public std::vector<std::shared_ptr<track_cell>>
{
public:
    cell_animator()
    {}

    void animate(view &v)
    {
        if(is_animating) return;
        _animate(v);
    }

    bool is_animating = false;

private:
    void _animate(view &v)
    {
        if(size() == 0) return;

        for(size_t i = 0; i < size(); )
        {
           if(data()[i]->background.current_color.interpolate())
               erase(begin() + i);
           else
               ++i;
        }

        v.refresh(  );
        v.post(fps, [&](){_animate(v);});
    }

    std::chrono::milliseconds fps = 30ms;
};
#endif // CELL_ANIMATOR_H
