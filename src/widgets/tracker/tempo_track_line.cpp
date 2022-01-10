#include<widgets/tracker/tempo_track_line.h>
#include<widgets/track_cell.h>

tempo_track_line::tempo_track_line(size_t , size_t l_idx, track_event_type t)
    : track_line(l_idx, t)
{
    update_label(true);
    bool is_grid_focus = jtracker::data.grid_step > 0 && (line_index % jtracker::data.grid_step) == 0;
    for(size_t i = 0; i < 2; i++)
    {
        cells.push_back(std::make_shared<track_cell>(60, get_event_color(track_event_type::none)));
        if(is_grid_focus) cells.back()->background.is_in_grid = is_grid_focus;
        //std::cout << "create tempo cell and click cbk is null : at : " << l_idx <<" " << i << " " << (click_cbk == nullptr) << std::endl;
        cells.back()->on_click = [&, i](context const& ctx, mouse_button btn)
        {
           // std::cout << "still clicking though callback is null perheaps " << std::endl;
            if(click_cbk != nullptr)
            {
             //   std::cout << " tempo cell callback in line " << i << std::endl;
                (*click_cbk)(ctx, btn, line_index, i);
            }
        };
        this->push_back(cells.back());
    }
}

void tempo_track_line::set_num_cols(size_t)
{
    return;
}
