#ifndef MATHS_H
#define MATHS_H

#include<math.h>
namespace jtracker
{

namespace maths
{

template<typename T>
T limit_number(T min, T max, T val)
{
    if(val >= max) return max;
    else if(val <= min) return min;
    else return val;
}


// Linear or exp interpolation
template<typename T>
inline T log_exp_point(T beg, T ending, int dur, int idx, T typ)
{
    T res;
    T type;
    if(typ == 0) {
        res = beg + (idx * (( ending - beg) / dur));
    }else {
        if(typ > 10) type = 10;
        else if(typ <  -10) type = -10;
        else type = typ;
        res = beg + (ending - beg) * (1 - std::exp(idx * type / (dur - 1))) / (1 - std::exp(type));
    }
    return res;
}

template<typename T>
inline T scaled_log_point(T val, T min, T max) {
    return ((std::log(val) - std::log(min)) / (std::log(max) - std::log(min))) * max;
}


inline double Hanning(int index, int length) {
    return  0.5 * (1 - cos(2 * M_PI * index / (length - 1 )));
}

inline double Hamming(int index, int length) {
    return 0.54 - (0.46 * cos(2 * M_PI * index / (length - 1)));
}

inline double Blackman(int index, int length) {
    return 0.42 - (0.5 * cos(2 * M_PI * index / (length - 1))) + (0.08 * cos(4 * M_PI * index / (length - 1)));
}

inline double BarkBand(int freq)
{
    return (13 * atan(0.00076 * double(freq) )) + (3.5 * atan(pow( double(freq) / 7500, 2)));
}


// Be careful, it is cutoff frequencies. The band 1, ends in cutoff 2. So the index 1 corresponds to start index 0, and width 1.
static constexpr int BarkBandsCutoffFrequencies[] = {
    20, 100,200,300,400,510,630, 770, 920, 1080, 1270, 1480, 1720, 2000, 2320, 2700, 3150, 3700, 4400, 5300, 6400, 7700, 9500, 12000, 15500
};

static constexpr int BarkBandWidths[] = {
  -1, 80, 100, 100, 100, 110, 120, 140, 150, 160, 190, 210, 240, 280, 320, 380, 450, 550, 700, 900, 1100, 1300, 1800, 2500, 3500
};


static int BarkFrequency(double band)
{
    int band_nbr = int(floor(band));
    if(band_nbr == 0) {
        throw("Error, bark band cannot be 0");
    }
    int begin = BarkBandsCutoffFrequencies[band_nbr - 1];
    int width = BarkBandWidths[band_nbr];
    double decimal = band - band_nbr;
    double freq = begin + (decimal * double(width));

    return freq;
}



}

}

#endif // MATHS_H
