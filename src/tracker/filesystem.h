#ifndef TRACKER_FILESYSTEM_H
#define TRACKER_FILESYSTEM_H

#include<filesystem>
#include<elements.hpp>
#include<string>
namespace fs = std::filesystem;
/////////////////////////////////////////////////////////////////////////////
/// This file contains information and utilities on tracker filesystem
/// - Data is stored in home local directory
/// -
/////////////////////////////////////////////////////////////////////////////

#if defined(ELEMENTS_HOST_UI_LIBRARY_COCOA)
constexpr static const char* TRACKER_DATA_DIR = "Library/Application Support";
#elif defined(ELEMENTS_HOST_UI_LIBRARY_GTK)
constexpr static const char* TRACKER_DATA_DIR = ".local/share";
#elif defined(ELEMENTS_HOST_UI_LIBRARY_WIN32)
constexpr static const char* TRACKER_DATA_DIR = "AppData/Local";
#endif

// verify the presence of backslash, and convert it to slash
static std::string to_unix_path(std::string winpath) {
    std::string res=winpath;
    for(std::size_t i =0; i< res.size(); i++) {
        if(res[i]== '\\') res[i] = '/';
    }
    return res;
}

static std::string get_home_path() {
    std::string home("");
#ifdef _WIN32
    std::string temp = std::string(std::getenv("HOMEDRIVE")) + std::string(std::getenv("HOMEPATH"));
    home = to_unix_path(temp);
#elif __linux__
   home = std::getenv("HOME") ? std::getenv("HOME") : ".";
#endif
    return home;
}

static std::string get_ennvironnement_variable(const char *var) {
    std::string ENV = std::getenv(var) ? std::getenv(var) : ".";
    return ENV;
}

class tracker_filesystem
{
public:
    tracker_filesystem()
        : home_directory(get_home_path())
        , tracker_dir( fs::path(home_directory).append(TRACKER_DATA_DIR).append(TRACKER_APP_NAME))
        , tracker_res( fs::path(tracker_dir).append("res"))
        , tracker_config(fs::path(tracker_dir).append("config"))
        , tracker_project(fs::path(tracker_dir).append("project"))
        , tracker_audio_config(fs::path(tracker_config).append("audio_config.json"))
        , tracker_general_config(fs::path(tracker_config).append("general_config.json"))
        , tracker_score_data(fs::path(tracker_project).append("score_data.json"))
        , tracker_orc_data(fs::path(tracker_project).append("orc_data.json"))
    {
        std::cout << "Home Directory : " << home_directory << std::endl
                  << "Tracker Dir : "  << tracker_dir << std::endl
                  <<"Tracker resources : "<<  tracker_res << std::endl
                 << "Tracker config :" << tracker_config << std::endl
                 << "Tracker project : " << tracker_project << std::endl;
        fs::create_directories(home_directory);
        fs::create_directories(tracker_dir);
        fs::create_directories(tracker_res);
        fs::create_directories(tracker_config);
        fs::create_directories(tracker_project);

    }

    // user home directory
    const fs::path home_directory;
    // Tracker root data dir
    const fs::path tracker_dir;
    // Resources
    const fs::path tracker_res;
    // Configuration files (params etc)
    const fs::path tracker_config;
    // Data of current projet
    const fs::path tracker_project;

    // Tracker files
    const fs::path tracker_audio_config;
    const fs::path tracker_general_config;
    const fs::path tracker_score_data;
    const fs::path tracker_orc_data;
};



#endif // TRACKER_FILESYSTEM_H
