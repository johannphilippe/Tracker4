#ifndef CURVE_H
#define CURVE_H

#include<models/curve_point.h>
#include<vector>
#include<nlohmann/json.hpp>

using json = nlohmann::json;

struct curve : public std::vector<curve_point>
{
    curve()
    {}

    curve(json &j)
    {
        if(!j.is_array())
            throw("JSON document must be an array to create a curve object");
        for(json::iterator it = j.begin(); it != j.end(); ++it)
        {
            push_back(curve_point::from_json(*it));
        }
    }

    json to_json()
    {
        json j;
        for(size_t i = 0; i < size(); i++)
            j.push_back(data()[i].to_json());
        return j;
    }

    static curve from_json(json j) {return curve(j);}
};

#endif // CURVE_H
