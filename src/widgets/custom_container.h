#ifndef CUSTOM_CONTAINER_H
#define CUSTOM_CONTAINER_H
#include<elements.hpp>
using namespace cycfi::elements;

class highlight_container_base : public proxy_base
{
public:
    highlight_container_base(element_ptr ptr)
    {}

    void draw(context const& ctx) override
    {
        if(_is_focused)
        {
            ctx.canvas.line_width(1);
            ctx.canvas.stroke_color(colors::white_smoke);
            ctx.canvas.stroke_round_rect(ctx.bounds, 6);
            ctx.canvas.stroke();
        }

        subject().draw(ctx);
    }


    view_limits limits(basic_context const& ctx) const override
    {
        return subject().limits(ctx);
    }

    bool cursor(context const& ctx, point, cursor_tracking status) override
    {
        bool was_focused = _is_focused;
        if(status == cursor_tracking::leaving) {
            _is_focused = false;
            if(was_focused)
                ctx.view.refresh(*this);
            return true;
        }
        _is_focused = true;
        if(!was_focused)
            ctx.view.refresh(*this);
        return true;
    }

protected:
    bool _is_focused = false;
};


template<typename Subject>
inline proxy<cycfi::remove_cvref_t<Subject>, highlight_container_base>
highlight_container(Subject &&sub)
{
    return {std::forward<Subject>(sub)};
}

#endif // CUSTOM_CONTAINER_H
