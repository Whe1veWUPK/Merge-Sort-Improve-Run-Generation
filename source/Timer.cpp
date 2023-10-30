#include<iostream>
#include<ratio>
#include "Timer.hpp"

void Timer::startTimer() {
    //��ʼ��ʱ
    this->start = std::chrono::high_resolution_clock::now();
}
void Timer::endTimer() {
    //������ʱ
    this->end = std::chrono::high_resolution_clock::now();
}
void Timer::calculateTime() {
    //������ʱ
    // // ����Ϊ��λ����������ʱ��
    std::cout << "In second time:";
    std::chrono::duration<double, std::ratio<1, 1>> diffS = this->end - this->start;
    std::cout << "Time is " << diffS.count() << " s\n";
    // // �Ժ���Ϊ��λ����������ʱ��
    std::cout << "In millisecond time:";
    std::chrono::duration<double, std::ratio<1, 1000>> diffM = this->end - this->start;
    std::cout << "Time is " << diffM.count() << " ms\n";
    // // ��΢��Ϊ��λ����������ʱ��
    std::cout << "In microsecond time:";
    std::chrono::duration<double, std::ratio<1, 1000000>> diffMicro = this->end - this->start;
    std::cout << "Time is " << diffMicro.count() << " us\n";
    // // ������Ϊ��λ����������ʱ��
    std::cout << "In nanosecond time:";
    std::chrono::duration<double, std::ratio<1, 1000000000>> diffN = this->end - this->start;
    std::cout << "Time is " << diffN.count() << " ns\n";
}