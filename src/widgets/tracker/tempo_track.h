#ifndef TEMPO_TRACK_H
#define TEMPO_TRACK_H

#include<elements.hpp>
#include<widgets/custom_labels.h>
#include<widgets/expandable.h>
#include<widgets/tracker/tempo_track_line.h>
#include<widgets/track_content.h>
#include<widgets/synced_scrollers.h>
using namespace cycfi::elements;

class tempo_track : public array_composite<2, vtile_element>
{
public:
    array_composite<2, vtile_element> make_buttons_tile();
    tempo_track();

    bool click(context const& ctx, mouse_button btn) override;
    bool text(context const& ctx, text_info info) override;
    //void end_focus() override;

    track_content<tempo_track_line> t_content;
};

class tempo_track_ptr : public std::shared_ptr<tempo_track>
{
public:
    tempo_track_ptr();
};

class tempo_track_ptr_t
{
public:
    tempo_track_ptr_t();
    tempo_track_ptr ptr;
};

class tempo_track_expander : public tempo_track_ptr_t,
        public expander_base< expandable_origin_mode::right, htile_composite>
{
public:
    tempo_track_expander();
    tempo_track_expander(element_ptr sub);
};



#endif // TEMPO_TRACK_H
