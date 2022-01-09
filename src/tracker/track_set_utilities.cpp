#include "track_set_utilities.h"
#include<tracker/jtracker.h>
namespace jtracker {

void track_set_utilities::set_text_to_selected_cells( std::vector<track_t> &tracks, std::string_view v)
{
    std::cout << "set to selected cells" << std::endl;
    for(auto & it : tracks)
    {
        if(std::holds_alternative<tracker_track_ptr>(it))
        {
            std::cout << "tracker track ->";
            tracker_track_ptr t = std::get<tracker_track_ptr>(it);
            t->t_content.selection.for_each([&](cell_selection &sel)
            {
                std::cout << "has selection " ;
               std::shared_ptr<track_cell> c_ptr = t->t_content.get_cell_at(sel.line_index, sel.column_index);
               if(c_ptr.get() == nullptr) return;
               c_ptr->set_text(v);
            });
            std::cout << std::endl;
        }
    }
    jtracker::get_app()->_view.refresh();
}

}
