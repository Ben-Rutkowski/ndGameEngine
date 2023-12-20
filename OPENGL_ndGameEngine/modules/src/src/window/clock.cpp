#include "window.hpp"

// === Time === 
double Clock::delta(Watch type) {
    StopWatch& watch = getWatch(type);
    return watch.delta(getTime());
}

bool Clock::check(Watch type, double min_interval) {
    StopWatch& watch = getWatch(type);
    return watch.check(getTime(), min_interval);
}

void Clock::click(Watch type) {
    StopWatch& watch = getWatch(type);
    watch.click(getTime());
}

// === Rate ===
double Clock::rate(Watch type) {
    StopWatch& watch = getWatch(type);
    return watch.rate(getTime());
}

// === Private ===
double Clock::getTime() { return glfwGetTime(); }
StopWatch& Clock::getWatch(Watch type) { return watches[(int)type]; }