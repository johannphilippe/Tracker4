#include "widgets/track_line.h"
#include<widgets/custom_labels.h>

void track_line::set_num_cols(size_t num_cols)
{
    if(num_cols == cells.size()) return;
    bool is_grid_focus = jtracker::data.grid_step > 0 && (line_index % jtracker::data.grid_step) == 0;
    if(num_cols > cells.size())
    {
        for(size_t i = cells.size(); i < num_cols; i++)
        {
            cells.push_back(
                // The 40 width is for P2 in Csound. It should be smaller (30, or 25) but needs to wait for the size bug to be fixed in elements.
                std::make_shared<track_cell>((i==1) ? 40 : 60, get_event_color(event_type))
                );
            if(is_grid_focus)
                cells.back()->background.is_in_grid = true;
            cells.back()->on_click = [&, i](context const& ctx, mouse_button btn)
            {
                if(click_cbk != nullptr)
                    (*click_cbk)(ctx, btn, line_index, i);
            };

            cells.back()->text_box.on_text = [&, i](std::string_view v)
            {
                if(text_cbk != nullptr)
                    (*text_cbk)(line_index, i, v);
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
    bool is_grid_focus = jtracker::data.grid_step > 0 && (line_index % jtracker::data.grid_step) == 0;
    color c = is_grid_focus ?
            jtracker::theme.track_label_index_hot_color :
            jtracker::theme.track_label_index_color;
    auto lab = make_fixed_size_label_with_background<4>(std::to_string(line_index), c );

    for(size_t i = 0; i < cells.size(); ++i)
    {
        cells[i]->background.is_in_grid = is_grid_focus;
    }

    if(create)
        push_back(lab);
    else
        data()[0] = lab;

}
