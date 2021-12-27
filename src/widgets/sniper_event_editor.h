#ifndef SNIPER_EVENT_EDITOR_H
#define SNIPER_EVENT_EDITOR_H
#include<elements.hpp>
#include<widgets/spinbox.h>
#include<models/sniper_track_event.h>
#include<tracker/jtracker_globals.h>
using namespace cycfi::elements;



class sniper_event_editor : public array_composite<2, vtile_element>
{
public:
    sniper_event_editor(sniper_track_event& event) :
        array_composite<2, vtile_element>(vtile(
                                              vspacer(event.bounds.top),
                                              htile(
                                                  link(top_spin),
                                                  link(bottom_spin)
                                                  )
                                              )
                                          )
    {
        set_data(event);
    }

    fixed_width_spinbox<float, 100> top_spin, bottom_spin;

    void set_data(sniper_track_event& event)
    {
        const float &line_h = jtracker::data.track_line_height;
        top_spin.set_controller(spin_controller<float>(
                    0,
                    event.bounds.bottom / line_h - 1,
                    event.bounds.top / line_h,
                    0.05));
        bottom_spin.set_controller(spin_controller<float>(
                    top_spin.controller.value + 1,
                    jtracker::data.number_of_lines,
                    event.bounds.bottom / line_h,
                    0.05));
        top_spin.on_change = [&](float val)
        {
            bottom_spin.controller.min = val + 1;
            event.bounds.top = val * line_h;
        };
        bottom_spin.on_change = [&](float val)
        {
            top_spin.controller.max = val - 1;
            event.bounds.bottom = val * line_h;
        };

        this->at(0) = vspacer(event.bounds.top);
    }


    view_limits limits(basic_context const& ctx) const override
    {
        view_limits l = array_composite<2, vtile_element>::limits(ctx);
        return {{200, l.min.y}, {200, l.max.y}};
    }
private:
};

#endif // SNIPER_EVENT_EDITOR_H
