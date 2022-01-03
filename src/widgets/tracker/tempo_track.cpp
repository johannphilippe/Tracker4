#include "tempo_track.h"

////////////////////////////////////////////////////////////////////////
// Tempo track
////////////////////////////////////////////////////////////////////////
array_composite<2, vtile_element> tempo_track::make_buttons_tile()
{
    auto clear_but = icon_button(icons::cancel, 1.0, jtracker::theme.clear_buttons_color);
    clear_but.on_click = [&](bool)
    {
      t_content.clear_cells();
    };

    return vtile(htile(
            fixed_size_label<8>(std::string("tempo")),
            clear_but
            ), vspacer(5));
}

tempo_track::tempo_track()
    : array_composite<2, vtile_element>( vtile( make_buttons_tile(),link(t_content)  ))
{
    t_content.update_lines();
}

bool tempo_track::click(context const& ctx, mouse_button btn)
{
    context rctx {ctx, ctx.bounds};
    rctx.bounds.top += this->at(0).limits(ctx).max.y;
    if(rctx.bounds.includes(btn.pos))
    {
        return this->at(1).click(rctx, btn);
    }
    return array_composite<2, vtile_element>::click(ctx, btn);
}

bool tempo_track::text(const context &ctx, text_info info)
{
    return at(1).text(ctx, info);
}

////////////////////////////////////////////////////////////////////////
// Pointer class to tempo track
////////////////////////////////////////////////////////////////////////

tempo_track_ptr::tempo_track_ptr()
    : std::shared_ptr<tempo_track>(std::make_shared<tempo_track>())
{
}

////////////////////////////////////////////////////////////////////////
// Class that holds the pointer
// Needed to initialize the pointer first in the tempo_track_expander
////////////////////////////////////////////////////////////////////////

tempo_track_ptr_t::tempo_track_ptr_t()
    : ptr()
{}


////////////////////////////////////////////////////////////////////////
// Expander of tempo track
////////////////////////////////////////////////////////////////////////
tempo_track_expander::tempo_track_expander()
    : tempo_track_ptr_t()
    , expander_base< expandable_origin_mode::right, htile_composite>(jtracker::get_app()->_view,
                                                                     share(
                                                                         vscroller(
                                                                             right_margin(15, hold(this->ptr)))), 1)
{}

tempo_track_expander::tempo_track_expander(element_ptr elem)
    : expander_base<expandable_origin_mode::right, htile_composite>(jtracker::get_app()->_view, elem, 1)
{}
