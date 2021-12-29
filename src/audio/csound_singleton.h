#ifndef CSOUND_SINGLETON_H
#define CSOUND_SINGLETON_H

#include<csound/csound_threaded.hpp>

class csound_singleton : public CsoundThreaded
{
public:
    static csound_singleton* get_instance();
private:
    csound_singleton();
    csound_singleton& operator= (const csound_singleton&) = delete;
    csound_singleton (const csound_singleton&) = delete;

    static csound_singleton *instance;
};

#endif // CSOUND_SINGLETON_H
