/*=============================================================================
   Copyright (c) 2021 Johann Philippe
   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/

#ifndef CURVE_POINT_H
#define CURVE_POINT_H

#include<elements.hpp>
#include<nlohmann/json.hpp>

using namespace cycfi::elements;
using json = nlohmann::json;

struct curve_point : public point
{
    curve_point(float x_, float y_, float curve_ = 0.0f) :
        point(x_, y_), curve(curve_)
    {}

    curve_point(point &p, float curve_ = 0.0f) :
        point(p), curve(curve_)
    {}

    curve_point(json &j)
    {
        if(!j.contains("x") || !j.contains("y"))
            throw("Curve point cannot be parsed from JSON");
        x = j["x"].get<float>();
        y = j["y"].get<float>();

        if(j.contains("curve"))
            curve = j["curve"].get<float>();
        else
            curve = 0.0f;
    }

    template<typename T>
    float distance_to(T &p)
    {
        const float a2 = (p.x - x) * (p.x - x);
        const float b2 = (p.y - y) * (p.y - y);
        const float dist = sqrt(a2 + b2);
        return dist;
    }

    json to_json()
    {
        json j;
        j["x"] = x;
        j["y"] = y;
        j["curve"] = curve;
        return j;
    }

    static curve_point from_json(json &j) { return curve_point(j); }

   float curve;
};

#endif // CURVE_POINT_H
