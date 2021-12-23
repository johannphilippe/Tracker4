#ifndef TRACK_H
#define TRACK_H

#include<elements.hpp>
#include<widgets/custom_labels.h>
#include<tracker/jtracker.h>
#include<iostream>

#include<widgets/track_content.h>
#include<widgets/track_base.h>
using namespace cycfi::elements;

namespace jtracker {

class tracker_track :
        public track_base<track_type::tracker_track>,
        public array_composite<2, vtile_element>
{
public:
    inline auto make_buttons_tile()
    {
        auto clear_but = icon_button(icons::cancel, 1);
        clear_but.on_click = [&](bool) {t_content.clear_cells();};
        auto resize_but = icon_button(icons::resize_horizontal, 1);
        resize_but.on_click = [&](bool) {
            t_content.toggle_show();
            jtracker::tracker_app::get_instance()->_view.layout();
            jtracker::tracker_app::get_instance()->_view.refresh();
        };
        return htile( fixed_size_label<4>("tk 1"),resize_but, clear_but );
    }

    tracker_track(size_t num_line = 8, size_t num_cols = 4,
          std::function<void(size_t, size_t, std::string_view)>
          f = [](size_t, size_t, std::string_view){}) :
    array_composite<2, vtile_element>(vtile(make_buttons_tile(), link(t_content) ))
    {
        t_content.set_num_cols(num_cols);
        t_content.set_num_lines(num_line);
    }

    track_content t_content;
};
}

#endif // TRACK_H
