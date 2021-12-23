#ifndef MATHS_H
#define MATHS_H

namespace jtracker
{

namespace maths
{

template<typename T>
T limit_number(T min, T max, T val)
{
    if(val > max) return max;
    else if(val < min) return min;
    else return val;
}

}

}

#endif // MATHS_H
