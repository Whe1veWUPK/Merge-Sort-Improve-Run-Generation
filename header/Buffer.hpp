#pragma once
class Buffer {
private:
    int bufferSize; // buffer的size大小
    int curLocation; // buffer 此时处理到的位置
    int curSize; //此时buffer里面存放的数据量
    bool isFull; //此时buffer的状态 是否为满
    bool isEmpty; //此时buffer的状态 是否为空

public:
    int* buffer;//实际的buffer
    Buffer(int bufferSize); //Buffer的构造函数
    Buffer(); //Buffer的默认构造函数
    Buffer(const Buffer&b);//Buffer 的拷贝构造函数
    int getBufferSize(); //bufferSize 的 get函数
    int getCurLocation();  //curLocation 的 get 函数
    int getCurSize(); //curSize 的 get 函数
    void setBufferSize(int bufferSize); // bufferSize 的 set 函数
    void setCurLoation(int curLocation); // curLocation 的 set 函数
    void setCurSize(int curSize); // curSize 的 set 函数
    void moveCurPos();//更新curPos的函数 每次更新1
    void append(int index, int value);// Buffer 插入的函数 内含更新 empty状态 full状态
    void resize(int resize);//重置Buffer容量 的函数
    void quickSort(int left, int right);//对Buffer 内部的数据进行快速排序 时间复杂度为 T(n)=O(nlogn)
    bool Full(); //查询 buffer的状态 是否为满
    bool Empty(); //查询 buffer的状态 是否为空
    bool isOver(); // 查询当前buffer是否已经完成Merge
};