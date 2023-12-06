#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include "TimePoint.h"

class Screen {
    int windowSize;

public:
    Screen();
    Screen(int window);

    std::vector<TimePoint> local(std::vector<TimePoint>& errorDataList, double minimumSpeedConstraint, double maximumSpeedConstraint);
    std::vector<TimePoint> update(std::vector<TimePoint>& rectifiedDataList, double minimumSpeedConstraint, double maximumSpeedConstraint, int delayed_position, int firstFiniteDataPoints);
    std::vector<TimePoint> heuristic(std::vector<TimePoint>& rectifiedDataList, double minimumSpeedConstraint, double maximumSpeedConstraint, int delayed_position, int firstFiniteDataPoints);
};

#endif // SCREEN_H
