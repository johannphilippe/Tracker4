#ifndef TRACKER_MENUS_H
#define TRACKER_MENUS_H

#include<elements.hpp>
#include<dialogs/options_dialog.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

namespace jtracker {

auto make_file_popup()
{
    auto popup  = button_menu("File", menu_position::bottom_right);

    auto sk_new = shortcut_key{key_code::n, mod_action};
    auto sk_open = shortcut_key{key_code::o, mod_action};
    auto sk_save = shortcut_key{key_code::s, mod_action};

    auto new_file = menu_item("New project", sk_new);
    auto open_file = menu_item("Open project", sk_open);
    auto save_file = menu_item("Save file", sk_save);

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

auto make_option_popup()
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


auto make_tracker_menus()
{
    return htile(make_file_popup(), make_option_popup());
}


}

#endif // TRACKER_MENUS_H
