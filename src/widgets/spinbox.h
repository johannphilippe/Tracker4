#ifndef SPINBOX_H
#define SPINBOX_H

#include<elements.hpp>
#include<utilities/string_utilities.h>
#include<utilities/maths_utilities.h>
using namespace cycfi::elements;
using namespace cycfi::artist;

template<typename T>
struct spin_controller
{
    spin_controller(T min_, T max_, T init_, T step_) :
        value(init_), min(min_), max(max_), step(step_)
    {}
    T value, min, max, step;
};

/*****************************************
 * A scrollable, clickable, draggable Spinbox
 * (Number box with range and step)
*****************************************/
template<typename T>
class spinbox : public tracker<>
{

public:

    spinbox(spin_controller<T> control_) : tracker<>(), controller(control_),
        _text(std::to_string(controller.value)),
        on_change([&](T){}),
    focused(-1), clicked(false)
    {
        update_text();
    }

    virtual view_limits limits(basic_context const& ctx) const override;
    void draw(context const &ctx) override;
    bool click(context const &ctx, mouse_button btn) override;
    bool cursor(context const &ctx, point p, cursor_tracking)  override;
    bool scroll(context const&ctx, point dir, point ) override;
    void drag(const context &ctx, mouse_button btn) override;

    void increment()
    {
        controller.value =
                jtracker::maths::limit_number<T>
                (controller.min, controller.max, controller.value + controller.step);
        update_text();
    }

    void decrement()
    {
        controller.value =
                jtracker::maths::limit_number<T>
                (controller.min, controller.max, controller.value - controller.step);
        update_text();
    }

    void set_value(T val)
    {
        controller.value =
                jtracker::maths::limit_number<T>
                (controller.min, controller.max, val);
        update_text();
    }

    T get_value() {return controller.value;}

    spin_controller<T> controller;
    std::function<void(T)> on_change;

private:
    void update_text()
    {
        on_change(controller.value);
        _text = jtracker::string::get_string<T>(controller.value);
    }

    std::string _text;
    int focused;
    point click_pos;
    rect left_arrow_bounds, right_arrow_bounds;;
    bool clicked;

};

template<typename T>
inline auto make_labeled_spinbox(std::string text, spin_controller<T> ctrl, std::function<void(T)> on_change)
{
    spinbox<T> c(ctrl);
    c.on_change = on_change;
    return group(text, top_margin(40, margin({1,1,1,1}, vtile(hstretch(2, c)) ) ));
}

#endif // SPINBOX_H
