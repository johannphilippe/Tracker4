#ifndef CHRONO_H
#define CHRONO_H
#include<chrono>
#include<iostream>

using namespace std;
/*
 *  My wonderful simple chrono
 *
*/
template<typename T, typename D>
class JChrono {

public:
    JChrono() {

    }

    void start() {
        begin = chrono::high_resolution_clock::now();
        trigger();
    }

    T timeSinceLastTrigger() {
        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
        T dur = chrono::duration_cast<D>(t2 - t).count();
        trigger();
        return dur;
    }

    T timeSinceStart() {
        chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
        T dur = chrono::duration_cast<D>(t2 - begin).count();
        return dur;
    }

    void trigger() {
        t = chrono::high_resolution_clock::now();
    }

private:
    chrono::high_resolution_clock::time_point t, begin;

};


#endif // CHRONO_H
