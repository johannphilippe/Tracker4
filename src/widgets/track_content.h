#ifndef TRACK_CONTENT_H
#define TRACK_CONTENT_H

#include<elements.hpp>
#include<functional>
#include<vector>
#include<iostream>
#include<memory>
#include<widgets/custom_labels.h>
#include<widgets/track_cell.h>
#include<models/track_event.h>

using namespace cycfi::elements;
using namespace std::chrono_literals;


struct cell_animator : public std::vector<std::shared_ptr<track_cell>>
{
    constexpr static auto fps = 33ms;
    cell_animator() : is_animating(false)
    {}

    void animate(view &v)
    {
        if(is_animating) return;
        _animate(v);
    }

    bool is_animating;

private:
    void _animate(view &v)
    {
        if(size() == 0) return;

        for(size_t i = 0; i < size(); )
        {
           if(data()[i]->background.current_color.interpolate())
               erase(begin() + i);
           else
               ++i;
        }

        v.refresh(  );
        v.post(fps, [&](){_animate(v);});
    }
};

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


/*
 * Represents a line of cells.
*/
class track_line : public htile_composite
{
public:
    track_line(size_t num, size_t l_idx, std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> f,
               track_event_type ev_type = track_event_type::none) :
    htile_composite(),
    line_index(l_idx), callback(f),
    event_type(ev_type)
    {
        set_num_cols(num);
    }

    void set_num_cols(size_t num_cols)
    {
        if(num_cols == cells.size()) return;
        if(num_cols > cells.size())
        {
            reserve(num_cols);
            cells.reserve(num_cols);
            for(size_t i = size(); i < num_cols; i++)
            {
                cells.push_back(std::make_shared<track_cell>((i==1) ? 40: 60, get_event_color(event_type)));
                cells.back()->on_click = [&, i](context const& ctx, mouse_button btn)
                {
                    on_click(ctx, btn, i);
                    return true;
                };
                //cells.back()->text_box.set_text(std::to_string(static_cast<int>(event_type) ) );
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

    std::function<void(context const&ctx, mouse_button btn, size_t col_index)> on_click;
    std::function<void(key_info k)> on_key = [](key_info){};

    std::vector<std::shared_ptr<track_cell>> cells;
    size_t line_index;
    std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> callback;
    int _to_focus;

    track_event_type event_type;
};

/*
 * Represents a spreadsheet of cells with selection states.
*/
class track_content : public vtile_composite
{
public:
    track_content(size_t num_lines = 8, size_t num_cols = 4) :
        vtile_composite(), callback([&](size_t, size_t, std::string_view){}),
        num_cols(num_cols), fully_visible(false)
    {
        set_num_lines(num_lines);
    }
    track_content(size_t num_lines, size_t num_cols,
                 std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> f) :
        vtile_composite(), callback(f), num_cols(num_cols), fully_visible(false)
    {
        set_num_lines(num_lines);
    }

    void set_text_callback(std::function<void(size_t l_idx, size_t c_idx, std::string_view text)> f);

    void set_num_lines(size_t num_lines);
    void set_num_cols(size_t cols);
    void toggle_show();
    void show_fully(bool b);

    std::u32string_view get_at(size_t line, size_t col);
    std::string get_as_string_at(size_t line, size_t col);
    void set_at(size_t line, size_t col, std::string_view text);
    void set_at(size_t line, size_t col, std::string text);
    void clear_cells();

    bool key(context const&, key_info) override;
    void click_select(context const&ctx, mouse_button btn, size_t line, size_t col);
    void unselect();

    std::shared_ptr<track_cell>& get_cell_at(size_t line, size_t col);
    std::shared_ptr<track_cell>& get_main_cell();


    std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> callback;
    std::vector<std::shared_ptr<track_line>> lines;
    size_t num_cols;
    bool fully_visible;
    cell_selector selection;

    cell_animator animator;

protected:

    void display_visible();
};


#endif // TRACK_CONTENT_H
