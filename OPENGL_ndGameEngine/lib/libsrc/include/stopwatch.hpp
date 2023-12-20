#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

/* CLASS StopWatch

A StopWatch is used to measure time intervals and measure 
the rate in which an operation is performed.

The StopWatch contains a stored time and a counter. When the 
StopWatch is clicked, the stored time is updated and the counter
is reset. At any point a user can get the delta and meausre the
current time to the stored time. The clock and be checked to see
if a certain time interval has passed. Each time the StopWatch is
checked the count is incremented.

======== ATTRIBUTES ========
- double stored_time
- int    counter

======== METHODS ========
-------- CONSTRUCTOR --------
- StopWatch : Sets stored_time and counter to 0.

-------- CONTROLS --------
- check : Returns true if the time elapsed is at least as long 
    as the minimum interval. Each time this is called, count 
    is incremented.
    ==Parameters==
        - double current_time
        - double min_interval
    ==Return==
        - bool : true if the time elapsed is at least the
            minimum interval

- click : Sets time to current time and resets counter to 0.
    ==Parameter==
        - double current_time

-------- GETS --------
- delta : Returns the difference of the current time and the time stored.
    ==Parameter==
        - double current_time
    ==Return==
        - double : difference of the current time and the time stored.

- rate : Returns the rate at which the clocked is checked per second since
    last click.
    ==Parameter==
        - double current_time
    ==Return==
        - double : the value (coutner / delta)

-------- PRIVATE --------
- setTime : Sets the stored time to the given time
    ==Parameter==
        - double current_time

- resetCount : resets counter to 0

- stepCount : increments counter by 1 

*/

class StopWatch {
// --- Attributes ---
private:
    double stored_time;
    int    counter;

// --- Constructor ---
public:
    StopWatch();

// --- Controls ---
public:
    bool check(double current_time, double min_interval);
    void click(double current_time);

// --- Gets ---
public:
    double delta(double current_time);
    double rate(double current_time);

// --- Private ---
private:
    void setTime(double current_time);
    void resetCount();
    void stepCount();
};

#endif