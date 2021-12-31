#ifndef ELEMENTS_DIALOG_BASE_H
#define ELEMENTS_DIALOG_BASE_H

#include<elements.hpp>
#include<tracker/jtracker.h>
using namespace cycfi::elements;

// internal dialog to the main window
class elements_dialog_base
{
public:
    elements_dialog_base() {}
    elements_dialog_base(std::string name)
        : _name(name)
    {}
    virtual void make_content() {content_ptr = share(box(colors::blue));}

    void make_dialog()
    {
        make_content();
        point win_size = jtracker::get_app()->_win.size();
        dialog_ptr = share( closable_child_window(_name
                                           , point(win_size.x / 2, win_size.y / 2)
                                           , link(*content_ptr) ));
    }

    void open()
    {
        if(_is_displayed) return;
        make_dialog();
        jtracker::get_app()->_view.add(dialog_ptr);
        _is_displayed = true;
    }

    void close()
    {
        if(!_is_displayed) return;
        jtracker::get_app()->_view.remove(dialog_ptr);
        _is_displayed = false;
    }


protected:
    std::string _name;
    bool _is_displayed = false;
    element_ptr content_ptr = nullptr;
    element_ptr dialog_ptr = nullptr;

};


class elements_dialog : public elements_dialog_base
{
public:
    elements_dialog(std::string name, std::function<element_ptr(void)> element_builder)
        : elements_dialog_base(name)
        , builder_function(element_builder)

    {
    }
    void make_content() override
    {
        content_ptr = builder_function();
    }

private:
    std::function<element_ptr(void)> builder_function;
};

#endif // ELEMENTS_DIALOG_BASE_H
