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
#include<tracker/jtracker.h>
#include<controllers/cell_selection_controller.h>
#include<widgets/dynamic_size_cell_composer.h>
#include<tracker/jtracker_globals.h>

#include<widgets/track_line.h>
#include<widgets/tracker/tempo_track_line.h>
using namespace cycfi::elements;
using namespace std::chrono_literals;
using namespace jtracker;

/*
 * Represents a spreadsheet of dynamic cells
*/
template<typename T = track_line>
class track_content : public vdynamic_list
{
public:

    // line maker
    std::shared_ptr<T> make_line(size_t i);

    // track content constructor
    track_content(size_t num_lines = jtracker::data.number_of_lines, size_t num_cols = jtracker::data.default_column_number);


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

    // cell handle - simple utility to avoid verbosity
    std::shared_ptr<track_cell>& get_cell_at(size_t line, size_t col);
    std::shared_ptr<track_cell>& get_main_cell();

    bool fully_visible = false;

    cell_selector selection;
    //cell_animator animator;

    void set_num_cols(size_t cols);
    size_t num_cols;

    void update_lines();
    void update_labels();

    std::vector<std::shared_ptr<T>> _lines;

    using cell_click_cbk_type = std::function<void(context const&, mouse_button, size_t, size_t)>;
    using text_callback_type = std::function<void(size_t, size_t, std::string_view)>;
    using new_focus_callback = std::function<void(size_t, size_t, std::u32string_view)>;

    // Callback redirecting to click_select
    cell_click_cbk_type cell_click_callback;

    // Callback to parent track_set
    text_callback_type text_callback;
    // new cell callback for parent track_set
    new_focus_callback cell_focus_callback;

    resize_condition resize_conditions;
protected:

    void display_visible();
};


#endif // TRACK_CONTENT_H
