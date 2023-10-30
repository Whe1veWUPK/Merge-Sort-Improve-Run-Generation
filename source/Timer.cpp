#include<iostream>
#include<ratio>
#include "Timer.hpp"

void Timer::startTimer() {
    //开始计时
    this->start = std::chrono::high_resolution_clock::now();
}
void Timer::endTimer() {
    //结束计时
    this->end = std::chrono::high_resolution_clock::now();
}
void Timer::calculateTime() {
    //计算用时
    // // 以秒为单位，返回所用时间
    std::cout << "In second time:";
    std::chrono::duration<double, std::ratio<1, 1>> diffS = this->end - this->start;
    std::cout << "Time is " << diffS.count() << " s\n";
    // // 以毫秒为单位，返回所用时间
    std::cout << "In millisecond time:";
    std::chrono::duration<double, std::ratio<1, 1000>> diffM = this->end - this->start;
    std::cout << "Time is " << diffM.count() << " ms\n";
    // // 以微秒为单位，返回所用时间
    std::cout << "In microsecond time:";
    std::chrono::duration<double, std::ratio<1, 1000000>> diffMicro = this->end - this->start;
    std::cout << "Time is " << diffMicro.count() << " us\n";
    // // 以纳秒为单位，返回所用时间
    std::cout << "In nanosecond time:";
    std::chrono::duration<double, std::ratio<1, 1000000000>> diffN = this->end - this->start;
    std::cout << "Time is " << diffN.count() << " ns\n";
}