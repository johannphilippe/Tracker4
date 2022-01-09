#ifndef CURVE_H
#define CURVE_H

#include<models/curve_point.h>
#include<vector>
#include<nlohmann/json.hpp>

using json = nlohmann::json;

enum class curve_mode {
        linear = 0,
        log_exp = 1,
        quadratic_bezier = 2,
        cubic_bezier = 3,
        cubic_spline = 4,
};

// This only contains the samples curve_point
struct curve : public std::vector<curve_point>
{
    curve()
    {}

    curve(json &j)
    {
        if(!j.contains("mode") || !j.contains("data") || !j["data"].is_array())
            throw(std::runtime_error("JSON document is not a valid curve object"));
        mode = static_cast<curve_mode>(j["mode"].get<int>());

        for(json::iterator it = j["data"].begin(); it != j["data"].end(); ++it)
        {
            push_back(curve_point::from_json(*it));
        }
    }
    static curve from_json(json j) {return curve(j);}

    json to_json()
    {
        json j;
        j["mode"] = static_cast<int>(mode);
        j["data"] = json::array();
        for(size_t i = 0; i < size(); i++)
            j["data"].push_back(data()[i].to_json());

        return j;
    }

    curve_mode mode = curve_mode::linear;
};

#endif // CURVE_H
