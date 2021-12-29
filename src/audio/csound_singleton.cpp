#include "csound_singleton.h"

csound_singleton* csound_singleton::get_instance()
{
    if(csound_singleton::instance == nullptr)
    {
        csound_singleton::instance = new csound_singleton();
    }
    return csound_singleton::instance;
}

csound_singleton::csound_singleton() : CsoundThreaded()
{
}

csound_singleton* csound_singleton::instance = nullptr;
