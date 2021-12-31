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
    auto make_label (size_t i)
    {
        const color c = (jtracker::data.grid_step > 0 && ((i % jtracker::data.grid_step) == 0) ) ?
                    jtracker::theme.track_label_index_hot_color :
                    jtracker::theme.track_label_index_color;
        return std::make_shared<fixed_size_label_with_background<4>>(std::to_string(i), c ) ;
        //return make_fixed_size_label_with_background<4>(std::to_string(i), c);
    };

    line_number_labels() : vdynamic_list(
                                       basic_vertical_cell_composer(
                                   jtracker::data.number_of_lines,
                                   [&](size_t i) {return make_label(i);}))
    {}
};

class sniper_track_box : public htile_composite
{
public:

  sniper_track_box() : htile_composite()
  {
      push_back(share(link(labels)));
      push_back(share(link(sn_content)));
      sn_content.on_event = [&](bool b, sniper_track_event& event) {show_editor(b, event);};
  }

  void update_labels()
  {
      labels.resize(jtracker::data.number_of_lines);
  }

  void update_lines()
  {
      update_labels();
  }


  line_number_labels labels;
  sniper_track_content sn_content;
  void show_editor(bool b, sniper_track_event& event = sniper_null_event)
  {
      if(!b)
      {
          if(editor_displayed) pop_back();
          editor_displayed = false;
          return;
      }
      if(editor_displayed)
      {
          pop_back();
      }

      editor = std::make_shared<sniper_event_editor>(event);
      push_back(editor);
      editor_displayed = true;
      jtracker::get_app()->_view.refresh(*this);
  }

  std::shared_ptr<sniper_event_editor> editor;

private:
  bool editor_displayed = false;

};

#endif // SNIPER_TRACK_BOX_H
