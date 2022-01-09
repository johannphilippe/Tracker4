#ifndef COMMON_WIDGETS_H
#define COMMON_WIDGETS_H
#include<elements.hpp>
using namespace cycfi::elements;

inline auto make_tip(std::string text)
{
   return layer(
      margin({ 20, 8, 20, 8 }, label(text))
    , panel{}
   );
}

template<typename Subject>
inline auto labeled_box(Subject && s)
{
    return layer(
                std::forward<Subject>(s),
                rbox(colors::blue)
                );
}

struct empty_rbox : element
{
    empty_rbox(color c, float radius)
        : element()
        , _color(c)
        , _radius(radius)
    {}

    void draw(const context &ctx) override
    {
        auto& cnv = ctx.canvas;
        cnv.begin_path();
        cnv.add_round_rect(ctx.bounds, _radius);
        cnv.stroke_style(_color);
        cnv.stroke();
    }

    color _color;
    float _radius;
};


#endif // COMMON_WIDGETS_H
