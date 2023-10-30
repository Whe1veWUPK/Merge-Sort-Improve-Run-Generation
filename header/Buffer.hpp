#pragma once
class Buffer {
private:
    int bufferSize; // buffer��size��С
    int curLocation; // buffer ��ʱ������λ��
    int curSize; //��ʱbuffer�����ŵ�������
    bool isFull; //��ʱbuffer��״̬ �Ƿ�Ϊ��
    bool isEmpty; //��ʱbuffer��״̬ �Ƿ�Ϊ��

public:
    int* buffer;//ʵ�ʵ�buffer
    Buffer(int bufferSize); //Buffer�Ĺ��캯��
    Buffer(); //Buffer��Ĭ�Ϲ��캯��
    Buffer(const Buffer&b);//Buffer �Ŀ������캯��
    int getBufferSize(); //bufferSize �� get����
    int getCurLocation();  //curLocation �� get ����
    int getCurSize(); //curSize �� get ����
    void setBufferSize(int bufferSize); // bufferSize �� set ����
    void setCurLoation(int curLocation); // curLocation �� set ����
    void setCurSize(int curSize); // curSize �� set ����
    void moveCurPos();//����curPos�ĺ��� ÿ�θ���1
    void append(int index, int value);// Buffer ����ĺ��� �ں����� empty״̬ full״̬
    void resize(int resize);//����Buffer���� �ĺ���
    void quickSort(int left, int right);//��Buffer �ڲ������ݽ��п������� ʱ�临�Ӷ�Ϊ T(n)=O(nlogn)
    bool Full(); //��ѯ buffer��״̬ �Ƿ�Ϊ��
    bool Empty(); //��ѯ buffer��״̬ �Ƿ�Ϊ��
    bool isOver(); // ��ѯ��ǰbuffer�Ƿ��Ѿ����Merge
};