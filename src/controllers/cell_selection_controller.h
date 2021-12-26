#ifndef CELL_SELECTION_CONTROLLER_H
#define CELL_SELECTION_CONTROLLER_H

#include<elements.hpp>
using namespace cycfi::elements;

/*
 * Represents a simple cell selection
*/
struct cell_selection
{
    cell_selection() :
        line_index(0), column_index(0)
    {}

    cell_selection(size_t line_idx, size_t col_idx) :
        line_index(line_idx), column_index(col_idx)
    {}

    void select(size_t line, size_t column)
    {
        line_index = line;
        column_index = column;
    }

    void set_selection(size_t line, size_t col)
    {
        line_index = line;
        column_index = col;
    }

    size_t line_index, column_index;
};

/*
 * Represents a list of cell selection states
 * editing and selected state
*/
struct cell_selector
{
    cell_selector() :
        has_selection(false), is_editing(false)
    {}

    void select_main(cell_selection c)
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

    void select_main(size_t line, size_t col)
    {
        select_main(cell_selection(line, col));
    }

    void remove_at(size_t index)
    {
        if(index > selected.size())
            throw("Out of bounds remove in cell selector");
        if(index == current_cell_index)
        {
            has_selection = is_editing = false;
        }
        selected.erase(selected.begin() + index);
    }

    void clear()
    {
        selected.clear();
        has_selection = is_editing = false;
    }

    cell_selection main_selected_cell() {return selected[current_cell_index];}
    size_t main_selected_line() {return selected[current_cell_index].line_index;}
    size_t main_selected_column() {return selected[current_cell_index].column_index;};

    int is_selected(size_t line, size_t column) {
        for(size_t i = 0; i < selected.size(); ++i)
            if(selected[i].line_index == line && selected[i].column_index == column)
                return i;
        return -1;
    }

    std::vector<cell_selection> selected;
    size_t current_cell_index;
    bool has_selection, is_editing;
};


#endif // CELL_SELECTION_CONTROLLER_H
