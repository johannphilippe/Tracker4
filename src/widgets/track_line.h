#ifndef TRACK_LINE_H
#define TRACK_LINE_H

#include<widgets/track_cell.h>
#include<widgets/custom_labels.h>

/*
 * Represents a line of cells.
*/
class track_line : public htile_composite
{
public:

    track_line(size_t l_idx, track_event_type t)
        : htile_composite()
        , line_index(l_idx)
        , event_type(t)
    {}

    track_line(size_t num, size_t l_idx,
               track_event_type ev_type = track_event_type::none) :
        htile_composite(),
        line_index(l_idx),
        event_type(ev_type)
    {
        update_label(true);
        set_num_cols(num);
    }

    virtual void set_num_cols(size_t num_cols);
    void update_label(bool create = false);

    std::function<void(context const& ctx, mouse_button btn, size_t, size_t)> *click_cbk = nullptr;
    std::function<void(size_t, size_t, std::string_view)> *text_cbk = nullptr;

    std::vector<std::shared_ptr<track_cell>> cells;
    size_t line_index = 0;

    int _to_focus;

    track_event_type event_type;

    std::function<void(size_t line_idx, size_t cell_idx, std::string_view t)> text_callback =
            [](size_t, size_t, std::string_view) {};
};


#endif // TRACK_LINE_H
