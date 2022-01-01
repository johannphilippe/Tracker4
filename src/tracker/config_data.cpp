#include<tracker/config_data.h>

audio_cs_option::audio_cs_option(std::string opt, bool enable)
    : option(opt)
    , enabled(enable)
{}

audio_cs_option::audio_cs_option(json &j)
{
    option = j["option"].get<std::string>();
    enabled = j["enabled"].get<bool>();
}


json audio_cs_option::to_json()
{
    json j;
    j["option"] = option;
    j["enabled"] = enabled;
    return j;
}

audio_cs_option audio_cs_option::from_json(json &j) {return audio_cs_option(j);}

tracker_audio_config_data::tracker_audio_config_data() {}
tracker_audio_config_data::tracker_audio_config_data(json &j)
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
    if(j.contains("additional_options")
        && j["additional_options"].is_array()
        && (j["additional_options"].size() > 0))
    {
        for(auto it = j["additional_options"].begin(); it != j["additional_options"].end(); ++it)
        {
            additional_options.push_back(audio_cs_option(it.value()));
        }
    }
}

tracker_audio_config_data tracker_audio_config_data::from_json(json &j)
{
    return tracker_audio_config_data(j);
}

json tracker_audio_config_data::to_json()
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



