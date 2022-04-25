//
// Created by jonathan on 24/04/2022.
//

#ifndef FASTER_PROFILING_UTILS_H
#define FASTER_PROFILING_UTILS_H

#include <chrono>
#include <iostream>

using namespace std;
class Timer{
public:
    Timer(string function_name){
        function = function_name;
        startTime = std::chrono::high_resolution_clock::now();
    }

    ~Timer(){
    }

    void Stop(){
        auto endTime = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();
        auto duration = end - start;
        cout<< "Duration(" << function << ") : " << duration << endl;
    }
private:
    string function;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

};
#endif //FASTER_PROFILING_UTILS_H
