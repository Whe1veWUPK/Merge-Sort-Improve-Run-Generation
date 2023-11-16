#include <iostream>
#include "Buffer.hpp"
Buffer::Buffer() {
    //Buffer���޲ι��캯��
    this->setBufferSize(10);
    this->setCurLoation(0);
    this->setCurSize(0);
    this->isFull = false;
    this->isEmpty = true;
    this->buffer = new int[this->bufferSize];

}
Buffer::Buffer(int bufferSize) {
    //Buffer�Ĺ��캯��
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
    //����curPos�ĺ���
    this->curLocation += 1;
}
void Buffer::backCurPos(){
    //����curPos�ĺ���
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
    //��buffer �� �������� �ĺ���
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
    //�����趨 buffer �� size Ӧ��ÿһ�ֽ�����ʹ��
    this->bufferSize = resize;
    this->buffer = new int[this->bufferSize];
    this->isEmpty = true;
    this->isFull = false;
    this->curSize = 0;
    this->curLocation = 0;
}
void Buffer::quickSort(int left, int right) {
    //��������
    if (left < right) {
        int i = left, j = right, x = this->buffer[left];
        while (i < j) {
            while (i < j && this->buffer[j] >= x) // ���������ҵ�һ��С��x����
                j--;
            if (i < j)
                this->buffer[i++] = this->buffer[j];
            while (i < j && this->buffer[i] < x) // ���������ҵ�һ�����ڵ���x����
                i++;
            if (i < j)
                this->buffer[j--] = this->buffer[i];
        }
        this->buffer[i] = x;
        this->quickSort(left, i - 1); // �ݹ����
        this->quickSort(i + 1, right);
    }

}
bool Buffer::Full() {
    //�ж� buffer�Ƿ�Ϊ��
    return this->isFull;
}
bool Buffer::Empty() {
    //�ж� buffer�Ƿ�Ϊ��
    return this->isEmpty;
}
bool Buffer::isOver() {
    //��ѯ��ǰbuffer�Ƿ��Ѿ�Merge Over
    if (this->curLocation >= this->curSize) {
        return true;
    }
    else {
        return false;
    }
}