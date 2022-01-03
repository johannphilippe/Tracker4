#ifndef TRACKER_MENUS_H
#define TRACKER_MENUS_H

#include<elements.hpp>
#include<dialogs/options_dialog.h>
#include<tracker/tracker_actions.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

namespace jtracker {

///////////////////////////////////////////////////////////////
// Files Menu
///////////////////////////////////////////////////////////////
inline auto make_file_popup()
{
    auto popup  = button_menu("Files", menu_position::bottom_right);

    auto sk_new = shortcut_key{key_code::n, mod_action};
    auto sk_open = shortcut_key{key_code::o, mod_action};
    auto sk_save = shortcut_key{key_code::s, mod_action};

    auto new_file = menu_item("New project", sk_new);
    auto open_file = menu_item("Open project", sk_open);
    auto save_file = menu_item("Save file", sk_save);

    open_file.on_click = [&]()
    {
        open_project_action();
    };

    auto menu =
       layer(
          vtile(
                    new_file,
                    open_file,
                    save_file
          ),
          panel{}
       );
    popup.menu(hsize(300, menu));
    return popup;
}

///////////////////////////////////////////////////////////////
// Options Menu
///////////////////////////////////////////////////////////////
inline auto make_option_popup()
{
    auto popup = button_menu("Options", menu_position::bottom_right);
    auto sk_param = shortcut_key{key_code::p, mod_action};

    auto params = menu_item("Parameters", sk_param);

    params.on_click = [&]()
    {
        options_dialog::get_instance()->open();
    };

    auto menu =
            layer(
                vtile(
                    params
                    ),
                panel{}
                );
    popup.menu(hsize(300, menu));
    return popup;
}


inline auto make_tracker_menus()
{
    return htile(
                hsize(120, make_file_popup()),
                hsize(120, make_option_popup()),
                box(colors::red.opacity(0))
                );
}


}

#endif // TRACKER_MENUS_H
