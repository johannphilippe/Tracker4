#include "tracker_actions.h"

namespace jtracker {

std::atomic<bool> file_dialog_opened(false);

void new_project_action()
{

}

void open_project_action()
{
    if(file_dialog_opened) return;
    auto async = [&]()
    {
        file_dialog_opened = true;
        std::cout << "open project " << std::endl;
        auto f = pfd::open_file("Select a project to load", jtracker::data.filesystem.home_directory, {"Tracker Project files .json, .jt", "*.json *.txt *.jo"} );

        std::cout << "selected files : \n";
        for(auto const &name : f.result())
        std::cout << name << std::endl;
        file_dialog_opened = false;
    };
    std::thread t(async);
    t.detach();
}

}
