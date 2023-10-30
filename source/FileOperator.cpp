#include<iostream>
#include<fstream>
#include<sstream>
#include "FileOperator.hpp"
FileOperator::FileOperator() {

}
FileOperator::FileOperator(const FileOperator&f) {

}
void FileOperator::writeToInputBuffer(std::string filepath, int predictSize, int startPostion, Buffer* inputBuffer) {
    // ��Ԥ���С��������д�뵽 ������ inputBuffer��
    int bufferSize = inputBuffer->getBufferSize();
    // ���Ԥ���С��ʵ���ṩ�� buffer��С��ͬ  �����������Ϣ
    if (bufferSize < predictSize) {
        std::cerr << "The input buffer size is not enough !"
            << "\n";
        std::cerr << "Predict Size is: " << predictSize << " Current Size is: " << bufferSize << "\n";
    }
    std::fstream fin(filepath, std::ios::in);
    // ����ļ��޷��������������Ϣ
    if (!fin.is_open()) {
        std::cerr << "The file: " << filepath << " can not be opened"
            << "\n";
    }
    std::string line;
    int curPostion = 0;
    int readSize = 0;
    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        if (curPostion < startPostion) {
            ++curPostion;
        }
        else {
            if (readSize >= predictSize) {
                //����Ѿ�����Ԥ��Size�Ĵ�С ��ֱ���˳�
                break;
            }
            else {
                int n;
                iss >> n;
                inputBuffer->append(readSize, n);
                ++readSize;
            }
        }
    }
    fin.close();
}
void FileOperator::writeToFile(std::string filepath, int predictSize, bool isFirst, Buffer* outputBuffer) {
    int bufferSize = outputBuffer->getBufferSize();
    // ���Ԥ���С��ʵ���ṩ�� buffer��С��ͬ  �����������Ϣ
    if ( predictSize>bufferSize) {
        std::cerr << "The predict size is bigger than the buffer's size !"
            << "\n";
        std::cerr << "Predict Size is: " << predictSize << " Current Size is: " << bufferSize << "\n";
    }
    std::fstream fout;
    if (isFirst) {
        //�����ĳһ�ִεĵ�һ�� outputBuffer��� ����д�ĵ�
        fout.open(filepath, std::ios::out);
    }
    else {
        //������� ����д�ĵ�
        fout.open(filepath, std::ios::out | std::ios::app);
    }
    // ����ļ��޷��������������Ϣ
    if (!fout.is_open()) {
        std::cerr << "The file: " << filepath << " can not be opened"
            << "\n";
    }
    std::ostringstream oss;
    for (int i = 0; i < predictSize; ++i) {
        oss << outputBuffer->buffer[i] << "\n";
    }

    std::string outString = oss.str();
    //����Ϣ������ļ���
    fout << outString;
    fout.close();
}

void FileOperator::updateInputFile(std::string outputPath, std::string inputPath) {
    //��һ�ִν����� ��input�ĵ����и���
    std::fstream fin(outputPath, std::ios::in);
    std::fstream fout(inputPath, std::ios::out);
    if (!fin.is_open()) {
        std::cerr << "The File: " << outputPath << " can not be opened"
            << "\n";

    }
    if (!fout.is_open()) {
        std::cerr << "The File: " << inputPath << " can not be opened"
            << "\n";
    }

    std::string line;
    while (std::getline(fin, line)) {
        fout << (line + "\n");
    }

    fin.close();
    fout.close();
}
void FileOperator::createInitialRuns(std::string filepath, std::string outputPath, int runSize, Buffer* inputBuffer) {
    int curPos = 0;
    int dataSize = this->getDataSize(filepath);
    bool isFirst = true;
    while (curPos < dataSize) {
        //��ȡ�� inputBuffer ����
        this->writeToInputBuffer(filepath, runSize, curPos, inputBuffer);
        //��������
        curPos += inputBuffer->getCurSize();
        //����
        inputBuffer->quickSort(0, inputBuffer->getCurSize() - 1);
        //д���ļ���ȥ
        this->writeToFile(outputPath, inputBuffer->getCurSize(), isFirst, inputBuffer);
        //����isFirst
        isFirst = false;
        //����buffer
        inputBuffer->resize(runSize);
    }
    //�����ĵ�
    this->updateInputFile(outputPath, filepath);
}
int FileOperator::getDataSize(std::string filepath) {
    std::fstream fin(filepath, std::ios::in);
    if (!fin.is_open()) {
        std::cerr << "The File: " << filepath << " can not be opened"
            << "\n";
    }
    std::string line;


    int size = 0;

    while (std::getline(fin, line)) {
        ++size;
    }
    fin.close();
    return size;
}