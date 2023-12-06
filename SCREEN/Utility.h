#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "TimePoint.h"

// Utility class that provides various utility functions
class Utility {
public:
    // Path to the data files
    static const std::string PATH;

    // Function to read data from a file and return a vector of TimePoint objects
    std::vector<TimePoint> readData(const std::string& filename, const std::string& filePath, int index, const std::string& splitOp) {
        // Check if filename or filePath is empty
        if (filename.empty() || filePath.empty())
            throw std::invalid_argument("Invalid File name or Invalid File Path !!");

        // Vector to hold the data points
        std::vector<TimePoint> list;

        // Open the data file
        std::ifstream datafile(filePath + filename);
        std::string line;

        // Check if the file was opened successfully
        if (!datafile.is_open()) {
            throw std::runtime_error("File not found: " + filePath + filename);
        }

        // Read the file line by line
        while (getline(datafile, line)) {
            std::istringstream iss(line);
            std::string token;
            std::vector<std::string> dataPoints;

            // Split the line into tokens
            while (getline(iss, token, splitOp.empty() ? ',' : splitOp[0])) {
                dataPoints.push_back(token);
            }

            // Create a TimePoint object and add it to the list
            TimePoint dataPoint(std::stol(dataPoints[0]), std::stod(dataPoints[index]));
            list.push_back(dataPoint);
        }

        // Return the list of data points
        return list;
    }

    // Function to calculate the root mean square error between two series of data points
    double calculate_RMS(const std::vector<TimePoint>& truthSeries, const std::vector<TimePoint>& resultSeries) {
        double cost = 0;
        double delta;
        int len = truthSeries.size();

        // Calculate the sum of the squares of the differences
        for (int index = 0; index < len; ++index) {
            delta = resultSeries[index].constraint - truthSeries[index].value;
            cost += delta * delta;
        }

        // Calculate the mean
        cost /= len;

        // Return the square root of the mean
        return std::sqrt(cost);
    }

    // Function to write a series of data points to a file
    void writeData(const std::string& filename, const std::vector<TimePoint>& data) {
        // Open the output file
        std::ofstream outFile(filename);

        // Check if the file was opened successfully
        if (!outFile.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        // Write the data points to the file
        for (const auto& point : data) {
            outFile << point.time << "," << point.value << "," << point.constraint << std::endl;
        }
    }
};

// Initialize the PATH constant
const std::string Utility::PATH = "data/";

#endif // UTILITY_H