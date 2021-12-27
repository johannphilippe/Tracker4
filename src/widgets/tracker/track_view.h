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
    track_view_bar() :
        array_composite<2, layer_element>(pane("Track controls",
                                               htile(
                                                   simple_heading(link(seq_nbr), "Sequence" ),
                                                   hspacer(5),
                                                   simple_heading(link(track_nbr), "Tracks"),
                                                   hspacer(5),
                                                   simple_heading(link(line_nbr),"Lines"),
                                                   hspacer(5),
                                                   simple_heading(link(grid_step),"Grid")
                                                   )
                                               )
                                          )
    {}

    track_bar_spin track_nbr, seq_nbr, line_nbr;
    grid_step_spin grid_step;
};

class main_cell_editor : public basic_input_box
{
public:
    main_cell_editor() : basic_input_box()
    {
    }
};

class main_cell_editor_layout : public array_composite<2, layer_element>
{
public:

    main_cell_editor_layout() :
        array_composite<2, layer_element>(layer(  margin({5,5,5, 5}, align_center_middle( link(editor) )),
                                                  rbox(colors::dark_orchid.opacity(0.4), 5)))
    {}

    main_cell_editor editor;

    view_limits limits(basic_context const& ctx) const override
    {
        view_limits l = editor.limits(ctx);
        l.max.y *= 1.5;
        return l;
    }
};

class track_set : public htile_composite
{
public:
    track_set() : htile_composite()
    {}

    inline auto make_separator()
    {
        return hspacer(7);
    }

    void set_num_tracks(size_t number)
    {
        if(number > tracks.size())
        {
            for(size_t i = tracks.size(); i < number; i++)
            {
                add_track();
            }
        } else if(number < tracks.size())
        {
            while(number < tracks.size())
            {
                pop_track();
            }
        }
    }

    void update_lines()
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

    void update_labels(bool both = true)
    {
        for(auto & it : tracks)
        {
            if( both && std::holds_alternative<tracker_track_ptr>(it))
                std::get<tracker_track_ptr>(it)->t_content.update_labels();
            else if(std::holds_alternative<sniper_track_ptr>(it))
                std::get<sniper_track_ptr>(it)->sn_box.update_labels();
        }

    }

    void add_track(track_type t = track_type::tracker_track)
    {
        switch (t) {
        case track_type::tracker_track:
        {
            tracker_track_ptr t_ptr(new tracker_track(tracks.size()));
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

    void pop_track()
    {
        if(size() > 0)
        {
            pop_back();
            tracks.pop_back();
        }
    }

    void remove_track(size_t index)
    {
        if(index >= tracks.size()) return;
        erase(begin() + index, begin() + index + 1);
        tracks.erase(tracks.begin() + index, tracks.begin() + index + 1);
    }

    view_stretch stretch() const override
    {
        return {2, 2};
    }


    view_limits limits(basic_context const& ctx) const override
    {
        view_limits l  = htile_composite::limits(ctx);
        l.max = {full_extent, full_extent};
        return l;
    }

    std::vector<std::variant<tracker_track_ptr, sniper_track_ptr>> tracks;
};

class track_view : public array_composite<3, vtile_element>
{
public:
    track_view() :
        array_composite<3, vtile_element>(vtile(
                                              link(bar),
                                              vtile(vspacer(8), link(text_box), vspacer(8)),
                                              hold(
                                                  share(
                                                      scroller(
                                                          margin({5, 20, 5, 30},
                                                                 link(
                                                                     t_set
                                                                     )
                                                                 )
                                                          )
                                                      )
                                                  )
                                              )
                                          )
    {
        bar.track_nbr.on_change = [&](size_t v)
        {
            t_set.set_num_tracks(v);
            t_set.update_lines();
            jtracker::tracker_app::get_instance()->_view.refresh(*this);
        };
        bar.track_nbr.set_value(2);
        t_set.set_num_tracks(bar.track_nbr.get_value());

        bar.line_nbr.set_value(16);

        // Design note : should probably set always the global data, then refresh what needs to be
        // better than calling setters all the time
        bar.line_nbr.on_change = [&](size_t l)
        {
          jtracker::data.number_of_lines = l;
          t_set.update_lines();
          t_set.update_labels(false);
          view &v = jtracker::tracker_app::get_instance()->_view;
          v.layout();
          v.refresh();
        };

        bar.grid_step.on_change = [&](size_t change)
        {
          jtracker::data.grid_step = change;
          t_set.update_labels();
          jtracker::tracker_app::get_instance()->_view.refresh(*this);
        };
    }

    view_limits limits(basic_context const& ctx) const override
    {
        view_limits normal = array_composite<3, vtile_element>::limits(ctx);
        point view_size = ctx.view.size();
        normal.max = view_size;
        return normal;
    }


    track_view_bar bar;
    main_cell_editor_layout text_box;
    track_set t_set;
};

}
#endif // TRACK_VIEW_H
