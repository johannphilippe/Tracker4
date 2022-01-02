#ifndef TRACK_VIEW_H
#define TRACK_VIEW_H

#include<elements.hpp>
#include<vector>
#include<memory>
#include<variant>
#include<widgets/tracker/tracker_track.h>
#include<widgets/tracker/sniper_track.h>
#include<widgets/track_base.h>
#include<widgets/spinbox.h>
#include<widgets/tracker/tempo_track.h>
#include<widgets/expandable.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

namespace jtracker {

using tracker_track_ptr = std::shared_ptr<tracker_track>;
using sniper_track_ptr = std::shared_ptr<sniper_track>;

class track_bar_spin : public spinbox<size_t>
{
public:
    track_bar_spin() : spinbox<size_t>(spin_controller<size_t>(1, std::numeric_limits<size_t>::max(), 1, 1))
    {}
};

class grid_step_spin : public spinbox<int>
{
public:
    grid_step_spin() : spinbox<int>(spin_controller<int>(0, std::numeric_limits<int>::max(), 0, 1))
    {}
};


class track_view_bar : public array_composite<2, layer_element>
{
public:
    track_view_bar();

    track_bar_spin track_nbr, seq_nbr, line_nbr;
    grid_step_spin grid_step;
};

class main_cell_editor : public basic_input_box
{
public:
    main_cell_editor() : basic_input_box("General editor")
    {}
};

class main_cell_editor_layout : public array_composite<2, layer_element>
{
public:
    main_cell_editor_layout();
    view_limits limits(basic_context const& ctx) const override;
    main_cell_editor editor;
};

//////////////////////////////////////////////////////////////////////////
// Track Set
// Horizontal list of Tracks
/////////////////////////////////////////////////////////////////////////
class track_set : public htile_composite
{
public:
    track_set();

    void set_num_tracks(size_t number);
    void update_lines();
    void update_labels(bool both = true);
    void add_track(track_type t = track_type::tracker_track);
    void pop_track();
    void remove_track(size_t index);

    view_stretch stretch() const override;
    view_limits limits(basic_context const& ctx) const override;

    std::vector<std::variant<tracker_track_ptr, sniper_track_ptr>> tracks;
};

//////////////////////////////////////////////////////////////////////////
// Track View
// Vertical list containing track set, tempo_track and common controls
/////////////////////////////////////////////////////////////////////////
class track_view : public array_composite<3, vtile_element>
{
public:
    track_view();

    view_limits limits(basic_context const& ctx) const override;

    track_view_bar bar;
    main_cell_editor_layout text_box;
    track_set t_set;
    tempo_track_expander t_tempo;
};

}
#endif // TRACK_VIEW_H
