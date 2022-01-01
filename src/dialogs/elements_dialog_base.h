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
    virtual void make_content();

    void make_dialog();

    void open();
    void close();


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
    {}

    void make_content() override;
private:
    std::function<element_ptr(void)> builder_function;
};

#endif // ELEMENTS_DIALOG_BASE_H
