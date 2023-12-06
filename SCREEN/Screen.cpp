#include "Screen.h"
#include <algorithm> // For std::sort
#include <vector>
#include <iostream>

Screen::Screen() : windowSize(0) {}

Screen::Screen(int window) : windowSize(window) {}


std::vector<TimePoint> Screen::local(std::vector<TimePoint>& errorDataList, double minimumSpeedConstraint, double maximumSpeedConstraint) {
    double minimumOfBounds, maximumOfBounds;

    for (size_t dataPoint = 0; dataPoint < errorDataList.size(); ++dataPoint) {
        if (dataPoint == 0) {
            minimumOfBounds = maximumOfBounds = errorDataList[dataPoint].value;
        } else {
            minimumOfBounds = errorDataList[dataPoint - 1].constraint + minimumSpeedConstraint * (errorDataList[dataPoint].time - errorDataList[dataPoint - 1].time);
            maximumOfBounds = errorDataList[dataPoint - 1].constraint + maximumSpeedConstraint * (errorDataList[dataPoint].time - errorDataList[dataPoint - 1].time);
        }

        if (dataPoint < errorDataList.size() + windowSize - 1) {
            std::vector<double> midValues;
            for (size_t i = dataPoint + 1; i < errorDataList.size(); ++i) {
                if (errorDataList[i].time > errorDataList[dataPoint].time + windowSize) {
                    break;
                }
                midValues.push_back(errorDataList[i].value + minimumSpeedConstraint * (errorDataList[dataPoint].time - errorDataList[i].time));
                midValues.push_back(errorDataList[i].value + maximumSpeedConstraint * (errorDataList[dataPoint].time - errorDataList[i].time));
            }
            midValues.push_back(errorDataList[dataPoint].value);
            std::sort(midValues.begin(), midValues.end());

            double median = midValues[midValues.size() / 2];
            if (maximumOfBounds < median) {
                errorDataList[dataPoint].constraint = maximumOfBounds;
            } else if (minimumOfBounds > median) {
                errorDataList[dataPoint].constraint = minimumOfBounds;
            } else {
                errorDataList[dataPoint].constraint = median;
            }
        } else {
            errorDataList[dataPoint].constraint = errorDataList[dataPoint].value;
        }
    }

    return errorDataList;
}

std::vector<TimePoint> Screen::update(std::vector<TimePoint>& rectifiedDataList, double minimumSpeedConstraint, double maximumSpeedConstraint, int delayed_position, int firstFiniteDataPoints) {
    int i;
    for (i = delayed_position - 1; i >= 0; i--) {
        std::vector<double> midValues;
        if (rectifiedDataList[i].time < rectifiedDataList[delayed_position].time - windowSize) {
            break;
        }
        double cond = (rectifiedDataList[i].constraint - rectifiedDataList[delayed_position].value) / (rectifiedDataList[i].time - rectifiedDataList[delayed_position].time);
        if (cond < minimumSpeedConstraint || cond > maximumSpeedConstraint) {
            midValues.push_back(rectifiedDataList[delayed_position].value + minimumSpeedConstraint * (rectifiedDataList[i].time - rectifiedDataList[delayed_position].time));
            midValues.push_back(rectifiedDataList[delayed_position].value + maximumSpeedConstraint * (rectifiedDataList[i].time - rectifiedDataList[delayed_position].time));
            midValues.push_back(rectifiedDataList[i].value);
            std::sort(midValues.begin(), midValues.end());
            rectifiedDataList[i].constraint = midValues[midValues.size() / 2];
        }
    }

    int j;

    for ( j = i + 1; j <= firstFiniteDataPoints; j++) {
        double minimumOfBounds = rectifiedDataList[j - 1].constraint + minimumSpeedConstraint * (rectifiedDataList[j].time - rectifiedDataList[j - 1].time);
        double maximumOfBounds = rectifiedDataList[j - 1].constraint + maximumSpeedConstraint * (rectifiedDataList[j].time - rectifiedDataList[j - 1].time);
        if (minimumOfBounds <= maximumOfBounds) {
            double median = rectifiedDataList[j].value;
            if (maximumOfBounds < median) {
                rectifiedDataList[delayed_position].constraint = maximumOfBounds;
            } else if (minimumOfBounds > median) {
                rectifiedDataList[delayed_position].constraint = minimumOfBounds;
            } else {
                rectifiedDataList[delayed_position].constraint = median;
            }
        }
    }

    return rectifiedDataList;
}

std::vector<TimePoint> Screen::heuristic(std::vector<TimePoint>& rectifiedDataList, double minimumSpeedConstraint, double maximumSpeedConstraint, int delayed_position, int firstFiniteDataPoints) {
    double minimumOfBounds = std::max(rectifiedDataList[delayed_position - 1].constraint + minimumSpeedConstraint * (rectifiedDataList[delayed_position].time - rectifiedDataList[delayed_position - 1].time),
                          rectifiedDataList[delayed_position + 1].constraint + maximumSpeedConstraint * (rectifiedDataList[delayed_position].time - rectifiedDataList[delayed_position + 1].time));
    double maximumOfBounds = std::min(rectifiedDataList[delayed_position - 1].constraint + maximumSpeedConstraint * (rectifiedDataList[delayed_position].time - rectifiedDataList[delayed_position - 1].time),
                          rectifiedDataList[delayed_position + 1].constraint + minimumSpeedConstraint * (rectifiedDataList[delayed_position].time - rectifiedDataList[delayed_position + 1].time));

    if (minimumOfBounds <= maximumOfBounds) {
        double median = rectifiedDataList[delayed_position].value;
        if (maximumOfBounds < median) {
            rectifiedDataList[delayed_position].constraint = maximumOfBounds;
        } else if (minimumOfBounds > median) {
            rectifiedDataList[delayed_position].constraint = minimumOfBounds;
        } else {
            rectifiedDataList[delayed_position].constraint = median;
        }
    } else {
        // std::cout << " It calls the update method (Algorithm - 2)" << std::endl;
        return update(rectifiedDataList, minimumSpeedConstraint, maximumSpeedConstraint, delayed_position, firstFiniteDataPoints);
    }

    return rectifiedDataList;
}
