#include "sniper_track_content.h"

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

    rect track_bounds = ctx.bounds;
    track_bounds.height(line_h * jtracker::data.number_of_lines);

    //Background
    ctx.canvas.fill_color(jtracker::theme.sniper_track_background_color);
    ctx.canvas.fill_round_rect(track_bounds, 6);
    ctx.canvas.fill();

    auto size = ctx.bounds.size();
    auto pos = ctx.bounds.top_left();

    // Lines
    float y = pos.y + line_h;
    ctx.canvas.line_width(0.1);
    ctx.canvas.stroke_color(jtracker::theme.sniper_track_lines_color.opacity(1));
    for(size_t i = 1; i < jtracker::data.number_of_lines; i++)
    {
        ctx.canvas.move_to(pos.x + line_offset, y);
        ctx.canvas.line_to(pos.x + size.x - line_offset, y);
        y += line_h;
    }

    // Border
    ctx.canvas.stroke_color(colors::navajo_white);
    ctx.canvas.stroke_round_rect(track_bounds, 6);
    ctx.canvas.stroke();


    // Events
    for(size_t i = 0; i < events.size(); i++)
    {
        rect bnds = events[i].bounds;
        bnds.top += ctx.bounds.top;
        bnds.bottom += ctx.bounds.top;
        bnds.left = ctx.bounds.left;
        bnds.right = ctx.bounds.right;
        const color event_color = events[i].current_color->opacity(events[i].current_color->alpha - (_focused_event == static_cast<int>(i) ? 0.0 : 0.2));

        // Event rect
        ctx.canvas.fill_color(event_color);
        bnds.left += line_offset;
        bnds.right -= line_offset;
        // if overf
        if(bnds.bottom > ctx.bounds.bottom)
        {
            bnds.bottom = ctx.bounds.bottom;
        }

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
        point middle(bnds.left + bnds.width() / 2, bnds.top + 10);
        const float top_y = events[i].bounds.top / line_h;
        const float bot_y = events[i].bounds.bottom / line_h;
        std::string txt("top : " + jtracker::string::get_string<float>(top_y) +
                        " bottom : " + jtracker::string::get_string<float>(bot_y) );
        ctx.canvas.stroke_color(
                    jtracker::color_utilities::invert_color(
                        event_color).opacity(
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
              events[ix].current_color->set_color_target(events[ix].current_color->opacity(1.0));
          } else if(!was_selected)
          {      
              events[ix].selected = false;
              events[ix].current_color->set_color_target(events[ix].current_color->opacity(0.5));
          }
          jtracker::tracker_app::get_animator_pool().push_back(events[ix].current_color);
      }
      jtracker::tracker_app::get_animator_pool().animate(jtracker::tracker_app::get_instance()->_view);
    };

    if(btn.down )
    {
        // finds an event under the mouse
        if(int i = find_event(); i != -1)
        {
            if(_drag_resize_mode.resize_mode != sniper_track_drag_resize::mode::none)
            {
                return true;
            }
            if(btn.modifiers & mod_control)
            {
                events[i].selected = !events[i].selected;
                events[i].current_color->set_color_target(
                            events[i].current_color->opacity( events[i].selected ? 1.0 : 0.5));

                jtracker::tracker_app::get_animator_pool().push_back(events[i].current_color);
                jtracker::tracker_app::get_animator_pool().animate(jtracker::tracker_app::get_instance()->_view);
            }
            else // no modifiers
            {
                select_one(i);
                on_event(true, events[i]);
            }
            ctx.view.refresh();
            return true;
            // event found, no new event
        }

        // no event found, create new one
        // 								Check if an event will fit
        const float line_h = jtracker::data.track_line_height;
        if (btn.pos.y <  (ctx.bounds.height() + ctx.bounds.top - (line_h / 2) ) )
        {

            track_event_type type = static_cast<track_event_type>( (rand() % 5) + 1);
            sniper_track_event event{rect(ctx.bounds.left,  btn.pos.y - (line_h / 2) - ctx.bounds.top,
                                          ctx.bounds.right, btn.pos.y + (line_h / 2) - ctx.bounds.top),
                        type, true, std::make_shared<color_animation>(get_event_color(type).opacity(1))
                                    };
            unselect_events();
            events.push_back(event);

            _focused_event = events.size() -1;
            on_event(true, events.back());
            ctx.view.refresh(*this);
        }
    }
    return true;
}

bool sniper_track_content::cursor(const context &ctx, point p, cursor_tracking status)
{
    if(status == cursor_tracking::leaving)
    {
        set_cursor(cursor_type::arrow);
        ctx.view.refresh(*this);
    }

    // find bounds to change cursor for dragging resize
    auto find_up_low_bounds = [&](size_t ix) -> sniper_track_drag_resize::mode
    {
        const float top_y = events[ix].bounds.top + ctx.bounds.top;
        if( std::abs(p.y - top_y) < bounds_distance_selection)
            return sniper_track_drag_resize::mode::top;
        const float bottom_y = events[ix].bounds.bottom + ctx.bounds.top;
        if( std::abs(bottom_y - p.y) < bounds_distance_selection)
            return sniper_track_drag_resize::mode::bottom;
        return sniper_track_drag_resize::mode::none;
    };


    _should_refresh = false;

    for(size_t i = 0; i < events.size(); i++)
    {
        if(find_up_low_bounds(i) != sniper_track_drag_resize::mode::none)
        {
            set_cursor(cursor_type::v_resize);
            _should_refresh = true;
        }
        else
        {
            set_cursor(cursor_type::cross_hair);
            _should_refresh = true;
        }

        // select focused event
        rect bnds = events[i].bounds;
        bnds.top += ctx.bounds.top - bounds_distance_selection;
        bnds.bottom += ctx.bounds.top + bounds_distance_selection;
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
    // if trying to resize
    const float line_h = jtracker::data.track_line_height;
    switch (_drag_resize_mode.resize_mode) {
    case sniper_track_drag_resize::mode::none:
    {
        break;
    }
    case sniper_track_drag_resize::mode::top:
    {
        const float diff_top_y = btn.pos.y  - events[_focused_event].bounds.top - ctx.bounds.top;
        events[_drag_resize_mode.index].bounds.top += diff_top_y;
        if(events[_drag_resize_mode.index].bounds.height() < line_h)
            events[_drag_resize_mode.index].bounds.top -= (line_h - events[_drag_resize_mode.index].bounds.height());
        on_event(true, events[_drag_resize_mode.index]);
        ctx.view.refresh(*this);
        return;
    }
    case sniper_track_drag_resize::mode::bottom:
    {
        const float diff_bottom_y = btn.pos.y  - events[_focused_event].bounds.bottom - ctx.bounds.top;
        events[_drag_resize_mode.index].bounds.bottom += diff_bottom_y;
        if(events[_drag_resize_mode.index].bounds.height() < line_h)
            events[_drag_resize_mode.index].bounds.bottom += (line_h - events[_drag_resize_mode.index].bounds.height());
        on_event(true, events[_drag_resize_mode.index]);
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
            // This is top limit. When sequences are implemented, an event should be able to fit in two seqs,
            // so allow overflow in top (if seq > 1) and bottom
            // if those situations occurs, we should register those events as transversal across sequences
            if(it.bounds.top < 0 && it.bounds.bottom > 0) {
                it.bounds.top = 0;
                break;
            } else if(it.bounds.bottom + ctx.bounds.top > ctx.bounds.bottom
                      &&
                      it.bounds.top < ctx.bounds.bottom)
            {
                it.bounds.bottom = ctx.bounds.bottom - ctx.bounds.top;
                break;
            }
            it.bounds.bottom += item_diff;
        }
    }
    if(_should_refresh) {
        on_event(true, events[_focused_event]);
        //ctx.view.refresh();
    }
}

bool sniper_track_content::scroll(context const& ctx, point dir, point)
{
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
    const float line_h = line_height() * jtracker::data.number_of_lines;
    return {{250, line_h}, {300, line_h}};
}

void sniper_track_content::unselect_events()
{
    for(auto & it : events)
    {
        it.selected = false;
        it.current_color->set_color_target(it.current_color->opacity(0.5));
        jtracker::tracker_app::get_animator_pool().push_back(it.current_color);
    }
    jtracker::tracker_app::get_animator_pool().animate(jtracker::get_app()->_view);
}
