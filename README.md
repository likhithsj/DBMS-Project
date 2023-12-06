# DBMS-Project

------------------
We have chosen the research paper "SCREEN: Stream Data Cleaning under Speed Constraints," which was presented at SIGMOD 2015, to implement in C++.

**Algorithm_File**
- `Screen.cpp`: This file has Algorithm 1 (local), algorithm 2 (update), and Algorithm 3 (heuristic) which are from the paper.

**Test_File**
- `main.cpp`: Class containing methods to run test cases based on different Datasets with different Algorithms mentioned above.

**Header_Files**
- `TimePoint.h`: header for TimePoint indicating a time point.
- `Utility.h`: header to load data from the DataSet,  and to calculate RMS value.

Datasets
---------

we have collected two datasets from the internet. The first one is Stock data which was collected from Yahoo and the second dataset is Fuel data which was collected from Kaggle. The format of the data in the DataSet is the timestamp of the data, dirty data, and truth data. These data sets can be found in the code folder `data/`.

To run the program first run 
```
make clean
```
then run 
```
make
```
then to run all the algorithms run the below command
```
./main
```

Parameters
-----------
The input and output of **Screen** algorithm 1 (local) is:

Function to call:

```
local(errorDataList, double minimumSpeedConstraint, double maximumSpeedConstraint);
```

Input:

```
vector<TimePoint> dirty 
double maximumSpeedConstraint = 10;
double minimumSpeedConstraint = -10;
```

Output

```
Testing first algo on the stock data 
RMS value of Dirty Data: 9.06559
RMS value of Local Data: 3.97865
Accuracy of the first algo for every 2000 points cleaned: 
81.3
81.375
81.5167
82.025
82.67
Total time taken to clean the data and calculate efficiency in milliseconds: 40
```
The input and output of **Screen** algorithm 2 (update) is:

Function to call:

```
update(rectifiedDataList, double minimumSpeedConstraint, double maximumSpeedConstraint, int delayed_position, int firstFiniteDataPoints)
```

Input:

```
vector<TimePoint> repairedList
double maximumSpeedConstraint = 6;
double minimumSpeedConstraint = -6;
int windowSize = 1;
int l = 6;
int s = 7;
```

Output

```
vector<TimePoint> resultList
```
The input and output of **Screen** algorithm 3 (heuristic) is:

Method

```
heuristic(rectifiedDataList, double minimumSpeedConstraint, double maximumSpeedConstraint, int delayed_position, int firstFiniteDataPoints)
```

Input:

```
double maximumSpeedConstraint = 6;
double minimumSpeedConstraint = -6;
int windowSize = 1;
int l = 6;
int s = 7;
```

Output

```
vector<TimePoint> resultList
```

- The maximumSpeedConstraint, minimumSpeedConstraint, and window size for the FUEL dataset are -10, 10, and 5 respectively.
- We have written test cases for all three algorithms following each dataset and a comparison between the local and update algorithms and local and heuristic algorithms.
- By running the above ./main command all the test cases run and outputs are obtained.

Finally, the obtained Output is attached below for the above-mentioned constraints

![image](https://github.com/likhithsj/DBMS-Project/assets/53929108/eaf77c58-7469-49ff-9793-7fc005dc2970)
![image](https://github.com/likhithsj/DBMS-Project/assets/53929108/6082be5e-5cf2-4a87-a6ec-d6dcb863afda)


