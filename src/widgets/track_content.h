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
#include<widgets/active_dynamic_list.h>

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
    track_line(size_t num, size_t l_idx, std::function<void(context const&, mouse_button, size_t)> on_click_,
               track_event_type ev_type = track_event_type::none) :
    htile_composite(),
    line_index(l_idx),
    event_type(ev_type),
      on_click(on_click_)
    {
        //this->push_back(share(fixed_size_label<4>(std::to_string(l_idx))));
        set_num_cols(num);
    }

    void set_num_cols(size_t num_cols)
    {
        if(num_cols == cells.size()) return;
        if(num_cols > cells.size())
        {
            reserve(num_cols);
            cells.reserve(num_cols);
            for(size_t i = cells.size(); i < num_cols; i++)
            {
                cells.push_back(std::make_shared<track_cell>((i==1) ? 40: 60, get_event_color(event_type)));
                cells.back()->on_click = [this, i](context const& ctx, mouse_button btn)
                {
                    this->on_click(ctx, btn, i);
                    return true;
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

    view_limits limits(basic_context const& ctx) const  override
    {
        if(size() != 0)
            return htile_composite::limits(ctx);
        return  {{0, 0}, {0,0}};
    }

    std::function<void(context const&ctx, mouse_button btn, size_t col_index)> on_click;
    std::function<void(key_info k)> on_key = [](key_info){};

    std::vector<std::shared_ptr<track_cell>> cells;
    size_t line_index;

    int _to_focus;

    track_event_type event_type;

    std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> callback =
            [](size_t, size_t, std::string_view) {};
};


/*
 * Represents a spreadsheet of cells with selection states.
*/
class track_content : public active_dynamic_list<track_line>
{
public:

    // line maker
    inline std::shared_ptr<track_line> make_line(size_t i, size_t num_lines, size_t num_cols)
    {
        auto click_cbk = [&, i, num_lines, num_cols](context const& ctx, mouse_button btn, size_t col)
        {
            std::cout << "click callback !!! " << i << " "  << col << std::endl;
          click_select(ctx, btn, i, col);
        };
        std::shared_ptr<track_line> line_ptr = std::make_shared<track_line>(num_cols, i, click_cbk, static_cast<track_event_type>(rand() % 6) );
        return line_ptr;
    }

    // track content constructor
    track_content(size_t num_lines = 64, size_t num_cols = 4) :
        active_dynamic_list(
            std::make_shared<active_cell_composer<track_line>>(num_lines,
                                                               [&, num_lines, num_cols](size_t i) -> std::shared_ptr<track_line> {return make_line(i, num_lines, num_cols);}
            )), num_cols(num_cols)
    {
    }

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
    bool click(context const& ctx, mouse_button btn) override;
    void click_select(context const&ctx, mouse_button btn, size_t line, size_t col);
    void unselect();

    std::shared_ptr<track_cell>& get_cell_at(size_t line, size_t col);
    std::shared_ptr<track_cell>& get_main_cell();

    std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> callback =
            [](size_t, size_t, std::string_view){};

    bool fully_visible = false;

    cell_selector selection;
    cell_animator animator;

    size_t num_cols;
protected:

    void display_visible();
};


#endif // TRACK_CONTENT_H
