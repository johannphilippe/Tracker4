#ifndef TRACK_CONTENT_H
#define TRACK_CONTENT_H

#include<elements.hpp>
#include<functional>
#include<vector>
#include<iostream>
#include<memory>
#include<widgets/custom_labels.h>

using namespace cycfi::elements;

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
                view_limits l;
                l.max.x = (i == 1) ? 40 : 60;
                l.min.x = l.max.x;
                auto box = input_box();
                cells.push_back(box.second);
                cells.back()->on_text = [this, i](std::string_view text)
                {
                    callback(line_index, i, text);
                };

                this->push_back(share(limit(l, box.first )));
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


    std::vector<std::shared_ptr<basic_input_box>> cells;
    size_t line_index;
    std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> callback;
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


protected:

    void display_visible();
};


#endif // TRACK_CONTENT_H
