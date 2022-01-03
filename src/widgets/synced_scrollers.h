#ifndef SYNCED_SCROLLERS_H
#define SYNCED_SCROLLERS_H
#include<elements.hpp>
using namespace cycfi::elements;


template<typename SubjectOne, typename SubjectTwo>
inline std::pair< proxy< cycfi::remove_cvref_t<SubjectOne>, scroller_base>,
                  proxy< cycfi::remove_cvref_t<SubjectTwo>, scroller_base>>
sync_vscrollers(SubjectOne &&sub1, SubjectTwo &&sub2)
{
    auto v1 = vscroller(sub1);
    auto v2 = vscroller(sub2);

    v1.on_scroll = [&](point p)
    {
        v2.set_position(p);
    };

    v2.on_scroll = [&](point p)
    {
      v1.set_position(p);
    };

    return {v1, v2};
}



#endif // SYNCED_SCROLLERS_H
