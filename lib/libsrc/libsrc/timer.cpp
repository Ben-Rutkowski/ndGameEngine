#include "timer.hpp"

// === StopWatch ===
StopWatch::StopWatch()
    :stored_time{ 0.0 },
    counter{ 0 } {}

// === Time ===
double StopWatch::delta(double current_time) { return current_time - stored_time; }

bool StopWatch::check(double current_time, double min_interval) {
    stepCount();
    return delta(current_time) >= min_interval;
}

void StopWatch::click(double current_time) {
    setTime(current_time);
    resetCount();
}

// === Rate Count ===
double StopWatch::rate(double current_time) {
    return (double)counter / delta(current_time);
}

// === Private ===
void   StopWatch::setTime(double current_time)  { stored_time = current_time; }

void StopWatch::resetCount() { counter = 0; }
void StopWatch::stepCount()  { counter += 1; }
int  StopWatch::getCount()   { return counter; }