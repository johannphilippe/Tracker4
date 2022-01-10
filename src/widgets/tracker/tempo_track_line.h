#ifndef TEMPO_TRACK_LINE_H
#define TEMPO_TRACK_LINE_H

#include<widgets/track_line.h>

class tempo_track_line : public track_line
{
public:
    tempo_track_line(size_t num, size_t line_index, track_event_type t = track_event_type::none);
    void set_num_cols(size_t num_cols) override;
};


#endif // TEMPO_TRACK_LINE_H
