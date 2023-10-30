#pragma once
#include "Buffer.hpp"
class FileOperator {
public:
    FileOperator();//Ĭ�Ϲ��캯��
    FileOperator(const FileOperator&f);
    void writeToInputBuffer(std::string filepath, int predictSize, int startPosition, Buffer* inputBuffer);  //���ļ���Ԥ���������ݴ�ĳһλ�ÿ�ʼ��ȡ��inputBuffer��
    void writeToFile(std::string filepath, int predictSize, bool isFirst, Buffer* outputBuffer); //��outputBuffer�е�Ԥ����������д�뵽����ļ���
    void updateInputFile(std::string outputPath, std::string inputPath); //update the input data
    void createInitialRuns(std::string filepath, std::string outputPath, int runSize, Buffer* inputBuffer);//generate initial runs
    int getDataSize(std::string filepath);//get the data size in the disk
};