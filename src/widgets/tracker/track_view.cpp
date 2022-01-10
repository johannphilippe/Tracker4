#include<widgets/tracker/track_view.h>
#include<widgets/expandable.h>
#include<tracker/track_set_utilities.h>
#include<tracker/track_base.h>
#include<widgets/tracker/tracker_track.h>
#include<widgets/tracker/sniper_track.h>

//////////////////////////////////////////////////////////////////////////
// Materials
/////////////////////////////////////////////////////////////////////////
track_view_bar::track_view_bar() :
    array_composite<2, layer_element>(pane("Track controls",
                                           htile(
                                               simple_heading(link(seq_nbr), "Sequence" ),
                                               hspacer(5),
                                               simple_heading(link(track_nbr), "Tracks"),
                                               hspacer(5),
                                               simple_heading(link(line_nbr),"Lines"),
                                               hspacer(5),
                                               simple_heading(link(grid_step),"Grid")
                                               ))) {}


main_cell_editor_layout::main_cell_editor_layout() :
    array_composite<2, layer_element>(layer( margin( {5, 5, 5, 5}, (  vsize(100, scroller( hsize(16384, link(editor) ), no_scrollbars | no_vscroll)))),
                                              rbox(colors::dark_orchid.opacity(0.4), 5))) {}

view_limits main_cell_editor_layout::limits(basic_context const& ctx) const
{
    view_limits l = editor.limits(ctx);
    l.max.y *= 1.5;
    return l;
}

//////////////////////////////////////////////////////////////////////////
// Track Set
//////////////////////////////////////////////////////////////////////////

track_set::track_set() : htile_composite()
{}

inline auto make_separator()
{
    return hspacer(7);
}

void track_set::set_num_tracks(size_t number)
{
    if(number > tracks.size())
    {
        for(size_t i = tracks.size(); i < number; i++)
            add_track();
    } else if(number < tracks.size())
    {
        while(number < tracks.size())
            pop_track();
    }
}

void track_set::update_lines()
{
    for(auto & it : tracks)
    {
        if(std::holds_alternative<tracker_track_ptr>(it))
        {
            tracker_track_ptr t_ptr(std::get<tracker_track_ptr>(it));
            t_ptr->t_content.update_lines();
        }
        else if(std::holds_alternative<sniper_track_ptr>(it))
        {
            sniper_track_ptr t_ptr(std::get<sniper_track_ptr>(it));
            t_ptr->sn_box.update_lines();
        }
    }
}

void track_set::update_labels(bool both)
{
    for(auto & it : tracks)
    {
        if( both && std::holds_alternative<tracker_track_ptr>(it))
        std::get<tracker_track_ptr>(it)->t_content.update_labels();
        else if(std::holds_alternative<sniper_track_ptr>(it))
        std::get<sniper_track_ptr>(it)->sn_box.update_labels();
    }
}

void track_set::add_track(track_type t)
{
    switch (t) {
    case track_type::tracker_track:
    {
        tracker_track_ptr t_ptr(new tracker_track(tracks.size()));
        size_t t_idx = tracks.size();
        t_ptr->t_content.text_callback = [&, t_idx](size_t l, size_t c, std::string_view v)
        {
            std::cout << "cooolback here in track " << t_idx << " " << l << " " << c << " " << v << std::endl;
            text_cbk(t_idx, l, c, v);
        };
        t_ptr->t_content.cell_focus_callback = [&, t_idx](size_t l, size_t c, std::u32string_view v)
        {
           new_cell_focus_callback(t_idx, l, c, v);
        };
        std::variant<tracker_track_ptr, sniper_track_ptr > v;
        v = t_ptr;
        tracks.push_back(v);
        push_back(share(htile(
                layer(
                    link(*t_ptr),
                    rbox(jtracker::theme.track_background_color)
                    ),
                make_separator())));

    break;
    }
    case track_type::sniper_track:
    {
        sniper_track_ptr t_ptr(new sniper_track(tracks.size()));
        std::variant<tracker_track_ptr, sniper_track_ptr> v;
        v = t_ptr;
        tracks.push_back(v);
        push_back(share(htile(link(*t_ptr), make_separator())));
        break;
    }
    }
}

void track_set::pop_track()
{
    if(size() > 0)
    {
        pop_back();
        tracks.pop_back();
    }
}

void track_set::remove_track(size_t index)
{
    if(index >= tracks.size()) return;
    erase(begin() + index, begin() + index + 1);
    tracks.erase(tracks.begin() + index, tracks.begin() + index + 1);
}

view_stretch track_set::stretch() const
{return {2, 2};}


view_limits track_set::limits(basic_context const& ctx) const
{
    view_limits l  = htile_composite::limits(ctx);
    l.max = {full_extent, full_extent};
    return l;
}


track_scrollers::track_scrollers()
    : t_set(), t_tempo()
    , t_set_scroller(
          scroller(
              link(t_set)))
    , t_tempo_scroller(std::make_shared<tempo_scroller_t>(
                           vscroller(
                               right_margin(0, link( t_tempo )), no_scrollbars)))
    , t_tempo_expander(t_tempo_scroller)
{
    t_set_scroller.on_scroll = [&](point p )
    {
        // let it scroll horizontally without affecting the other one
        if(p.x != 0) return;
        t_tempo_scroller->set_position(p);
        jtracker::get_app()->_view.refresh(*t_tempo_scroller);
    };

    t_tempo_scroller->on_scroll = [&](point p)
    {
      t_set_scroller.set_position(p);
      jtracker::get_app()->_view.refresh(t_set_scroller);
    };
}

//////////////////////////////////////////////////////////////////////////
// Track View
/////////////////////////////////////////////////////////////////////////

track_view::track_view()
    : track_scrollers()
    , array_composite<5, vtile_element>(vtile(
                                          link(bar),
                                            vspacer(5),
                                            margin({0, 5, 0, 5},
                                                    link(text_box)),
                                                   //link(edit)),
                                            vspacer(5),
                                            hold(
                                                  share(
                                                      htile(
                                                          margin({5, 5, 5, 0},
                                                             link(t_set_scroller)
                                                                 ), hspacer(10),
                                                      margin({5,5, 5, 0},
                                                             link(t_tempo_expander) )
                                                        )
                                                    )
                                                )
                                            )
                                        )
{

    t_set.text_cbk = [&](size_t t, size_t l, size_t c, std::string_view v)
    {
        text_box.editor.set_text(v);
        jtracker::get_app()->_view.refresh(text_box.editor);
    };

    t_set.new_cell_focus_callback = [&](size_t t, size_t l, size_t c, std::u32string_view v)
    {
        text_box.editor.set_text(v);
        jtracker::get_app()->_view.refresh(text_box.editor);
    };

    text_box.editor.on_text = [&](std::string_view v)
    {
        track_set_utilities::set_text_to_selected_cells(t_set.tracks, v);
    };

    // on track_nbr change
    bar.track_nbr.on_change = [&](size_t v)
    {
        // Here it should update the global number of tracks, follow it to persistent data, then update tracks/lines
        // to implement
        t_set.set_num_tracks(v);
        t_set.update_lines();
        jtracker::tracker_app::get_instance()->_view.refresh(*this);
    };

    // should be done at init from "tracknbr" global  (remove this painful code)
    bar.track_nbr.set_value(2);
    t_set.set_num_tracks(bar.track_nbr.get_value());

    // idem
    bar.line_nbr.set_value(16);

    // on line number change
    bar.line_nbr.on_change = [&](size_t l)
    {
      jtracker::data.number_of_lines = l;
      t_set.update_lines();
      t_set.update_labels(false);
      t_tempo.t_content.update_lines();
      t_tempo.t_content.update_labels();
      view &v = jtracker::get_app()->_view;
      v.layout();
      v.refresh();
    };

    // on grid nbr change
    bar.grid_step.on_change = [&](size_t change)
    {
      jtracker::data.grid_step = change;
      t_set.update_labels();
      t_tempo.t_content.update_labels();
      jtracker::tracker_app::get_instance()->_view.refresh(*this);
    };

}

view_limits track_view::limits(basic_context const& ctx) const
{
    view_limits normal = array_composite<5, vtile_element>::limits(ctx);
    point view_size = ctx.view.size();
    normal.max = view_size;
    return normal;
}


