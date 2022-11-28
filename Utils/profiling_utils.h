#ifndef PRISM_PROFILING_UTILS_H
#define PRISM_PROFILING_UTILS_H

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
#endif //PRISM_PROFILING_UTILS_H
