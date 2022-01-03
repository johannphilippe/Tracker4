#include "track_content.h"

template<typename T>
std::shared_ptr<T> track_content<T>::make_line(size_t i)
{
    if(i >= _lines.size())
    {
        std::cout << "Overflow in lines " << std::endl;
        throw("Overflow in lines");
    }
    if(_lines[i] == nullptr)
    {
        _lines[i] = std::make_shared<T>( fully_visible ? num_cols : 4, i, static_cast<track_event_type>(rand() % 6));
        _lines[i]->click_cbk = &cell_click_callback;
    }
    return _lines[i];
}

    // track content constructor
template<typename T>
track_content<T>::track_content(size_t num_lines, size_t num_cols)
    : vdynamic_list(
        make_resizable_composer(num_lines, [this](size_t i) { return make_line(i);}, &resize_conditions))
    , num_cols(num_cols)
    , _lines(num_lines, nullptr)
    , cell_click_callback([this](context const& ctx, mouse_button btn, size_t l, size_t c) {this->click_select(ctx, btn, l, c);})
    , text_callback([](size_t, size_t, std::string_view){})
{}


template<typename T>
void track_content<T>::display_visible()
{
    for(auto & it : _lines)
    {
        if(it == nullptr) continue;
        it->set_num_cols((fully_visible) ? num_cols : 4);
    }
    resize_conditions.update_width = true;
    update();
}

template<typename T>
void track_content<T>::update_labels()
{
    for(auto & it : _lines)
        if(it.get() != nullptr)
            it->update_label();
}

template<typename T>
void track_content<T>::update_lines()
{
    resize(jtracker::data.number_of_lines);
    _lines.resize(jtracker::data.number_of_lines);
}

template<typename T>
void track_content<T>::show_fully(bool b)
{
    fully_visible = b;
    display_visible();
}

template<typename T>
void track_content<T>::toggle_show()
{
    fully_visible = !fully_visible;
    display_visible();
}

template<typename T>
void track_content<T>::set_num_cols(size_t cols)
{
    if(cols == num_cols) return;
    num_cols = cols;

    if(!fully_visible) return;

    for(auto & it : _lines)
    {
        if(it == nullptr) break;
        it->set_num_cols(cols);
    }
    resize_conditions.update_width = true;
}

template<typename T>
std::u32string_view track_content<T>::get_at(size_t line, size_t col)
{
    if(line >= _lines.size() || col >= num_cols)
        throw("Out of bounds -> line or col is above track size");
    return get_cell_at(line, col)->get_text();
}

template<typename T>
std::string track_content<T>::get_as_string_at(size_t line, size_t col)
{
    if(line >= _lines.size() || col >= num_cols)
        throw("Out of bounds -> line or col is above track size");
    std::u32string_view v = get_cell_at(line, col)->get_text();
    return std::string(v.begin(), v.end());
}

template<typename T>
void track_content<T>::set_at(size_t line, size_t col, std::string_view text)
{
    if(line >= _lines.size() || col >= num_cols)
        throw("Out of bounds -> line or col is above track size");
    get_cell_at(line, col)->set_text(text);
}

template<typename T>
void track_content<T>::set_at(size_t line, size_t col, std::string text)
{
    if(line >= _lines.size() || col >= num_cols)
        throw("Out of bounds -> line or col is above track size");
    get_cell_at(line, col)->set_text(text);
}

template<typename T>
void track_content<T>::clear_cells()
{
    for(auto & it : _lines)
        for(auto & it2 : it->cells)
            it2->set_text("");

}

template<typename T>
bool track_content<T>::key(context const& ctx, key_info k)
{
    if(!selection.has_selection) return true;
    cell_selection old = selection.selected[selection.current_cell_index];
    cell_selection new_sel = old;

    view &v = jtracker::tracker_app::get_instance()->_view;

    auto do_focus = [&]()
    {
        selection.is_editing = false;
        end_focus();
        focus(selection.main_selected_line());
        // +1 to avoid the label at the beginning of the line
        _lines[selection.main_selected_line()]->focus(selection.main_selected_column() + 1);
        get_main_cell()->focus(1);
        begin_focus();
        ctx.view.refresh();
    };

    auto update_selection = [&]()
    {
        if(old.line_index == new_sel.line_index && old.column_index == new_sel.column_index) return;
        for(auto & it : selection.selected)
        {
            if(it.column_index < _lines[it.line_index]->cells.size()) {
                get_cell_at(it.line_index, it.column_index)->background.unselect();
                tracker_app::get_animator_pool().push_back(get_cell_at(it.line_index, it.column_index)->background.current_color);
            }
        }

        selection.clear();
        selection.select_main(new_sel);
        get_cell_at(selection.main_selected_line(), selection.main_selected_column())->background.select();
        tracker_app::get_animator_pool().push_back(
                    get_cell_at(selection.main_selected_line(), selection.main_selected_column())->background.current_color
                    );
        tracker_app::get_animator_pool().animate(v);

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
          // -2 here because of the static label at the begining of the line
          new_sel.column_index = _lines[new_sel.line_index]->size() - 2;
          update_selection();
      }
    };

    auto next_cell = [&]()
    {
        if(new_sel.column_index <
                _lines[new_sel.line_index]->cells.size() - 1)
        {
            new_sel.column_index++;
            update_selection();
        } else if(new_sel.line_index < _lines.size() - 1)
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
            return get_main_cell()->text_box.key(ctx, k);
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
        if(new_sel.line_index < (_lines.size() - 1)) {
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
            get_main_cell()->text_box.set_text("");
            ctx.view.refresh(get_main_cell()->text_box);
            return true;
        }
        return get_main_cell()->text_box.key(ctx, k);
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

template<typename T>
void track_content<T>::click_select(context const&, mouse_button btn, size_t line, size_t col)
{
    if(btn.down) {
        view &v = jtracker::tracker_app::get_instance()->_view;

        auto simple_select = [&]()
        {
            for(auto & it : selection.selected)
            {
                if(it.column_index < _lines[it.line_index]->cells.size()) {
                    get_cell_at(it.line_index, it.column_index)->background.unselect();
                    tracker_app::get_animator_pool().push_back(get_cell_at(it.line_index, it.column_index)->background.current_color);
                }
            }
            selection.selected.clear();
            selection.select_main(line, col);
            get_cell_at(line, col)->background.select();
            tracker_app::get_animator_pool().push_back(get_cell_at(line, col)->background.current_color);
            tracker_app::get_animator_pool().animate(v);
        };

        // find path of cells to the current one
        auto shift_selection = [&]()
        {
            cell_selection current = selection.main_selected_cell();
            for(auto & it : selection.selected)
            {
                get_cell_at(it.line_index, it.column_index)->background.unselect();
                tracker_app::get_animator_pool().push_back(get_cell_at(it.line_index, it.column_index)->background.current_color);
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
                tracker_app::get_animator_pool().push_back(get_cell_at(it.line_index, it.column_index)->background.current_color);
            }
            tracker_app::get_animator_pool().animate(v);
        };

        auto alt_selection = [&]()
        {
          for(size_t c = 0; c < _lines[line]->cells.size(); c++ )
          {
             int sel = selection.is_selected(line, c);
             if(sel == -1)
             {
                 selection.selected.push_back(cell_selection(line, c));
                 get_cell_at(line, c)->background.select();
                 tracker_app::get_animator_pool().push_back(get_cell_at(line, c)->background.current_color);
             } else if(sel != - 1)  // != - 1
             {
                 get_cell_at(line, c)->background.unselect();
                 tracker_app::get_animator_pool().push_back(get_cell_at(line, c)->background.current_color);
                 selection.remove_at(sel);
             }
          }

          tracker_app::get_animator_pool().animate(v);

          selection.select_main(line, col);
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
                tracker_app::get_animator_pool().push_back(get_cell_at(c_sel.line_index, c_sel.column_index)->background.current_color);
                tracker_app::get_animator_pool().animate(v);
                selection.remove_at(sel);
            }
            else
            {
            // Main selected should be the last one
                selection.select_main(line, col);
                get_cell_at(selection.selected.back().line_index, selection.selected.back().column_index)->background.select();
                tracker_app::get_animator_pool().push_back(
                            get_cell_at(selection.selected.back().line_index, selection.selected.back().column_index)->background.current_color
                            );
                tracker_app::get_animator_pool().animate(v);
            }

        } else if(btn.modifiers & mod_shift)
        {
            if(!selection.has_selection)
                return simple_select();
            shift_selection();
        } else if(btn.modifiers & mod_alt)
        {
            alt_selection();
        }
    }
}

template<typename T>
void track_content<T>::unselect()
{
   view &v = jtracker::tracker_app::get_instance()->_view;
   if(selection.is_editing)
   {
       selection.is_editing = false;
       get_main_cell()->text_box.end_focus();
       v.refresh(*get_main_cell());
       return;
   }
   for(auto & it : selection.selected) {
       get_cell_at(it.line_index, it.column_index)->background.unselect();
       tracker_app::get_animator_pool().push_back(get_cell_at(it.line_index, it.column_index)->background.current_color);
   }

   tracker_app::get_animator_pool().animate(v);
   selection.clear();
}

template<typename T>
std::shared_ptr<track_cell>& track_content<T>::get_cell_at(size_t line, size_t col)
{
    return _lines[line]->cells[col];
}

template<typename T>
std::shared_ptr<track_cell>& track_content<T>::get_main_cell()
{
    return _lines[selection.main_selected_line()]->
            cells[selection.main_selected_column()];
}


template class track_content<track_line>;
template class track_content<tempo_track_line>;
