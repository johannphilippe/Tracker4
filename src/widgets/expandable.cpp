#include "expandable.h"



/*
expandable_base::expandable_base(expandable_origin_mode mode)
    : proxy_base()
    , _mode(mode)
    , _expandable_icon(_get_icon())
{}

void expandable_base::draw(context const& ctx)
{
    rect subject_bounds = ctx.bounds;
    rect surface_bounds = subject_bounds;
    rect icon_bounds;

    auto top = [&]()
    {
      subject_bounds.bottom -= weak_axis_size;
      if(_expanded) {
          surface_bounds.top = ctx.bounds.top + subject_bounds.bottom;
      }
      surface_bounds.height(weak_axis_size);
      //dummy
      icon_bounds = rect(surface_bounds.left + surface_bounds.width() / 2.0f , surface_bounds.top, surface_bounds.right - surface_bounds.width() / 2, surface_bounds.bottom);
    };

    switch(_mode)
    {
    case expandable_origin_mode::top:
    {
        top();
        break;
    }
    case expandable_origin_mode::left:
    {
        break;
    }
    case expandable_origin_mode::right:
    {
        break;
    }
    case expandable_origin_mode::bottom:
    {
        break;
    }
    }

    if(_expanded)
    {
        context rctx{ctx, subject_bounds};
        subject().draw(rctx);
    }
    ctx.canvas.fill_color(get_theme().panel_color);
    ctx.canvas.fill_rect(subject_bounds);
    ctx.canvas.fill();

    float fs = get_theme().icon_font._size;


    draw_icon(ctx.canvas, icon_bounds, _expandable_icon, fs ,colors::red);

}

bool expandable_base::click(context const&ctx, mouse_button btn)
{
    element * el = hit_test(ctx, btn.pos);
    if(el == &subject()) {
        std::cout << "hit subject "<< std::endl;
        subject().click(ctx, btn);
        return true;
    }
    // find if the click is on expandable surface, or on button
    rect subject_bounds = ctx.bounds;
    std::cout << "subject bounds" << subject_bounds.height()  << " " << subject_bounds.top << std::endl;

    if(!btn.down) return true;
    auto top = [&]()
    {

        //subject_bounds.bottom -= weak_axis_size;
        //rect surface(ctx.bounds.left, _expanded ? ctx.bounds.top + subject_bounds.height() : ctx.bounds.top, ctx.bounds.right, ctx.bounds.bottom);
        //if(surface.includes(btn.pos)) {
            _expanded = !_expanded;
            _expandable_icon = _get_icon();
            ctx.view.layout();
            ctx.view.refresh();
        //}
    };


    switch (_mode) {

    case expandable_origin_mode::top:
    {
        top();
        break;
    }
    case expandable_origin_mode::left:
    {
        break;
    }
    case expandable_origin_mode::right:
    {
        break;
    }
    case expandable_origin_mode::bottom:
    {
        break;
    }
    }

    return true;
}

view_limits expandable_base::limits(const basic_context &ctx) const
{
    view_limits l ;
    view_limits subject_limits = subject().limits(ctx);
    // plan to have enough room for a surface for expandable button

    auto vertical_bar = [&]()
    {
        l.min.x = weak_axis_size;
        l.max.x = weak_axis_size; // size_of_bar
        l.min.y = subject_limits.min.y;
        l.max.y = subject_limits.max.y;
        if(_expanded)
        {
            l.min.x += subject_limits.min.x;
            l.max.x += subject_limits.max.x;
        }
    };

    auto horizontal_bar = [&]()
    {
        l.min.y = weak_axis_size;
        l.max.y = weak_axis_size; // size_of_bar
        l.min.x = subject_limits.min.x;
        l.max.x = subject_limits.max.x;
        if(_expanded)
        {
            l.min.y += subject_limits.min.y;
            l.max.y += subject_limits.max.y;
        }
    };

    switch (_mode) {
    case expandable_origin_mode::top:
    {
        horizontal_bar();
        break;
    }
    case expandable_origin_mode::left:
    {
        vertical_bar();
        break;
    }
    case expandable_origin_mode::right:
    {
        vertical_bar();
        break;
    }
    case expandable_origin_mode::bottom:
    {
        horizontal_bar();
        break;
    }
    }
    return l;
}

uint32_t expandable_base::_get_icon()
{
    switch (_mode) {
    case expandable_origin_mode::top:
    {
        return (_expanded) ? icons::up_dir : icons::down_dir;
        break;
    }
    case expandable_origin_mode::left:
    {
        return (_expanded) ? icons::left_dir : icons::right_dir;
        break;
    }
    case expandable_origin_mode::right:
    {
        return (_expanded) ? icons::right_dir : icons::left_dir;
        break;
    }
    case expandable_origin_mode::bottom:
    {
        return (_expanded) ? icons::down_dir : icons::up_dir;
        break;
    }
    }
}

void expandable_base::prepare_subject(context &ctx)
{
    view_limits subject_limits = subject().limits(ctx);
    view_limits l;
    l.min.y = weak_axis_size;
    l.max.y = weak_axis_size; // size_of_bar
    l.min.x = subject_limits.min.x;
    l.max.x = subject_limits.max.x;
    if(_expanded)
    {
        l.min.y += subject_limits.min.y;
        l.max.y += subject_limits.max.y;
    }

    ctx.bounds.height( l.max.y );
}
*/
/*

template <typename Subject>
inline void halign_element<Subject>::prepare_subject(context& ctx)
{
   view_limits    e_limits          = this->subject().limits(ctx);
   float          elem_width        = e_limits.min.x;
   float          available_width   = ctx.bounds.width();

   if (available_width > elem_width)
      elem_width = std::min(available_width, e_limits.max.x);

   ctx.bounds.left += (available_width - elem_width) * this->align();
   ctx.bounds.width(elem_width);
}

*/
