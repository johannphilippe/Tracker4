#ifndef CELL_ANIMATOR_H
#define CELL_ANIMATOR_H

#include<elements.hpp>
#include<vector>
#include<memory>
#include<iostream>
#include<widgets/track_cell.h>
#include<utilities/paint_utilities.h>
using namespace cycfi::elements;
using namespace std::chrono_literals;
using namespace jtracker::color_utilities;

class color_animator :
        public std::vector< std::shared_ptr<interpolated_color> >
{
public:
    color_animator() {}

    void animate(view &v)
    {
        if(_is_animating || (size() == 0) ) return;
        _is_animating = true;
        _animate(v);
    }

private:
    void _animate(view & v)
    {
        if(size() == 0) {
            _is_animating = false;
            return;
        }
        for(size_t i = 0; i < size();)
        {
            if(data()[i]->interpolate())
                erase(begin() + i);
            else
                ++i;
        }
        v.refresh();
        v.post(fps, [&](){_animate(v);});
    }

    bool _is_animating = false;

    std::chrono::milliseconds fps = 30ms;
};

/*
class cell_animator :
        public std::vector<std::shared_ptr<track_cell>>
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
*/
#endif // CELL_ANIMATOR_H
