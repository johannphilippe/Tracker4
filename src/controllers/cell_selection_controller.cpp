#include<controllers/cell_selection_controller.h>

void cell_selection::select(size_t line, size_t column)
{
    line_index = line;
    column_index = column;
}

void cell_selection::set_selection(size_t line, size_t col)
{
    line_index = line;
    column_index = col;
}

void cell_selector::for_each(std::function<void(cell_selection &)> cbk)
{
    for(auto & it : selected)
        cbk(it);
}

void cell_selector::select_main(cell_selection c)
{
    has_selection = true;
    int s = is_selected(c.line_index, c.column_index);
    if(s != -1) {
        current_cell_index = s;
        return;
    }
    selected.push_back(c);
    current_cell_index = selected.size() - 1;
}

void cell_selector::select_main(size_t line, size_t col)
{
    select_main(cell_selection(line, col));
}

void cell_selector::remove_at(size_t index)
{

    if(index > selected.size())
        throw("Out of bounds remove in cell selector");
    if(index == current_cell_index)
    {
        has_selection = is_editing = false;
    }
    selected.erase(selected.begin() + index);
}

void cell_selector::clear()
{
    selected.clear();
    has_selection = is_editing = false;
}

cell_selection cell_selector::main_selected_cell() {return selected[current_cell_index];}
size_t cell_selector::main_selected_line() {return selected[current_cell_index].line_index;}
size_t cell_selector::main_selected_column() {return selected[current_cell_index].column_index;};

int cell_selector::is_selected(size_t line, size_t column)
{
    for(size_t i = 0; i < selected.size(); ++i)
        if(selected[i].line_index == line && selected[i].column_index == column)
            return i;
    return -1;
}
