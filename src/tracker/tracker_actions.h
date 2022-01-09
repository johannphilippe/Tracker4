#ifndef TRACKER_ACTIONS_H
#define TRACKER_ACTIONS_H

#include<elements.hpp>
#include<portable-file-dialogs/portable-file-dialogs.h>
#include<tracker/jtracker_globals.h>
#include<tracker/jtracker.h>
#include<atomic>

using namespace cycfi::elements;

namespace jtracker {

// Avoid several file dialogs to open
extern std::atomic<bool> file_dialog_opened;

///////////////////////////////////////////////////////////////
// Actions
///////////////////////////////////////////////////////////////

void new_project_action();
void open_project_action();
void save_project_action();
void save_project_as_action();

}
#endif // TRACKER_ACTIONS_H
