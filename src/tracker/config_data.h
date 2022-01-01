#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

#include<iostream>
#include<vector>
#include<nlohmann/json.hpp>

using json = nlohmann::json;

constexpr static const size_t audio_rate_list[] = {22'050,
                                                   24'000,
                                                   44'100,
                                                   48'000,
                                                   88'200,
                                                   96'000,
                                                   176'400,
                                                   192'000};

constexpr static const size_t audio_rate_list_size = sizeof(audio_rate_list)  /sizeof(size_t);

struct audio_cs_option
{
    audio_cs_option(std::string opt, bool enable = true);
    audio_cs_option(json &j);

    static audio_cs_option from_json(json &j);
    json to_json();

    std::string option;
    bool enabled = true;
};

class tracker_audio_config_data
{
public:
    tracker_audio_config_data();
    tracker_audio_config_data(json &j);
    static tracker_audio_config_data from_json(json &j);
    json to_json();

    size_t sample_rate = 48'000;
    size_t ksmps = 16;

    size_t i_nchnls = 0;
    size_t nchnls = 2;

    size_t dbfs = 1;

    std::vector<audio_cs_option> additional_options;

};

//
class tracker_config_data
{



};


#endif // CONFIG_DATA_H
