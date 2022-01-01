#include<dialogs/elements_dialog_base.h>

void elements_dialog_base::make_content()
{
    // dummy content
    content_ptr = share(box(colors::blue));
}

void elements_dialog_base::make_dialog()
{
    make_content();
    point win_size = jtracker::get_app()->_win.size();
    dialog_ptr = share( closable_child_window(_name
                       , point(win_size.x / 2, win_size.y / 2)
                       , hold(content_ptr) ));
}

void elements_dialog_base::open()
{
    if(_is_displayed) return;
    make_dialog();
    jtracker::get_app()->_view.add(dialog_ptr);
    _is_displayed = true;
}

void elements_dialog_base::close()
{
    if(!_is_displayed) return;
    jtracker::get_app()->_view.remove(dialog_ptr);
    _is_displayed = false;
}

void elements_dialog::make_content()
{
    content_ptr = builder_function();
}
