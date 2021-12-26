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
    for(auto & it : events)
    {
        ctx.canvas.fill_color(get_event_color(it.type).opacity( it.selected ? 1 : 0.6));
        rect bnds = it.bounds;
        bnds.left += line_offset;
        bnds.right -= line_offset;
        ctx.canvas.fill_round_rect(it.bounds, 6);
        ctx.canvas.fill();
    }

}

bool sniper_track_content::click(const context &ctx, mouse_button btn)
{
    auto find_event = [&]()
    {
        for(int i = 0; i < static_cast<int>(events.size()); i++)
            if(events[i].bounds.includes(btn.pos))
                return i;
        return -1;
    };

    if(btn.down )
    {
        if(int i = find_event(); i != -1)
        {
            events[i].selected = !events[i].selected;
            ctx.view.refresh();
            return true;
            // event found, no new event

        }
        // no event found, create new one
        // 								Check if an event will fit
        if (btn.pos.y <  (ctx.bounds.height() + ctx.bounds.top - 20)  )
        {
            sniper_track_event event{rect(ctx.bounds.left, btn.pos.y - 10, ctx.bounds.right, btn.pos.y + 10), static_cast<track_event_type>(rand() % 6)};
            events.push_back(event);
            ctx.view.refresh(*this);
        }
    }
    return true;
}

view_limits sniper_track_content::limits(const basic_context &) const
{
    // get the font to make the line height equivalent to tracker track
    const float line_h = line_height() * num_lines;
    return {{250, line_h}, {300, line_h}};
}
