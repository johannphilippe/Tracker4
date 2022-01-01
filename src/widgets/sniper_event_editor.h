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
    sniper_event_editor(sniper_track_event& event);

    fixed_width_spinbox<float, 100> top_spin, bottom_spin;

    void set_data(sniper_track_event& event);
    view_limits limits(basic_context const& ctx) const override;
private:
};

#endif // SNIPER_EVENT_EDITOR_H
