#include "widgets/track_line.h"

void track_line::set_num_cols(size_t num_cols)
{
    if(num_cols == cells.size()) return;
    if(num_cols > cells.size())
    {
        for(size_t i = cells.size(); i < num_cols; i++)
        {
            cells.push_back(
                // The 40 width is for P2 in Csound. It should be smaller (30, or 25) but needs to wait for the size bug to be fixed in elements.
                std::make_shared<track_cell>((i==1) ? 40 : 60, get_event_color(event_type))
                );
            cells.back()->on_click = [&, i](context const& ctx, mouse_button btn)
            {
                if(click_cbk != nullptr)
                    (*click_cbk)(ctx, btn, line_index, i);
            };
            this->push_back(cells.back());
        }
    }
    else if(num_cols < cells.size() && num_cols >= 4)
    {
        while(num_cols < cells.size())
        {
            cells.pop_back();
            pop_back();
        }
    }
}

void track_line::update_label(bool create)
{
    color c = (jtracker::data.grid_step > 0 && ((line_index % jtracker::data.grid_step) == 0) ) ?
            jtracker::theme.track_label_index_hot_color :
            jtracker::theme.track_label_index_color;
    auto lab = make_fixed_size_label_with_background<4>(std::to_string(line_index), c );
    if(create)
        push_back(lab);
    else
        data()[0] = lab;
}
