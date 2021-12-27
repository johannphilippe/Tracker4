#ifndef TRACK_H
#define TRACK_H

#include<elements.hpp>
#include<widgets/custom_labels.h>
#include<tracker/jtracker.h>
#include<iostream>

#include<widgets/track_content.h>
#include<widgets/track_base.h>
#include<widgets/spinbox.h>
using namespace cycfi::elements;

namespace jtracker {

class tracker_track :
        public track_base<track_type::tracker_track>,
        public array_composite<2, vtile_element>
{
public:
    inline auto make_buttons_tile(size_t track_index)
    {
        auto clear_but = icon_button(icons::cancel, 1, theme.clear_buttons_color);
        clear_but.on_click = [&](bool) {t_content.clear_cells();};
        auto resize_but = toggle_icon_button(icons::resize_horizontal, 1);

        resize_but.on_click = [&](bool) {
            t_content.end_focus();
            t_content.toggle_show();
            this->end_focus();
            focus(1);
            begin_focus();
            tracker_app::get_instance()->_view.layout();
            tracker_app::get_instance()->refresh();
        };

        auto col_number_spin = fixed_width_spinbox<size_t, 70>(spin_controller<size_t>(4, std::numeric_limits<size_t>::max(), 8, 1));
        col_number_spin.on_change = [&](size_t n)
        {
          t_content.set_num_cols(n);
        };

        return vtile(htile(
                    fixed_size_label<8>(std::string("track " + std::to_string(track_index))),
                    resize_but,
                    clear_but,
                    col_number_spin
                    ), vspacer(5));
    }

    tracker_track(size_t track_index = 0) :
    array_composite<2, vtile_element>(vtile(make_buttons_tile(track_index), link(t_content) ) )

    {
        t_content.update_lines();
    }

    bool click(context const& ctx, mouse_button btn)
    {
        context rctx {ctx, ctx.bounds};
        rctx.bounds.top += this->at(0).limits(ctx).max.y;
        if(rctx.bounds.includes(btn.pos))
        {
            return this->at(1).click(rctx, btn);
        }

        return array_composite<2, vtile_element>::click(ctx, btn);
    }

    void end_focus() override
    {
        t_content.unselect();
        array_composite<2, vtile_element>::end_focus();
    }

    track_content t_content;
};
}

#endif // TRACK_H
