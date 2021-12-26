#include "sniper_track_content.h"

void sniper_track_content::set_num_lines(size_t lines)
{
    num_lines = lines ;
}

void sniper_track_content::clear() {}

float sniper_track_content::line_height() const
{
    font f (jtracker::theme.cell_box_font);
    auto m = f.metrics();
    return (m.ascent + m.descent + m.leading + 5);
}

void sniper_track_content::draw(const context &ctx)
{
    const float line_h = line_height();

    //Background
    ctx.canvas.fill_color(jtracker::theme.sniper_track_background_color);
    ctx.canvas.fill_round_rect(ctx.bounds, 6);
    ctx.canvas.fill();

    auto size = ctx.bounds.size();
    auto pos = ctx.bounds.top_left();

    // Lines
    float y = pos.y + line_h;
    ctx.canvas.line_width(0.1);
    ctx.canvas.stroke_color(jtracker::theme.sniper_track_lines_color.opacity(1));
    for(size_t i = 1; i < num_lines; i++)
    {
        ctx.canvas.move_to(pos.x + line_offset, y);
        ctx.canvas.line_to(pos.x + size.x - line_offset, y);
        y += line_h;
    }

    // Border
    ctx.canvas.stroke_color(colors::navajo_white);
    ctx.canvas.stroke_round_rect(ctx.bounds, 6);
    ctx.canvas.stroke();


    // Events
    for(size_t i = 0; i < events.size(); i++)
    {
        rect bnds = events[i].bounds;
        bnds.top += ctx.bounds.top;
        bnds.bottom += ctx.bounds.top;
        if(bnds.top < ctx.bounds.top || bnds.bottom > ctx.bounds.bottom) continue;

        const color c = get_event_color(events[i].type).opacity(
                    (events[i].selected ? 1 : 0.5) - (static_cast<int>(i) == _focused_event ? 0.0 : 0.2)
                    );

        // Event rect
        ctx.canvas.fill_color(c);
        bnds.left += line_offset;
        bnds.right -= line_offset;
        ctx.canvas.fill_round_rect(bnds, 6);
        ctx.canvas.fill();
        // if focused, bound rectangle
        if(static_cast<int>(i) == _focused_event)
        {
            ctx.canvas.line_width(3);
            ctx.canvas.stroke_color(
                        jtracker::color_utilities::invert_color(get_event_color(events[i].type))
                        );
            ctx.canvas.stroke_round_rect(bnds, 6);
            ctx.canvas.stroke();
        }

        // Events text indications
        ctx.canvas.line_width(1);
        point middle(bnds.left + bnds.width() / 2, bnds.top + 5);
        std::string txt("top : " + jtracker::string::get_string<float>(events[i].bounds.top) +
                        " bottom : " + jtracker::string::get_string<float>(events[i].bounds.bottom) );
        ctx.canvas.stroke_color(
                    jtracker::color_utilities::invert_color(
                        jtracker::theme.sniper_track_background_color).opacity(
                        _focused_event == static_cast<int>(i) ? 1.0 : 0.5
                        )
                    );
        ctx.canvas.font(jtracker::theme.sniper_track_font);
        ctx.canvas.stroke_text(txt, middle);
        ctx.canvas.stroke();
    }
}

bool sniper_track_content::click(const context &ctx, mouse_button btn)
{
    auto find_up_low_bounds = [&](size_t ix) -> sniper_track_drag_resize::mode
    {
        const float top_y = events[ix].bounds.top + ctx.bounds.top;
        if(std::abs(top_y - btn.pos.y) < bounds_distance_selection)
            return sniper_track_drag_resize::mode::top;
        const float bottom_y = events[ix].bounds.bottom + ctx.bounds.top;
        if(std::abs(bottom_y - btn.pos.y) < bounds_distance_selection)
            return sniper_track_drag_resize::mode::bottom;
        return sniper_track_drag_resize::mode::none;
    };

    auto find_event = [&]()
    {
        for(int i = 0; i < static_cast<int>(events.size()); i++)
        {
            _drag_resize_mode.resize_mode = find_up_low_bounds(i);
            if(_drag_resize_mode.resize_mode != sniper_track_drag_resize::mode::none)
            {
                _drag_resize_mode.index = i;
                return i;
            }

            point p = btn.pos;
            p.y -= ctx.bounds.top;
            if(events[i].bounds.includes(p))
                return i;
        }
        return -1;
    };

    auto select_one = [&](size_t sel)
    {

      bool was_selected = events[sel].selected;
      for(size_t ix = 0; ix < events.size(); ix++)
      {
          if(ix == sel ) {
              events[ix].selected = true; //events[ix].selected;
          } else if(!was_selected)
          {
              events[ix].selected = false;
          }
      }
    };

    if(btn.down )
    {
        if(int i = find_event(); i != -1)
        {
            if(_drag_resize_mode.resize_mode != sniper_track_drag_resize::mode::none)
            {
                return true;
            }
            if(btn.modifiers & mod_control)
            {
                events[i].selected = !events[i].selected;
            }
            else
            {
                select_one(i);
            }
            ctx.view.refresh();
            return true;
            // event found, no new event
        }

        // no event found, create new one
        // 								Check if an event will fit
        if (btn.pos.y <  (ctx.bounds.height() + ctx.bounds.top - 10)  )
        {
            sniper_track_event event{rect(ctx.bounds.left,  btn.pos.y - 10 - ctx.bounds.top,
                                          ctx.bounds.right, btn.pos.y + 10 - ctx.bounds.top),
                        static_cast<track_event_type>(rand() % 6), true};
            for(auto & it : events) it.selected = false;
            events.push_back(event);
            _focused_event = events.size() -1;
            ctx.view.refresh(*this);
        }
    }
    return true;
}

bool sniper_track_content::cursor(const context &ctx, point p, cursor_tracking status)
{
    auto find_up_low_bounds = [&](size_t ix) -> sniper_track_drag_resize::mode
    {
        const float top_y = events[ix].bounds.top + ctx.bounds.top;
        if(std::abs(top_y - p.y) < bounds_distance_selection)
            return sniper_track_drag_resize::mode::top;
        const float bottom_y = events[ix].bounds.bottom + ctx.bounds.top;
        if(std::abs(bottom_y - p.y) < bounds_distance_selection)
            return sniper_track_drag_resize::mode::bottom;
        return sniper_track_drag_resize::mode::none;
    };


    _should_refresh = false;
    for(size_t i = 0; i < events.size(); i++)
    {
        if(find_up_low_bounds(i) != sniper_track_drag_resize::mode::none)
            set_cursor(cursor_type::v_resize);
        else
            set_cursor(cursor_type::cross_hair);
        rect bnds = events[i].bounds;
        bnds.top += ctx.bounds.top;
        bnds.bottom += ctx.bounds.top;
        if(bnds.includes(p))
        {
            _focused_event = i;
            _should_refresh = true;
        } else {
            if(_focused_event == static_cast<int>(i)) {
                _focused_event = -1;
                _should_refresh = true;
            }
        }
    }

    if(_should_refresh) {
        ctx.view.refresh(*this);
        return true;
    }
    return false;
}

void sniper_track_content::drag(const context &ctx, mouse_button btn)
{
    if(_focused_event == -1) return;
    _should_refresh = false;
    switch (_drag_resize_mode.resize_mode) {
    case sniper_track_drag_resize::mode::none:
    {
        break;
    }
    case sniper_track_drag_resize::mode::top:
    {
        const float diff_top_y = btn.pos.y  - events[_focused_event].bounds.top - ctx.bounds.top;
        events[_drag_resize_mode.index].bounds.top += diff_top_y;
        ctx.view.refresh(*this);
        return;
    }
    case sniper_track_drag_resize::mode::bottom:
    {
        const float diff_bottom_y = btn.pos.y  - events[_focused_event].bounds.bottom - ctx.bounds.top;
        events[_drag_resize_mode.index].bounds.bottom += diff_bottom_y;
        ctx.view.refresh(*this);
        return;
    }
    }

    const float diff_y = btn.pos.y  - events[_focused_event].bounds.top - ctx.bounds.top;
    // move difference if middle of event
    for(auto & it : events)
    {
        if(it.selected)
        {
            const float item_diff = diff_y - (events[_focused_event].bounds.height() / 2);
            _should_refresh = true;
            it.bounds.top += item_diff;
            it.bounds.bottom += item_diff;
        }
    }
    if(_should_refresh) ctx.view.refresh(*this);
}


bool sniper_track_content::scroll(context const& ctx, point dir, point p)
{
    std::cout << "scroll event passed to canvas " << std::endl;
    _should_refresh = false;
    for(size_t i = 0; i < events.size(); i++)
    {
        if(events[i].selected)
        {
            const float new_bottom = events[i].bounds.bottom + dir.y;
            if(new_bottom <= 1) continue;
            events[i].bounds.bottom = new_bottom;
            _should_refresh = true;

        }
    }
    if(_should_refresh) ctx.view.refresh(*this);
    return true;
}

view_limits sniper_track_content::limits(const basic_context &) const
{
    // get the font to make the line height equivalent to tracker track
    const float line_h = line_height() * num_lines;
    return {{250, line_h}, {300, line_h}};
}
