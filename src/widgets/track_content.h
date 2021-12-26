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
#include<animations/cell_animator.h>
#include<tracker/jtracker.h>
#include<controllers/cell_selection_controller.h>
#include<widgets/dynamic_size_cell_composer.h>

using namespace cycfi::elements;
using namespace std::chrono_literals;
using namespace jtracker;

/*
 * Represents a line of cells.
*/
class track_line : public htile_composite
{
public:
    track_line(size_t num, size_t l_idx,
               track_event_type ev_type = track_event_type::none) :
        htile_composite(),
        line_index(l_idx),
        event_type(ev_type)
    {
        push_back(share(fixed_size_label<4>(std::to_string(l_idx))));
        set_num_cols(num);
    }

    track_line(size_t num, size_t l_idx) :
        htile_composite(), line_index(l_idx)
    {
        set_num_cols(num);
    }


    void set_num_cols(size_t num_cols)
    {
        if(num_cols == cells.size()) return;
        if(num_cols > cells.size())
        {
            for(size_t i = cells.size(); i < num_cols; i++)
            {
                cells.push_back(
                            // The 40 is for P2 in Csound. It should be smaller (30, or 25) but needs to wait for the size bug to be fixed in elements.
                            std::make_shared<track_cell>((i==1) ? 40 : 60, get_event_color(event_type))
                            );
                cells.back()->on_click = [&, i](context const& ctx, mouse_button btn)
                {
                    if(click_cbk != nullptr)
                        (*click_cbk)(ctx, btn, line_index, i);
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
        return htile_composite::limits(ctx);
    }

    std::function<void(context const&ctx, mouse_button btn, size_t, size_t col_index)> *click_cbk = nullptr;
    std::function<void(key_info k)> on_key = [](key_info){};

    std::vector<std::shared_ptr<track_cell>> cells;
    size_t line_index = 0;

    int _to_focus;

    track_event_type event_type;

    std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> text_callback =
            [](size_t, size_t, std::string_view) {};

};


/*
 * Represents a spreadsheet of dynamic cells
*/
class track_content : public vdynamic_list
{
public:

    // line maker
    std::shared_ptr<track_line> make_line(size_t i)
    {
        if(i >= _lines.size())
        {
            std::cout << "Overflow in lines " << std::endl;
            throw("Overflow in lines");
        }
        if(_lines[i] == nullptr)
        {
            _lines[i] = std::make_shared<track_line>( fully_visible ? num_cols : 4, i, static_cast<track_event_type>(rand() % 6));
            _lines[i]->click_cbk = &cell_click_callback;

        }
        return _lines[i];
    }

    // track content constructor
    track_content(size_t num_lines = 64, size_t num_cols = 4) :
        vdynamic_list(
            make_resizable_composer(num_lines, [this](size_t i) { return make_line(i);}, &resize_conditions)
            ),
        num_cols(num_cols),
        _lines(num_lines, nullptr)
    {
    }

    void set_num_lines(size_t num_lines);
    void set_num_cols(size_t cols);

    // display only 4 cells or full track
    void toggle_show();
    void show_fully(bool b);

    // getters setters for text
    std::u32string_view get_at(size_t line, size_t col);
    std::string get_as_string_at(size_t line, size_t col);
    void set_at(size_t line, size_t col, std::string_view text);
    void set_at(size_t line, size_t col, std::string text);
    // clear all cells in track
    void clear_cells();

    //controls
    bool key(context const&, key_info) override;
    void click_select(context const&ctx, mouse_button btn, size_t line, size_t col);
    void unselect();

    // cell handle
    std::shared_ptr<track_cell>& get_cell_at(size_t line, size_t col);
    std::shared_ptr<track_cell>& get_main_cell();

    bool fully_visible = false;

    cell_selector selection;
    //cell_animator animator;

    size_t num_cols;

    std::vector<std::shared_ptr<track_line>> _lines;

    std::function<void(context const& ctx, mouse_button btn, size_t l, size_t c)> cell_click_callback =
            [&](context const& ctx, mouse_button btn, size_t l, size_t c) {this->click_select(ctx, btn, l, c);};
    std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> text_callback =
            [](size_t, size_t, std::string_view){};


    resize_condition resize_conditions;
protected:

    void display_visible();
};


#endif // TRACK_CONTENT_H
