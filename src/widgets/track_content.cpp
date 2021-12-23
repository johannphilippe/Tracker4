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
        for(size_t i = size(); i < num_lines; i++)
        {
            std::shared_ptr<track_line> line_ptr(new track_line( (fully_visible) ? num_cols : 4, i, callback));
            lines.push_back(line_ptr);
            lines.back()->on_focus = [&](size_t line, size_t col, bool focus)
            {
                select(line, col);
            };
            auto label = fixed_size_label<4>(std::to_string(i));
            push_back(share(htile(label, link(*lines.back().get()) )));
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


void track_content::set_text_callback(std::function<void(size_t l_idx, size_t c_idx, std::string_view text)> f)
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
    std::cout << "key callback " << std::endl;
    if(!selection.has_selected) return true;// vtile_composite::key(ctx, k);
    track_selection old = selection;
    switch(k.key)
    {
    case key_code::left:
    {
        if(selection.column_index > 0)
            selection.column_index--;
        break;
    }
    case key_code::right:
    {
        if(selection.column_index < (lines[selection.line_index]->cells.size() - 1) )
            selection.column_index++;
        break;
    }
    case key_code::up:
    {
        if(selection.line_index > 0)
            selection.line_index--;
        break;
    }
    case key_code::down:
    {
        if(selection.line_index < (lines.size() - 1))
            selection.line_index++;
        break;
    }
    case key_code::escape:
    {
        unselect();
        end_focus();
        break;
    }
    default:
    {
        std::cout << "vtile composite key " << std::endl;
        //return vtile_composite::key(ctx, k);
        return true;
        break;
    }
    }

    std::cout << "updating selection  " << std::endl;
    if(old.line_index != selection.line_index || old.column_index != selection.column_index) {
        view &v = jtracker::tracker_app::get_instance()->_view;
        lines[old.line_index]->cells[old.column_index]->background.unselect(v);
        lines[selection.line_index]->cells[selection.column_index]->background.select(v);

        end_focus();
        focus(selection.line_index);
        lines[selection.line_index]->focus(selection.column_index);
        lines[selection.line_index]->cells[selection.column_index]->focus(1);
        begin_focus();
        ctx.view.refresh();
    }
    return true;
}

void track_content::select(size_t line, size_t col)
{
    std::cout << "select click " << std::endl;
    view &v = jtracker::tracker_app::get_instance()->_view;
    if(selection.has_selected)
        lines[selection.line_index]->cells[selection.column_index]->background.unselect(v);
    selection.select(line, col);
    lines[line]->cells[col]->background.select(v);
}

void track_content::unselect()
{
   selection.unselect();
   view &v = jtracker::tracker_app::get_instance()->_view;
   lines[selection.line_index]->cells[selection.column_index]->background.unselect(v);
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
