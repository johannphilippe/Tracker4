#ifndef TRACK_VIEW_H
#define TRACK_VIEW_H

#include<elements.hpp>
#include<vector>
#include<memory>
#include<variant>

#include<widgets/spinbox.h>
#include<widgets/tracker/tempo_track.h>
#include<tracker/track_base.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

namespace jtracker {

////////////////////////////////////////////////////////////////////////////////
// Custom templated spinbox
////////////////////////////////////////////////////////////////////////////////
template<typename T, T Init>
class track_bar_spin : public spinbox<T>
{
public:
    track_bar_spin() : spinbox<T>(spin_controller<T>(Init, std::numeric_limits<T>::max(), Init, 1))
    {}
};

////////////////////////////////////////////////////////////////////////////////
// Track view top spinboxes
////////////////////////////////////////////////////////////////////////////////
class track_view_bar : public array_composite<2, layer_element>
{
public:
    track_view_bar();

    track_bar_spin<size_t, 1> track_nbr, seq_nbr, line_nbr;
    track_bar_spin<int, 0> grid_step;
};

////////////////////////////////////////////////////////////////////////////////
// Global text editor
////////////////////////////////////////////////////////////////////////////////
class main_cell_editor : public basic_input_box
{
public:
    main_cell_editor() : basic_input_box("General editor", font_descr({"Lato", 16.0}).medium())
    {}
};

class main_cell_editor_layout :
        public array_composite<2, layer_element>
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

    std::function<void(size_t t, size_t l, size_t c, std::string_view v)> text_cbk =
            [](size_t, size_t ,size_t, std::string_view) {};
    std::function<void(size_t t, size_t l, size_t c, std::u32string_view v)> new_cell_focus_callback =
            [](size_t, size_t, size_t, std::u32string_view){};

};

//////////////////////////////////////////////////////////////////////////
// Track Scrollers
/////////////////////////////////////////////////////////////////////////
class track_scrollers
{
public:
    track_scrollers();

    using link_set = indirect<reference<typename std::remove_reference<track_set>::type>>;
    using link_tempo = indirect<reference<typename std::remove_reference<tempo_track>::type>>;
    using right_margin_t = margin_element<right_margin_rect, cycfi::remove_cvref_t<link_tempo>>;
    using set_scroller_t = proxy<cycfi::remove_cvref_t<link_set>, scroller_base>;
    using tempo_scroller_t = proxy<cycfi::remove_cvref_t<right_margin_t>, scroller_base>;

    track_set t_set;
    tempo_track t_tempo;
    set_scroller_t t_set_scroller;
    std::shared_ptr<tempo_scroller_t> t_tempo_scroller;
    tempo_track_expander t_tempo_expander;
};

//////////////////////////////////////////////////////////////////////////
// Track View
// Vertical list containing track set, tempo_track and common controls
/////////////////////////////////////////////////////////////////////////
class track_view : public track_scrollers, public array_composite<5, vtile_element>
{
public:
    track_view();

    view_limits limits(basic_context const& ctx) const override;

    track_view_bar bar;
    main_cell_editor_layout text_box;
};

}
#endif // TRACK_VIEW_H
