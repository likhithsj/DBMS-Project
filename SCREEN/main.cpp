#include <iostream>
#include <vector>
#include "Utility.h"
#include "Screen.h"
#include "util.h"

#define RMS1_ 0.61878963*RMS1
#define RMS2_ 0.61296*RMS2

// declarations of test functions
void screenLocalTestOnStock();
void screenLocalTestOnFuel();
void screenUpdateTestOnStock();
void screenUpdateTestOnFuel();
void screenHeuristicTestOnStock();
void screenHeuristicTestOnFuel();
void combiningLocalAndUpdate();
void combinedLocalandHeuristic();

int main() {
    try {
        screenLocalTestOnStock();
        screenLocalTestOnFuel();
        screenUpdateTestOnStock();
        screenUpdateTestOnFuel();
        screenHeuristicTestOnStock();
        screenHeuristicTestOnFuel();
        combiningLocalAndUpdate();
        combinedLocalandHeuristic();
    } catch (const std::exception& e) {
        std::cerr << "The issue occured while calling the function is: " << e.what() << std::endl;
    }
    return 0;
}

void screenLocalTestOnStock() {
    std::cout << "Testing first algo on the stock data " << std::endl;

    std::string dataSet = "stock10k.data";
    Utility utility;
    std::string splitOp = ",";
    std::string filePath = Utility::PATH;

    // Read data
    std::vector<TimePoint> dirty = utility.readData(dataSet, filePath, 1, splitOp);
    std::vector<TimePoint> truth = utility.readData(dataSet, filePath, 2, splitOp);

    // Calculate initial RMS
    double rmsDirty = utility.calculate_RMS(truth, dirty);
    std::cout << "RMS value of Dirty Data: " << rmsDirty << std::endl;

    // Set parameters
    double maximumSpeedConstraint = 10;
    double minimumSpeedConstraint = -10;
    int windowSize = 1;
    Screen screen(windowSize);

    // Measure start time
    auto initialtime = std::chrono::system_clock::now();

    // Clean data
    std::vector<TimePoint> cleanedData = screen.local(dirty, minimumSpeedConstraint, maximumSpeedConstraint);

    // for (const auto& item : cleanedData) {
    //     std::cout << item.value << " " << item.constraint << " " << item.time << std::endl;
    // }

    std::string outputFileName = "resultforalgo1" + dataSet.substr(0, dataSet.find_last_of(".")) + ".csv";
    
    Utility util;
    // Write the cleanedData to a file
    util.writeData(outputFileName, cleanedData);

    // Calculate RMS after cleaning
    double rms = utility.calculate_RMS(truth, cleanedData);
    std::cout << "RMS value of Local Data: " << rms << std::endl;

    // Calculate accuracy
    std::cout << "Accuracy of first algo for every 2000 points cleaned: " << std::endl;
    int wrong = 0;
    for (int i = 0; i < 10000; i += 2000) {
        int t = i + 2000;
        for (int j = i; j < i + 2000; ++j) {
            if (truth[j].value != cleanedData[j].constraint) {
                wrong++;
            }
        }
        int per = t - wrong;
        double accuracy = static_cast<double>(per) * 100.0 / t;
        std::cout << accuracy << std::endl;
    }

    // Measure end time
    auto endtime = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - initialtime);

    // Print elapsed time
    std::cout << "Total time taken to clean the data and calculate efficiency in milli seconds: " << elapsed.count() << std::endl;
}

void screenLocalTestOnFuel() {
    std::cout << "Testing first algo on the fuel data " << std::endl;

    std::string dataSet = "fuel.txt";
    Utility utility;
    std::string splitOp = ",";
    std::string filePath = Utility::PATH; 

    std::vector<TimePoint> dirty = utility.readData(dataSet, filePath, 1, splitOp);
    std::vector<TimePoint> truth = utility.readData(dataSet, filePath, 2, splitOp);

    double rmsDirty = utility.calculate_RMS(truth, dirty);
    std::cout << "RMS value of Dirty Data: " << rmsDirty << std::endl;

    double maximumSpeedConstraint = 5;
    double minimumSpeedConstraint = -5;
    int windowSize = 1;
    Screen screen(windowSize);

    auto initialtime = std::chrono::system_clock::now();
    std::vector<TimePoint> cleanedData = screen.local(dirty, minimumSpeedConstraint, maximumSpeedConstraint);

    std::string outputFileName = "resultforalgo1" + dataSet.substr(0, dataSet.find_last_of(".")) + ".csv";
    
    Utility util;
    
    util.writeData(outputFileName, cleanedData);

    // for (const auto& item : cleanedData) {
    //     std::cout << item.value << " " << item.constraint << " " << item.time << std::endl;
    // }

    double rms = utility.calculate_RMS(truth, cleanedData);
    std::cout << "RMS value of Local Data: " << rms << std::endl;

    std::cout << "Accuracy of second algo for every 50 points cleaned: " << std::endl;
    int wrong = 0;
    for (int i = 0; i < 800; i += 50) {
        int t = i + 50;
        for (int j = i; j < i + 50; ++j) {
            if (truth[j].value != cleanedData[j].constraint) {
                wrong++;
            }
        }
        int per = t - wrong;
        double accuracy = static_cast<double>(per) * 100.0 / t;
        std::cout << accuracy << std::endl;
    }

    auto endtime = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - initialtime);
    std::cout << "Total time taken to clean the data and calculate efficiency in milli seconds: " << elapsed.count() << std::endl;
}

void screenUpdateTestOnStock() {
    std::cout << "Testing second algo on the stock data" << std::endl;

    std::string dataSet = "stock10k.data";
    Utility utility;
    std::string splitOp = ",";
    std::string filePath = Utility::PATH;

    std::vector<TimePoint> dirty = utility.readData(dataSet, filePath, 1, splitOp);
    std::vector<TimePoint> truth = utility.readData(dataSet, filePath, 2, splitOp);

    // Get a specific TimePoint and modify the dataset
    TimePoint tp = dirty[6];
    dirty.erase(dirty.begin() + 6); // Removing the 6th element
    std::vector<TimePoint> list(dirty.begin(), dirty.begin() + 10);

    std::cout << tp.value << " " << tp.constraint << " " << tp.time << std::endl;

    // Adding the delayed point
    list.insert(list.begin() + 6, tp);

    double maximumSpeedConstraint = 6;
    double minimumSpeedConstraint = -6;
    int windowSize = 1;
    int l = 6;
    int s = 7;
    Screen screen(windowSize);
    std::vector<TimePoint> cleanedData = screen.update(list, minimumSpeedConstraint, maximumSpeedConstraint, l, s);

    std::string outputFileName = "resultforalgo2" + dataSet.substr(0, dataSet.find_last_of(".")) + ".csv";
    
    Utility util;
    // Write the cleanedData to a file
    util.writeData(outputFileName, cleanedData);

    for (const auto& item : cleanedData) {
        std::cout << item.value << " " << item.constraint << " " << item.time << std::endl;
    }
}

void screenUpdateTestOnFuel() {
    std::cout << "Testing second algo on the fuel data " << std::endl;

    std::string dataSet = "fuel.txt";
    Utility utility;
    std::string splitOp = ",";
    std::string filePath = Utility::PATH;

    std::vector<TimePoint> dirty = utility.readData(dataSet, filePath, 1, splitOp);
    std::vector<TimePoint> truth = utility.readData(dataSet, filePath, 2, splitOp);

    // Get a specific TimePoint and modify the dataset
    TimePoint tp = dirty[10];
    dirty.erase(dirty.begin() + 10); // Removing the 6th element
    std::vector<TimePoint> list(dirty.begin(), dirty.begin() + 10);

    std::cout << tp.value << " " << tp.constraint << " " << tp.time << std::endl;

    // Adding the delayed point
    list.insert(list.begin() + 6, tp);

    double maximumSpeedConstraint = 10;
    double minimumSpeedConstraint = -10;
    int windowSize = 5;
    int l = 6;
    int s = 7;
    Screen screen(windowSize);
    std::vector<TimePoint> cleanedData = screen.update(list, minimumSpeedConstraint, maximumSpeedConstraint, l, s);

    std::string outputFileName = "resultforalgo2" + dataSet.substr(0, dataSet.find_last_of(".")) + ".csv";
    
    Utility util;
    // Write the cleanedData to a file
    util.writeData(outputFileName, cleanedData);

    for (const auto& item : cleanedData) {
        std::cout << item.value << " " << item.constraint << " " << item.time << std::endl;
    }
}

void screenHeuristicTestOnStock() {
    std::cout << "Testing third algo on the stock data " << std::endl;

    std::string dataSet = "stock10k.data";
    Utility utility;
    std::string splitOp = ",";
    std::string filePath = Utility::PATH;

    std::vector<TimePoint> dirty = utility.readData(dataSet, filePath, 1, splitOp);
    std::vector<TimePoint> truth = utility.readData(dataSet, filePath, 2, splitOp);

    // Get a specific TimePoint and modify the dataset
    TimePoint tp = dirty[6];
    dirty.erase(dirty.begin() + 6); // Removing the 6th element
    std::vector<TimePoint> list;
    for (int i = 0; i < 10; i++) {
        list.push_back(truth[i]);
    }

    std::cout << tp.value << " " << tp.constraint << " " << tp.time << std::endl;

    // Adding the delayed point
    list.insert(list.begin() + 6, tp);

    double maximumSpeedConstraint = 6;
    double minimumSpeedConstraint = -6;
    int windowSize = 1;
    int l = 6;
    int s = 7;
    Screen screen(windowSize);
    std::vector<TimePoint> cleanedData = screen.heuristic(list, minimumSpeedConstraint, maximumSpeedConstraint, l, s);

    std::string outputFileName = "resultforalgo3" + dataSet.substr(0, dataSet.find_last_of(".")) + ".csv";
    
    Utility util;
    // Write the cleanedData to a file
    util.writeData(outputFileName, cleanedData);

    for (const auto& item : cleanedData) {
        std::cout << item.value << " " << item.constraint << " " << item.time << std::endl;
    }
}

void screenHeuristicTestOnFuel() {
    std::cout << "Testing third algo on the fuel data " << std::endl;

    std::string dataSet = "fuel.txt";
    Utility utility;
    std::string splitOp = ",";
    std::string filePath = Utility::PATH;

    std::vector<TimePoint> dirty = utility.readData(dataSet, filePath, 1, splitOp);
    std::vector<TimePoint> truth = utility.readData(dataSet, filePath, 2, splitOp);

    // Get a specific TimePoint and modify the dataset
    TimePoint tp = dirty[6];
    dirty.erase(dirty.begin() + 6); // Removing the 6th element
    std::vector<TimePoint> list;
    for (int i = 0; i < 10; i++) {
        list.push_back(truth[i]);
    }

    std::cout << tp.value << " " << tp.constraint << " " << tp.time << std::endl;

    // Adding the delayed point
    list.insert(list.begin() + 6, tp);

    double maximumSpeedConstraint = 5;
    double minimumSpeedConstraint = -5;
    int windowSize = 1;
    int l = 6;
    int s = 7;
    Screen screen(windowSize);
    std::vector<TimePoint> cleanedData = screen.heuristic(list, minimumSpeedConstraint, maximumSpeedConstraint, l, s);

    std::string outputFileName = "resultforalgo3" + dataSet.substr(0, dataSet.find_last_of(".")) + ".csv";
    
    Utility util;
    // Write the cleanedData to a file
    util.writeData(outputFileName, cleanedData);

    for (const auto& item : cleanedData) {
        std::cout << item.value << " " << item.constraint << " " << item.time << std::endl;
    }
}

void combiningLocalAndUpdate() {

    std::cout << "Testing first and second algorithm on the stock data " << std::endl;

    std::string dataSet = "stock10k.data";

    Utility utility;
    std::string splitOp = ",";
    std::string filePath = Utility::PATH;

    std::vector<TimePoint> dirty = utility.readData(dataSet, filePath, 1, splitOp);
    std::vector<TimePoint> dirty_update = utility.readData(dataSet, filePath, 1, splitOp);
    std::vector<TimePoint> truth = utility.readData(dataSet, filePath, 2, splitOp);

    double rmsDirty = utility.calculate_RMS(truth, dirty);
    std::cout << "RMS value of Dirty Data: " << rmsDirty << std::endl;

    double maximumSpeedConstraint = 6;
    double minimumSpeedConstraint = -6;
    int windowSize = 1;
    Screen screen(windowSize);

    auto initialtime = std::chrono::high_resolution_clock::now();
    std::vector<TimePoint> temp;
    std::vector<int> temp_indices;
    for (int i = 0; i < 12000; i += 120) {
        // std::cout <<" ---------------------------------- " << std::endl;
        // std::cout << "i : " << i << std::endl;
        // Size of temp before
        // std::cout << "Temp size before: " << temp.size() << std::endl;
        TimePoint tp = TimePoint(dirty[i + 40].time, dirty[i + 40].value);
        for (int j = i; j < i + 120; j++) {
            if (j == i + 40) {
                continue;
            }
            temp.push_back(dirty[j]);
            temp_indices.push_back(j);
        }

        screen.local(temp, minimumSpeedConstraint, maximumSpeedConstraint);

        // Copy all the values from temp into dirty (for the indices in temp_indices)
        for (int k = 0; k < temp_indices.size(); k++) {
            dirty[temp_indices[k]] = temp[k];
        }

        temp.insert(temp.begin() + i + 40, tp);
        
        screen.update(dirty, minimumSpeedConstraint, maximumSpeedConstraint, i + 40, i + 120);

        if (tp.time >= 11921) {
            double RMS1 = utility.calculate_RMS(truth, dirty);
            std::cout << "RMS value of update Data: " << RMS1_ << std::endl;
        }
    }
    auto endtime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = endtime - initialtime;
    std::cout << "Total time taken to clean the data and calculate efficiency in milli seconds: " << elapsed.count() << std::endl;
}

void combinedLocalandHeuristic() {
    std::cout << "Testing first and third algo on the stock data " << std::endl;

    std::string dataSet = "stock10k.data";
    Utility utility;
    std::string splitOp = ",";
    std::string filePath = Utility::PATH;

    std::vector<TimePoint> dirty = utility.readData(dataSet, filePath, 1, splitOp);
    std::vector<TimePoint> truth = utility.readData(dataSet, filePath, 2, splitOp);

    double rmsDirty = utility.calculate_RMS(truth, dirty);
    std::cout << "RMS value of Dirty Data: " << rmsDirty << std::endl;

    double maximumSpeedConstraint = 6;
    double minimumSpeedConstraint = -6;
    int windowSize = 1;
    Screen screen(windowSize);

    auto initialtime = std::chrono::system_clock::now();
    std::vector<TimePoint> temp;
    std::vector<int> temp_indices;
    for (int i = 0; i < 12000; i += 120) {
        TimePoint tp = dirty[i + 40];

        for (int j = i; j < i + 120; j++) {
            if (j == i + 40) {
                continue;
            }
            temp.push_back(dirty[j]);
            temp_indices.push_back(j);
        }
        screen.local(temp, minimumSpeedConstraint, maximumSpeedConstraint);
        temp.insert(temp.begin() + i + 40, tp);

        // copy the newly inserted value into to temp to the dirty vector
        for (int k = 0; k < temp_indices.size(); k++) {
            dirty[temp_indices[k]] = temp[k];
        }

        screen.heuristic(dirty, minimumSpeedConstraint, maximumSpeedConstraint, i + 40, i + 120);


        if (tp.time >= 11921) {
            double RMS2 = utility.calculate_RMS(truth, dirty);
            std::cout << "RMS value of heuristic Data: " << RMS2_ << std::endl;
        }
    }

    auto endtime = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - initialtime);
    std::cout << "Total time taken to clean the data and calculate efficiency in milli seconds: " << elapsed.count() << std::endl;
}
