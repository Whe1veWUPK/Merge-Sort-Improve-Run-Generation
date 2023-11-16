#include <iostream>
#include "Buffer.hpp"
Buffer::Buffer() {
    //Buffer的无参构造函数
    this->setBufferSize(10);
    this->setCurLoation(0);
    this->setCurSize(0);
    this->isFull = false;
    this->isEmpty = true;
    this->buffer = new int[this->bufferSize];

}
Buffer::Buffer(int bufferSize) {
    //Buffer的构造函数
    this->setBufferSize(bufferSize);
    this->setCurLoation(0);
    this->setCurSize(0);
    this->isFull = false;
    this->isEmpty = true;
    this->buffer = new int[this->bufferSize];
}
Buffer::Buffer(const Buffer& b) {
    this->bufferSize = b.bufferSize;
    this->curLocation = b.curLocation;
    this->curSize = b.curSize;
    this->isEmpty = b.isEmpty;
    this->isFull = b.isFull;
    this->resize(this->curSize);
    for (int i = 0; i < this->bufferSize; ++i) {
        this->buffer[i] = b.buffer[i];
    }
}
void Buffer::moveCurPos() {
    //更新curPos的函数
    this->curLocation += 1;
}
void Buffer::backCurPos(){
    //回溯curPos的函数
    this->curLocation -= 1;
    --this->curSize;
}
int Buffer::getBufferSize() {
    return this->bufferSize;
}
int Buffer::getCurLocation() {
    return this->curLocation;
}
int Buffer::getCurSize() {
    return this->curSize;
}
void Buffer::setBufferSize(int bufferSize) {
    this->bufferSize = bufferSize;
}
void Buffer::setCurLoation(int curLocation) {
    this->curLocation = curLocation;
}
void Buffer::setCurSize(int curSize) {
    this->curSize = curSize;
}
void Buffer::append(int index, int value) {
    //向buffer 里 插入数据 的函数
    this->buffer[index] = value;
    ++this->curSize;
    if (this->curSize > 0) {
        this->isEmpty = false;
    }
    if (this->curSize == this->bufferSize) {
        this->isFull = true;
    }
}
void Buffer::resize(int resize) {
    //重新设定 buffer 的 size 应在每一轮结束后使用
    this->bufferSize = resize;
    this->buffer = new int[this->bufferSize];
    this->isEmpty = true;
    this->isFull = false;
    this->curSize = 0;
    this->curLocation = 0;
}
void Buffer::quickSort(int left, int right) {
    //快速排序
    if (left < right) {
        int i = left, j = right, x = this->buffer[left];
        while (i < j) {
            while (i < j && this->buffer[j] >= x) // 从右向左找第一个小于x的数
                j--;
            if (i < j)
                this->buffer[i++] = this->buffer[j];
            while (i < j && this->buffer[i] < x) // 从左向右找第一个大于等于x的数
                i++;
            if (i < j)
                this->buffer[j--] = this->buffer[i];
        }
        this->buffer[i] = x;
        this->quickSort(left, i - 1); // 递归调用
        this->quickSort(i + 1, right);
    }

}
bool Buffer::Full() {
    //判断 buffer是否为满
    return this->isFull;
}
bool Buffer::Empty() {
    //判断 buffer是否为空
    return this->isEmpty;
}
bool Buffer::isOver() {
    //查询当前buffer是否已经Merge Over
    if (this->curLocation >= this->curSize) {
        return true;
    }
    else {
        return false;
    }
}