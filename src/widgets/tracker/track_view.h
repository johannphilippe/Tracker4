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


class track_view_bar : public array_composite<2, layer_element>
{
public:
    track_view_bar() :
        array_composite<2, layer_element>(pane("Track controls", htile( link(seq_nbr), link(track_nbr), link(line_nbr) ))) {}
    track_bar_spin track_nbr, seq_nbr, line_nbr;
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
    main_cell_editor_layout() : array_composite<2, layer_element>(layer( link(editor), box(get_theme().frame_color)))
    {}

    main_cell_editor editor;
};

class track_set : public htile_composite
{
public:
    track_set() : htile_composite()
    {}

    inline auto make_separator()
    {
        return hspacer(15);
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

    void set_num_lines(size_t lines)
    {
        for(auto & it : tracks)
        {
            if(std::holds_alternative<tracker_track_ptr>(it))
            {
                tracker_track_ptr t_ptr(std::get<tracker_track_ptr>(it));
                if(t_ptr->t_content.lines.size() != lines)
                    t_ptr->t_content.set_num_lines(lines);
            }
        }
    }

    void add_track(track_type t = track_type::tracker_track)
    {
        switch (t) {
        case track_type::tracker_track:
        {
            tracker_track_ptr t_ptr(new tracker_track(8, 8));
            std::variant<tracker_track_ptr, sniper_track_ptr > v;
            v = t_ptr;
            tracks.push_back(v);
            push_back(share(htile(link(*t_ptr), make_separator())));

        break;
        }
        case track_type::sniper_track:
        {
            sniper_track_ptr t_ptr(new sniper_track());
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
        return {full_extent, full_extent};
    }


    view_limits limits(basic_context const& ctx) const override
    {
        //return {{full_extent, full_extent}, {full_extent, full_extent}};
        return htile_composite::limits(ctx);
    }

    std::vector<std::variant<tracker_track_ptr, sniper_track_ptr>> tracks;
};

class track_view : public array_composite<3, vtile_element>
{
public:
    track_view() :
        array_composite<3, vtile_element>(vtile( link(bar), link(text_box), scroller(  margin({5, 20, 5, 30}, link(t_set)) )))
    {
        bar.track_nbr.on_change = [&](size_t v)
        {
            t_set.set_num_tracks(v);
            t_set.set_num_lines(bar.line_nbr.get_value());
            jtracker::tracker_app::get_instance()->_view.refresh(*this);
        };
        bar.track_nbr.set_value(4);
        t_set.set_num_tracks(bar.track_nbr.get_value());

        bar.line_nbr.set_value(16);
        t_set.set_num_lines(bar.line_nbr.get_value());
        bar.line_nbr.on_change = [&](size_t l)
        {

          t_set.set_num_lines(l);
          view &v = jtracker::tracker_app::get_instance()->_view;
          jtracker::tracker_app::get_instance()->_view.post([&](){
              v.layout();
              v.refresh();
          });
        };
    }

    view_limits limits(basic_context const& ctx) const override
    {
        view_limits l;
        l.max.x = ctx.view_bounds().width();
        l.max.y = ctx.view_bounds().height();
        return l;
    }


    track_view_bar bar;
    main_cell_editor_layout text_box;
    track_set t_set;
};

}
#endif // TRACK_VIEW_H
