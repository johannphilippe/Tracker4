#ifndef TRACK_CONTENT_H
#define TRACK_CONTENT_H

#include<elements.hpp>
#include<functional>
#include<vector>
#include<iostream>
#include<memory>
#include<widgets/custom_labels.h>
#include<widgets/track_cell.h>

using namespace cycfi::elements;

struct track_selection
{
    track_selection() : has_selected(false),
        line_index(0), column_index(0)
    {}

    track_selection(bool has_select, size_t line_idx, size_t col_idx) :
        has_selected(has_select), line_index(line_idx), column_index(col_idx)
    {}

    void select(size_t line, size_t column)
    {
        has_selected = true;
        line_index = line;
        column_index = column;
    }

    void unselect() {has_selected = false;}

    void set_selection(size_t line, size_t col, bool focus)
    {
        has_selected = focus;
        line_index = line;
        column_index = col;
    }

    bool has_selected;
    size_t line_index, column_index;
};

class track_line : public htile_composite
{
public:
    track_line(size_t num, size_t l_idx,
               std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> f) :
        htile_composite(),
        line_index(l_idx), callback(f)
    {
        set_num_cols(num);
    }

    void set_num_cols(size_t num_cols)
    {
        if(num_cols == cells.size()) return;
        if(num_cols > cells.size())
        {
            for(size_t i = size(); i < num_cols; i++)
            {
                cells.push_back(std::make_shared<track_cell>((i==1) ? 40: 60));
                cells.back()->on_click = [&](auto, auto)
                {
                  return true;
                };
                cells.back()->on_focus = [&,i](bool focus)
                {
                    this->on_focus(line_index, i, focus);
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

    std::function<void(size_t line, size_t col, bool)> on_focus = [](size_t, size_t, bool){};
    std::function<void(key_info k)> on_key = [](key_info){};

    std::vector<std::shared_ptr<track_cell>> cells;
    size_t line_index;
    std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> callback;
    int _to_focus;
};

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

    std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> callback;
    std::vector<std::shared_ptr<track_line>> lines;
    size_t num_cols;
    bool fully_visible;

    bool key(context const&, key_info) override;

    void select(size_t line, size_t col);
    void unselect();
    track_selection selection;

protected:

    void display_visible();
};


#endif // TRACK_CONTENT_H
