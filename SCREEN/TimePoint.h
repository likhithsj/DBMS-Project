#ifndef TIMEPOINT_H
#define TIMEPOINT_H

class TimePoint {
public:
    long time;        // First declared
    double value;     // Second declared
    double constraint; // Third declared
    bool isModified;  // Fourth declared

    TimePoint(long time, double constraint) 
        : time(time), value(constraint), constraint(constraint), isModified(false) {}
        
};

#endif // TIMEPOINT_H
