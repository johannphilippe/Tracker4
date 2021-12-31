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
    audio_cs_option(std::string opt, bool enable = true)
        : option(opt)
        , enabled(enable)
    {
    }

    audio_cs_option(json &j)
    {
        option = j["option"].get<std::string>();
        enabled = j["enabled"].get<bool>();
    }

    std::string option;
    bool enabled = true;

    json to_json()
    {
        json j;
        j["option"] = option;
        j["enabled"] = enabled;
        return j;
    }

    static audio_cs_option from_json(json &j) {return audio_cs_option(j);}


};

class tracker_audio_config_data
{
public:
    tracker_audio_config_data() {}
    tracker_audio_config_data(json &j)
    {
        assert(j.is_object());
        if(j.contains("sample_rate"))
            sample_rate = j["sample_rate"].get<size_t>();
        if(j.contains("ksmps"))
            ksmps = j["ksmps"].get<size_t>();
        if(j.contains("i_nchnls"))
            i_nchnls = j["i_nchnls"].get<size_t>();
        if(j.contains("nchnls"))
            nchnls = j["nchnls"].get<size_t>();
        if(j.contains("dbfs"))
            dbfs = j["dbfs"].get<size_t>();
        if(j.contains("additional_options") && j["additional_options"].is_array() && (j["additional_options"].size() > 0))
        {
            for(auto it = j["additional_options"].begin(); it != j["additional_options"].end(); ++it)
            {
                additional_options.push_back(audio_cs_option(it.value()));
            }
        }
    }
    static tracker_audio_config_data from_json(json &j) {return tracker_audio_config_data(j);}

    json to_json()
    {
        json j;
        j["sample_rate"] = sample_rate;
        j["ksmps"] = ksmps;
        j["i_nchnls"] = i_nchnls;
        j["nchnls"] = nchnls;
        j["dbfs"] = dbfs;
        if(additional_options.size()) {
            j["additional_options"] = json::array();
            for(auto & it : additional_options)
            {
                j["additional_options"].push_back(it.to_json());
            }
        }
        return j;
    }


    void print_data()
    {
        std::cout << "audio config : " <<
                     sample_rate << "\n" <<
                     ksmps << "\n" <<
                     dbfs << "\n" <<
                     i_nchnls << "\n" <<
                     nchnls << std::endl;
    }

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
