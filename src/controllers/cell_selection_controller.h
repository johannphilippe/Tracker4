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

    void select(size_t line, size_t column);
    void set_selection(size_t line, size_t col);

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

    void select_main(cell_selection c);
    void select_main(size_t line, size_t col);

    void remove_at(size_t index);
    void clear();

    cell_selection main_selected_cell();
    size_t main_selected_line();
    size_t main_selected_column();

    int is_selected(size_t line, size_t column);

    std::vector<cell_selection> selected;
    size_t current_cell_index;
    bool has_selection, is_editing;
};


#endif // CELL_SELECTION_CONTROLLER_H
