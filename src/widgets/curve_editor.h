#ifndef CURVE_EDITOR_H
#define CURVE_EDITOR_H

#include<elements.hpp>
#include<models/curve_point.h>
#include<models/curve.h>
#include<maths/cubic_spline.h>

/*
 * Curve Editor
 * A graphic 2D editor for time curves allowing different curve types :
 * - Linear
 * - Logarithmic / Exponential
 * - Cubic Spline
 * - Cubic Bezier
 * - Quadratic Bezier
*/

using namespace cycfi::elements;
using namespace cycfi::artist;

enum class curve_mode {
        linear = 0,
        log_exp = 1,
        quadratic_bezier = 2,
        cubic_bezier = 3,
        cubic_spline = 4,
};

struct curve_editor_controller
{
    curve_editor_controller(curve_mode m = curve_mode::linear,
                            int grain = 2048, int grid_steps_ = 10) :
        mode(m),
        granularity(grain),
        grid_steps(grid_steps_)
    {}
    curve_mode mode;
    int granularity;
    int grid_steps;
    bool grid_enabled = true;
    curve samples;
    color background_color, grid_color, curve_color, button_color;
};

class curve_editor : public tracker<>, receiver<curve_editor_controller>
{
public:
    constexpr static const  float selection_distance = 0.02f;

    curve_editor() :  controller(curve_mode::linear, 2048, 10) //grid_steps(10), granularity(2048), mode(curve_mode::linear)
    {}

    // Control methods
    curve &get_curve() {return controller.samples;}
    void clear() {controller.samples.clear();}
    void set_curve(curve crv) {controller.samples = crv;}

    //internal draw and action methods
    bool key(const context &ctx, key_info k) override;
    bool scroll(context const&ctx, point dir, point p) override;
    void draw(const context &ctx) override;
    void layout(const context &ctx) override;
    bool click(const context &ctx, mouse_button btn) override;
    void end_tracking(const context &, tracker_info &) override;
    void drag(const context &ctx, mouse_button btn) override;
    curve_editor_controller value() const override;
    void value(curve_editor_controller val) override;
    bool cursor(context const &ctx, point p, cursor_tracking status)  override;

    float get_logexp_curve(float beg, float ending, int dur, int idx, float typ);
    void add_sample(point &p, const context &ctx);
    void draw_segments(const context &ctx);
    void draw_samples(const context &ctx);
    void snap_to_grid(const context &ctx);
    void draw_grid(const context &ctx);
    int find_sample(point &p, const context &ctx);
    point in_bounds(const context &ctx,  point &p);

private:
    curve_editor_controller controller;
    point last_position;
    curves::cubic_spline<float> spline;
    int selected = -1;
    int focused = -1;

};


#endif // CURVE_EDITOR_H
