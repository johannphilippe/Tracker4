#include "track_content.h"


void track_content::show_fully(bool b)
{
    fully_visible = b;
    display_visible();
}

void track_content::toggle_show()
{
    fully_visible = !fully_visible;
    display_visible();
}

void track_content::set_num_lines(size_t num_lines)
{
    if(num_lines > lines.size())
    {

        auto comp = [&](size_t index)
        {
        };

        lines.reserve(num_lines);
        reserve(num_lines);
        for(size_t i = size(); i < num_lines; i++)
        {
            track_event_type t = static_cast<track_event_type>(rand() % 6);
            std::shared_ptr<track_line> line_ptr(
                        new track_line( (fully_visible) ? num_cols : 4, i, callback, t));
            lines.push_back(line_ptr);
            lines.back()->on_click = [&, i](context const&ctx, mouse_button btn, size_t col_index)
            {
                click_select(ctx, btn, i, col_index);
            };
            auto label = fixed_size_label<4>(std::to_string(i));
            //push_back(share(htile(label, link(*lines.back().get()) )));
            push_back(share(htile(label, *lines.back() )));
        }
    } else if(num_lines < lines.size())
    {
        while(num_lines < lines.size())
        {
            lines.pop_back();
            pop_back();
        }
    }
}

void track_content::set_num_cols(size_t cols)
{
    if(cols == num_cols) return;
    num_cols = cols;
    display_visible();
}


void track_content::set_text_callback(std::function<void(size_t l_idx,
                                                         size_t c_idx, std::string_view text)> f)
{
    callback = f;
    for(auto & it : lines)
        it->callback = this->callback;
}


std::u32string_view track_content::get_at(size_t line, size_t col)
{
    if(line >= lines.size() || col >= num_cols)
        throw("Out of bounds -> line or col is above track size");
    return lines[line]->cells[col]->get_text();
}

std::string track_content::get_as_string_at(size_t line, size_t col)
{
    if(line >= lines.size() || col >= num_cols)
        throw("Out of bounds -> line or col is above track size");
    std::u32string_view v = lines[line]->cells[col]->get_text();
    return std::string(v.begin(), v.end());
}

void track_content::set_at(size_t line, size_t col, std::string_view text)
{
    if(line >= lines.size() || col >= num_cols)
        throw("Out of bounds -> line or col is above track size");
    lines[line]->cells[col]->set_text(text);
}

void track_content::set_at(size_t line, size_t col, std::string text)
{
    if(line >= lines.size() || col >= num_cols)
        throw("Out of bounds -> line or col is above track size");
    lines[line]->cells[col]->set_text(text);
}

void track_content::clear_cells()
{
    for(auto & it : lines)
        for(auto & it2 : it->cells)
        it2->set_text("");

}

/*

bool track_content::key(const context &ctx, key_info k)
{
}

*/
bool track_content::key(context const& ctx, key_info k)
{
    if(!selection.has_selection) return true;// vtile_composite::key(ctx, k);
    cell_selection old = selection.selected[selection.current_cell_index];
    cell_selection new_sel = old;

    view &v = jtracker::tracker_app::get_instance()->_view;

    auto do_focus = [&]()
    {
        selection.is_editing = false;
        end_focus();
        focus(selection.main_selected_line());
        lines[selection.main_selected_line()]->
                focus(selection.main_selected_column());
        lines[selection.main_selected_line()]->
                cells[selection.main_selected_column()]->focus(1);
        begin_focus();
        ctx.view.refresh();
    };

    auto update_selection = [&]()
    {
        if(old.line_index == new_sel.line_index && old.column_index == new_sel.column_index) return;
        for(auto & it : selection.selected)
        {
            get_cell_at(it.line_index, it.column_index)->background.unselect();
            animator.push_back(get_cell_at(it.line_index, it.column_index));
        }

        selection.clear();
        selection.select_main(new_sel);
        get_cell_at(selection.main_selected_line(), selection.main_selected_column())->background.select();
        animator.push_back(get_cell_at(selection.main_selected_line(), selection.main_selected_column()));
        animator.animate(v);

        do_focus();
    };

    auto prev_cell = [&]()
    {
      if(new_sel.column_index > 0)
      {
          new_sel.column_index--;
          update_selection();
      } else if(new_sel.line_index > 0)
      {
          new_sel.line_index--;
          new_sel.column_index = lines[new_sel.line_index]->size() - 1;
          update_selection();
      }
    };

    auto next_cell = [&]()
    {
        if(new_sel.column_index <
                lines[new_sel.line_index]->cells.size() - 1)
        {
            new_sel.column_index++;
            update_selection();
        } else if(new_sel.line_index < lines.size() - 1)
        {
            new_sel.line_index++;
            new_sel.column_index = 0;
            update_selection();
        }
    };

    if(selection.is_editing)
    {
        switch (k.key) {
        case key_code::left:
        case key_code::right:
        {
            std::cout << "forward left right " << std::endl;
            return lines[selection.main_selected_line()]->
                    cells[selection.main_selected_column()]->text_box.key(ctx, k);
        }
        default:
            break;
        }
    }

    switch(k.key)
    {
    case key_code::tab:
    {
        if(k.modifiers & mod_shift) {prev_cell();}
        else  {next_cell();}
        break;
    }
    case key_code::left:
    {
        prev_cell();
        break;
    }
    case key_code::right:
    {
        next_cell();
        break;
    }
    case key_code::up:
    {
        if(new_sel.line_index > 0) {
            new_sel.line_index--;
            update_selection();
        }
        break;
    }
    case key_code::down:
    {
        if(new_sel.line_index < (lines.size() - 1)) {
            new_sel.line_index++;
            update_selection();
        }
        break;
    }
    case key_code::escape:
    {
        unselect();
        break;
    }
    case key_code::backspace:
    {
        selection.is_editing = true;
        if(k.modifiers & mod_control)
        {
            lines[selection.main_selected_line()]->
                    cells[selection.main_selected_column()]->text_box.set_text("");
            ctx.view.refresh(lines[selection.main_selected_line()]->
                    cells[selection.main_selected_column()]->text_box);
            return true;
        }
        return lines[selection.main_selected_line()]->
                cells[selection.main_selected_column()]->text_box.key(ctx,k);
        break;
    }
    default:
    {
        selection.is_editing = true;
        update_selection();
        break;
    }
    }
    return true;
}

void track_content::click_select(context const&, mouse_button btn, size_t line, size_t col)
{
    if(btn.down) {
        view &v = jtracker::tracker_app::get_instance()->_view;

        auto simple_select = [&]()
        {
            for(auto & it : selection.selected)
            {
                get_cell_at(it.line_index, it.column_index)->background.unselect();
                animator.push_back(get_cell_at(it.line_index, it.column_index));
            }
            selection.selected.clear();
            selection.select_main(line, col);
            get_cell_at(line, col)->background.select();
            animator.push_back(get_cell_at(line, col));
            animator.animate(v);
        };

        // find path of cells to the current one
        auto shift_selection = [&]()
        {
            cell_selection current = selection.main_selected_cell();
            for(auto & it : selection.selected)
            {
                get_cell_at(it.line_index, it.column_index)->background.unselect();
                animator.push_back(get_cell_at(it.line_index, it.column_index));
            }

            selection.clear();
            std::pair<size_t, size_t> line_p =
                    std::make_pair(std::min(current.line_index, line),
                                   std::max(current.line_index, line));
            std::pair<size_t, size_t> col_p =
                    std::make_pair(std::min(current.column_index, col),
                                   std::max(current.column_index, col));
            for(size_t l = line_p.first; l <= line_p.second; l++)
            {
                for(size_t c = col_p.first; c <= col_p.second; c++)
                {
                    selection.selected.push_back(cell_selection(l,c));
                }
            }
            selection.select_main(line, col);
            for(auto & it : selection.selected)
            {
                get_cell_at(it.line_index, it.column_index)->background.select();
                animator.push_back(get_cell_at(it.line_index, it.column_index));
            }
            animator.animate(v);
        };

        if(btn.modifiers == 0)
        {
            simple_select();
        } else if(btn.modifiers & mod_control)
        {
            // find if is already selected
            int sel = selection.is_selected(line, col);
            cell_selection c_sel = selection.selected[sel];
            if(sel != -1)
            {
                //lines[selection.selected[sel].line_index]->
                //        cells[selection.selected[sel].column_index]->background.unselect();
                get_cell_at(c_sel.line_index, c_sel.column_index)->background.unselect();
                animator.push_back(get_cell_at(c_sel.line_index, c_sel.column_index));
                animator.animate(v);
                selection.remove_at(sel);
            }
            else
            {
            // Main selected should be the last one
                selection.select_main(line, col);
                get_cell_at(selection.selected.back().line_index, selection.selected.back().column_index)->background.select();
                animator.push_back(get_cell_at(selection.selected.back().line_index, selection.selected.back().column_index));
                animator.animate(v);
            }

        } else if(btn.modifiers & mod_shift)
        {
            if(!selection.has_selection)
                return simple_select();
            shift_selection();
        }
    }
}

void track_content::unselect()
{
   view &v = jtracker::tracker_app::get_instance()->_view;
   if(selection.is_editing)
   {
       selection.is_editing = false;
       //lines[selection.main_selected_line()]->
       //        cells[selection.main_selected_column()]->text_box.end_focus();
       get_main_cell()->text_box.end_focus();
       v.refresh(*get_main_cell());
       return;
   }
   for(auto & it : selection.selected) {
       get_cell_at(it.line_index, it.column_index)->background.unselect();
       animator.push_back(get_cell_at(it.line_index, it.column_index));
   }

   animator.animate(v);
   selection.clear();
}

std::shared_ptr<track_cell>& track_content::get_cell_at(size_t line, size_t col)
{
    return lines[line]->cells[col];
}

std::shared_ptr<track_cell>& track_content::get_main_cell()
{
    return lines[selection.main_selected_line()]->cells[selection.main_selected_column()];
}

void track_content::display_visible()
{
    if(fully_visible)
    {
        for(auto & it : lines)
            it->set_num_cols(num_cols);
    } else
    {
        for(auto & it : lines)
            it->set_num_cols(4);
    }
}
