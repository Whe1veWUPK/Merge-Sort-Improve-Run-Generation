#pragma once
#include<chrono>
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
public:
    void startTimer(); //开始计时
    void endTimer(); //结束计时
    void calculateTime(); //计算时间并显示
};