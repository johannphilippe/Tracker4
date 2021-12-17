#include<widgets/curve_editor.h>

    // Get simple logarithmic or exponential curve interpolation point
    float curve_editor::get_logexp_curve(float beg, float ending, int dur, int idx, float typ)
    {
      double res;
      double type;
      if(typ == 0) {
        res = beg + (idx * (( ending - beg) / dur));
      }else {
        if(typ > 10) type = 10;
        else if(typ <  -10) type = -10;
        else type = typ;
        res = beg + (ending - beg) * (1 - exp(idx * type / (dur - 1))) / (1-exp(type));
      }
      return res;
    }



    // find a sample according to distance
    int curve_editor::find_sample(point &p, const context &ctx)
    {
        auto size = ctx.bounds.size();
        auto pos = ctx.bounds.top_left();
        const point relative( (p.x - pos.x) / size.x, (p.y - pos.y) / size.y);

        for(size_t i = 0; i < controller.samples.size(); i++) {
            const float distance = controller.samples[i].distance_to(relative);
            if(distance < selection_distance) {
                return i;
            }
        }
        return -1;
    }

    void curve_editor::add_sample(point &p, const context &ctx)
    {
        auto pos = ctx.bounds.top_left();
        auto size = ctx.bounds.size();
        curve_point cp( (p.x - pos.x) / size.x ,  (p.y - pos.y) / size.y , 0.0);
        if(controller.samples.empty()) {
            controller.samples.push_back(cp);
            selected = 0;
            return;
        }
        if(controller.samples[0].x > cp.x) {
            controller.samples.insert(controller.samples.begin(), cp);
            selected = 0;
            return;
        }

        if(controller.samples.back().x < cp.x) {
            controller.samples.push_back(cp);
            selected = controller.samples.size() - 1;
            return;
        }
        for(size_t i = 0; i < controller.samples.size() - 1; i++) {
            if( controller.samples[i].x < cp.x && controller.samples[i+1].x > cp.x)
            {
                controller.samples.insert(controller.samples.begin() + i + 1, cp);
                selected = i + 1;
                return;
            }
        }
    }

    void curve_editor::draw_segments(const context &ctx)
    {
        if(controller.samples.empty()) return;
        canvas &cnv = ctx.canvas;
        auto size = ctx.bounds.size();
        auto pos = ctx.bounds.top_left();
        cnv.stroke_color(colors::purple);
        cnv.line_width(1.5);
        switch(controller.mode)
        {
        case curve_mode::linear:
        {
            for(size_t i = 0; i < controller.samples.size() - 1; i++) {

                if(i == 0 ) {
                    const point p(controller.samples[i].x * size.x + pos.x, controller.samples[i].y * size.y + pos.y);
                    cnv.move_to(p);
                }
                const point p2(controller.samples[i + 1].x * size.x + pos.x, controller.samples[i + 1].y * size.y + pos.y);
                cnv.line_to(p2);
            }

            cnv.stroke();
            break;
        }
        case curve_mode::log_exp:
        {

            const point origin(controller.samples[0].x * size.x + pos.x, controller.samples[0].y * size.y + pos.y);
            cnv.move_to(origin);
            for(size_t i = 0; i < controller.samples.size() - 1; i++) {
                if(controller.samples[i].curve == 0.0f) {
                    const point p2(controller.samples[i + 1].x * size.x + pos.x, controller.samples[i + 1].y * size.y + pos.y);
                    cnv.line_to(p2);
                } else {
                    const float granularized_begx = controller.samples[i].x * controller.granularity;
                    const float granularized_fbx = controller.samples[i + 1].x * controller.granularity;
                    const float amp_x = abs(granularized_begx - granularized_fbx);
                    for(int l_idx = 0; l_idx < int(amp_x); l_idx++ ) {
                        const float curve =  (controller.samples[i].y > controller.samples[i + 1].y) ? -controller.samples[i].curve :  controller.samples[i].curve;
                        const float y_val = get_logexp_curve(controller.samples[i].y, controller.samples[i + 1].y, int(amp_x), l_idx, curve) * size.y + pos.y;
                        const float x_val = ((l_idx + granularized_begx) / controller.granularity) * size.x + pos.x;
                        const point p2(x_val, y_val);
                        cnv.line_to(p2);
                    }
                }
            }
            cnv.stroke();
            break;
        }
        case curve_mode::cubic_spline:
        {
            if(controller.samples.size() < 3) return;
            const float step = 1.0f / float(controller.granularity) * size.x;
            vector<float> &vec = spline.interpolate_from_points(controller.samples, controller.granularity, size);


            point p1, p2;
            const int start_x = int(controller.samples.front().x * controller.granularity) + 1;
            const int end_x = int(controller.samples.back().x * controller.granularity) - 1;

            p1 = point( (float(start_x) / float(controller.granularity) ) * size.x + pos.x, (vec[start_x] * size.y) + pos.y);
            p1 = in_bounds(ctx, p1);
            cnv.move_to(p1);

            for(int i = start_x; i < end_x; i++)
            {
                const float x_relative = float(i) / float(controller.granularity);
                const float x_absolute = ( x_relative )  * size.x + pos.x;
                p2 = point(x_absolute + step, vec[i+1] * size.y + pos.y);
                p2 = in_bounds(ctx, p2);
                cnv.line_to(p2);
            }
            cnv.stroke();
            break;
        }

        case curve_mode::cubic_bezier:
        {
            if(controller.samples.size() < 4) return;

            const point p1(controller.samples[0].x * size.x  + pos.x, controller.samples[0].y * size.y + pos.y);
            cnv.move_to(p1);

            for(size_t i = 0; i < controller.samples.size() - 3; i+=3) {
                const point p2(controller.samples[i + 1].x * size.x + pos.x, controller.samples[i + 1].y * size.y + pos.y);
                const point p3(controller.samples[i + 2].x * size.x + pos.x, controller.samples[i + 2].y * size.y + pos.y);
                const point p4(controller.samples[i + 3].x * size.x + pos.x, controller.samples[i + 3].y * size.y + pos.y);
                cnv.bezier_curve_to(p2, p3, p4);
            }

            cnv.stroke();
            break;

        }
        case curve_mode::quadratic_bezier:
        {
            if(controller.samples.size() < 3) return;

            const point p1(controller.samples[0].x * size.x + pos.x, controller.samples[0].y * size.y + pos.y);
            cnv.move_to(p1);
            for(size_t i = 0; i < controller.samples.size() - 2; i+=2) {
                const point p2(controller.samples[i + 1].x * size.x + pos.x, controller.samples[i + 1].y * size.y + pos.y);
                const point p3(controller.samples[i + 2].x * size.x + pos.x, controller.samples[i + 2].y * size.y + pos.y);
                cnv.quadratic_curve_to(p2, p3);
            }
            cnv.stroke();
            break;
        }
        default:
            break;
        }
    }

    void curve_editor::draw_samples(const context &ctx)
    {
        if(controller.samples.empty()) return;
        canvas& cnv = ctx.canvas;
        auto size = ctx.bounds.size();
        auto pos = ctx.bounds.top_left();

        cnv.fill_color(colors::red.opacity(0.5));

        for(int i = 0; i < int(controller.samples.size()); i++)
        {
            if(i == focused) {
               cnv.fill_color(colors::red.opacity(0.2));
               cnv.add_circle(controller.samples[i].x * size.x + pos.x, controller.samples[i].y * size.y + pos.y, 10);
               cnv.fill();
               cnv.fill_color(colors::red);
            }
            if(i == selected) cnv.fill_color(colors::salmon);
            else cnv.fill_color(colors::red.opacity(0.7));
            cnv.add_circle(controller.samples[i].x * size.x + pos.x, controller.samples[i].y * size.y + pos.y, 5);
            cnv.fill();
        }
    }

    void curve_editor::draw_grid(const context &ctx)
    {
        if(!controller.grid_enabled) return;
        canvas& cnv = ctx.canvas;
        auto size = ctx.bounds.size();
        auto pos = ctx.bounds.top_left();
        cnv.line_width(0.5);
        cnv.stroke_color(colors::ivory.opacity(0.3));
        for(size_t i = 0; i < static_cast<size_t>(controller.grid_steps + 1); i++)
        {
            const float y = (pos.y + (float(i / 10.0f) * size.y));
            point p1(pos.x, y);
            point p2(pos.x + size.x, y);
            path p(rect(p1, p2));
            cnv.add_path(p);
            cnv.stroke();


            const float x = (pos.x + (float(i / 10.0f) * size.x));
            p1 = point(x, pos.y);
            p2 = point(x, pos.y + size.y);
            p = path(rect(p1, p2)) ;
            cnv.add_path(p);

            cnv.stroke();
        }
    }


    void curve_editor::snap_to_grid(const context &ctx)
    {
        if(focused == -1) return;
        if(!controller.grid_enabled) return;

        float x_snap = -1.0f, y_snap = -1.0f;

        const float step =  1.0f / float(controller.grid_steps);
        const float step_d2 = step / 2.0f;
         for(size_t i = 0; i < static_cast<size_t>(controller.grid_steps + 1); i++) {
             const float relative = (float(i) / static_cast<float>(controller.grid_steps));

             const float x_dist = std::abs(controller.samples[focused].x - relative);
             if(x_dist < (step_d2)) x_snap = relative;

             const float y_dist = std::abs(controller.samples[focused].y - relative);
             if(y_dist < (step_d2)) y_snap = relative;

             if( (x_snap != -1.0f) && (y_snap != -1.0f) ) break;
         }

         controller.samples[focused].x = x_snap;
         controller.samples[focused].y = y_snap;
         if( size_t(focused) < controller.samples.size() - 1 && controller.samples[focused].x > controller.samples[focused + 1].x) {
             std::swap(controller.samples[focused], controller.samples[focused + 1]);
             focused += 1;
         }
         else if( focused > 0 && controller.samples[focused].x < controller.samples[focused - 1].x ) {
             std::swap(controller.samples[focused], controller.samples[focused - 1]);
             focused -= 1;
         }
         ctx.view.refresh();
    }


    point curve_editor::in_bounds(const context &ctx,  point &p)
    {
        auto pos = ctx.bounds.top_left();
        auto size = ctx.bounds.size();
        point pb(p);
        if(pb.x < pos.x ) pb.x = pos.x;
        else if(pb.x > (pos.x + size.x)) pb.x = pos.x + size.x;

        if(pb.y  < pos.y) pb.y = pos.y;
        else if(pb.y > (pos.y + size.y)) pb.y = pos.y + size.y;

        return pb;
    }

    bool curve_editor::click(const context &ctx, mouse_button btn)
    {
        if(btn.down) {
            const int found = find_sample(btn.pos, ctx);
            if(btn.num_clicks >= 2 && found != -1) { // double click (200 ms)
                snap_to_grid(ctx);
            }
            else { // simple click
                if(found == -1 && btn.modifiers == 0) { // no sample here
                    add_sample(btn.pos, ctx);
                    focused = selected; // add_sample selects the added sample
                } else { // sample here
                    if( (btn.modifiers == mod_alt || btn.modifiers == mod_shift) && found != -1 ) {
                        controller.samples.erase(controller.samples.begin() + found);
                        selected = -1;
                        focused = -1;
                    } else {
                        selected = found;
                    }
                }
            }
        } else { // btn up
            selected = -1;
        }
        ctx.view.refresh();
        return true;
    }

    void curve_editor::end_tracking(const context &, tracker_info &)
    {
    }

    void curve_editor::drag(const context &ctx, mouse_button btn)
    {
        auto size = ctx.bounds.size();
        auto pos = ctx.bounds.top_left();
        const point btn_pos = in_bounds(ctx, btn.pos);
        const point btn_pos_relative( (btn_pos.x - pos.x) / size.x, (btn_pos.y - pos.y) / size.y );
        if(selected != -1)  { // drag sample and swap if necessary
            controller.samples[selected].x = btn_pos_relative.x;
            controller.samples[selected].y = btn_pos_relative.y;
            if( size_t(selected) < controller.samples.size() - 1 && controller.samples[selected].x > controller.samples[selected + 1].x) {
                std::swap(controller.samples[selected], controller.samples[selected + 1]);
                selected +=1;
                focused += 1;
            }
            else if( selected > 0 && controller.samples[selected].x < controller.samples[selected - 1].x ) {
                std::swap(controller.samples[selected], controller.samples[selected - 1]);
                selected -=1;
                focused -= 1;
            }
            ctx.view.refresh();
        } else if(selected == -1 && (btn.modifiers == mod_alt || btn.modifiers == mod_shift)){ // find segment
            if( (btn_pos_relative.x < controller.samples.front().x) || (btn_pos_relative.x > controller.samples.back().x) ) return;
            for(size_t i = 0; i < controller.samples.size() - 1; i++)
            {
                   if( (btn_pos_relative.x > controller.samples[i].x) && (btn_pos_relative.x < controller.samples[i + 1].x))
                   {
                      controller.samples[i].curve += (last_position.y - btn_pos_relative.y);
                      if(controller.samples[i].curve > 10.0) controller.samples[i].curve = 10.0f;
                      else if(controller.samples[i].curve < -10.0) controller.samples[i].curve = -10.0f;
                      break;
                   }
            }

            if(controller.mode == curve_mode::log_exp) {
                ctx.view.refresh();
            }
        }
    }

    curve_editor_controller curve_editor::value() const
    {
        return controller;
    }

    void curve_editor::value(curve_editor_controller ctrl)
    {
        controller = ctrl;
    }

    bool curve_editor::scroll(context const&ctx, point dir, point p)
    {
        auto size = ctx.bounds.size();
        auto pos = ctx.bounds.top_left();
        const point relative((p.x - pos.x) / size.x, (p.y - pos.y) / size.y);
        for(size_t i = 0; i < controller.samples.size() - 1; i++)
        {
            if(relative.x > controller.samples[i].x && relative.x < controller.samples[i + 1].x) {
                controller.samples[i].curve += (dir.y / 10.0);
                ctx.view.refresh();
                return true;
            }
        }
        return true;
    }

    void curve_editor::draw(const context &ctx)
    {
        canvas& cnv = ctx.canvas;
        cnv.fill_color(color(0.1, 0.1, 0.1));
        cnv.add_rect(ctx.bounds);
        cnv.fill();
        this->draw_grid(ctx);
        this->draw_segments(ctx);
        this->draw_samples(ctx);
    }

    void curve_editor::layout(const context &ctx)
    {
    }

    bool curve_editor::cursor(context const &ctx, point p, cursor_tracking status)
    {
        const int found = find_sample(p, ctx);
        if(found != focused) {
            focused = found;
            ctx.view.refresh();
        }
        last_position = point( (p.x - ctx.bounds.top_left().x) / ctx.bounds.size().x,
                               (p.y - ctx.bounds.top_left().y) / ctx.bounds.size().y);
        return true;
    }

    bool curve_editor::key(const context &ctx, key_info k)
    {
        return false;
    }
