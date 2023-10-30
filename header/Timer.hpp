#pragma once
#include<chrono>
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
public:
    void startTimer(); //��ʼ��ʱ
    void endTimer(); //������ʱ
    void calculateTime(); //����ʱ�䲢��ʾ
};