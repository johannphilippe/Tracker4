#ifndef SNIPER_TRACK_H
#define SNIPER_TRACK_H

#include<elements.hpp>
#include<widgets/sniper_track_content.h>
#include<widgets/sniper_track_box.h>
#include<widgets/custom_labels.h>
using namespace cycfi::elements;

class sniper_track :
        public vtile_composite
{
public:

    inline auto make_buttons_tile(size_t track_index)
    {
        auto clear_but = icon_button(icons::cancel, 1, jtracker::theme.clear_buttons_color);
        clear_but.on_click = [&](bool) {sn_box.sn_content.clear();};

        auto display_editor = icon_button(icons::resize_small, 1, jtracker::theme.spin_background_color);
        display_editor.on_click = [&](bool )
        {
          sn_box.show_editor(false);
        };


        return vtile(htile(
                    fixed_size_label<8>(std::string("track " + std::to_string(track_index))),
                    clear_but, display_editor
                    ), vspacer(5));
    }

    sniper_track(size_t track_index) : vtile_composite()
    {
        push_back(share(make_buttons_tile(track_index)));
        push_back(share(link(sn_box) ));
    }

    sniper_track_box sn_box;
    //sniper_track_content sn_content;
};

#endif // SNIPER_TRACK_H
