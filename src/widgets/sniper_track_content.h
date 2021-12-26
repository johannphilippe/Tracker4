#ifndef SNIPER_TRACK_CONTENT_H
#define SNIPER_TRACK_CONTENT_H

#include<elements.hpp>
#include<tracker/jtracker_theme.h>
#include<iostream>
#include<vector>
#include<models/sniper_track_event.h>
#include<models/track_event.h>
#include<utilities/paint_utilities.h>
#include<utilities/string_utilities.h>
using namespace cycfi::elements;

class sniper_track_content : public tracker<>
{
public:
    constexpr static const float bounds_distance_selection = 5.0f;

    sniper_track_content(size_t nlines = 16) :
        tracker<>(), num_lines(nlines)
    {}

    view_limits limits(basic_context const& ctx) const override;

    void draw(context const& ctx) override;
    bool click(const context &ctx, mouse_button btn) override;
    bool cursor(context const& ctx, point p, cursor_tracking status) override;
    void drag(const context &ctx, mouse_button btn) override;
    bool scroll(context const& ctx, point dir, point p) override;

    void set_num_lines(size_t lines);

    void clear();

    size_t num_lines;

    std::vector<sniper_track_event> events;
protected:

    constexpr static const float line_offset = 20;
    float line_height() const;
    int _focused_event = -1;
    bool _should_refresh = false;
    sniper_track_drag_resize _drag_resize_mode;
};

#endif // SNIPER_TRACK_CONTENT_H
