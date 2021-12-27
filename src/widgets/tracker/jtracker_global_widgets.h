#ifndef JTRACKER_GLOBAL_WIDGETS_H
#define JTRACKER_GLOBAL_WIDGETS_H

#include<tracker/jtracker_globals.h>
#include<elements.hpp>
using namespace cycfi::elements;

namespace jtracker {


class sniper_labels_list : public vdynamic_list
{
public:
    element_ptr make_label(size_t i);
    sniper_labels_list();
};

//extern std::shared_ptr<sniper_labels_list> sniper_labels;

}


#endif // JTRACKER_GLOBAL_WIDGETS_H
