#ifndef SNIPER_TRACK_BOX_H
#define SNIPER_TRACK_BOX_H

#include<elements.hpp>
#include<tracker/jtracker_globals.h>
#include<tracker/jtracker_theme.h>
#include<widgets/custom_labels.h>
#include<widgets/sniper_track_content.h>
#include<iostream>
#include<widgets/sniper_event_editor.h>
using namespace cycfi::elements;

class line_number_labels : public vdynamic_list
{
public:
    auto make_label (size_t i);
    line_number_labels();
};

class sniper_track_box : public htile_composite
{
public:

  sniper_track_box();

  void update_labels();
  void update_lines();

  void show_editor(bool b, sniper_track_event& event = sniper_null_event);

  line_number_labels labels;
  sniper_track_content sn_content;

  std::shared_ptr<sniper_event_editor> editor;

private:
  bool editor_displayed = false;

};

#endif // SNIPER_TRACK_BOX_H
