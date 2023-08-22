#ifndef TIMER_HPPA
#define TIMER_HPPA

// === StopWatch ===
class StopWatch {
private:
    double stored_time;
    int    counter;

public:
    StopWatch();

// --- Time ---
    double delta(double current_time);
    bool   check(double current_time, double min_interval);
    void   click(double current_time);

// --- Rate Count ---
    double rate(double current_time);

// --- Private ---
private:
    void setTime(double current_time);
    void resetCount();
    void stepCount();
    int  getCount();
};

#endif