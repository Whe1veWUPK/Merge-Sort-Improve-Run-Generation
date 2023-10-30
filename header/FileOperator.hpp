#pragma once
#include "Buffer.hpp"
class FileOperator {
public:
    FileOperator();//默认构造函数
    FileOperator(const FileOperator&f);
    void writeToInputBuffer(std::string filepath, int predictSize, int startPosition, Buffer* inputBuffer);  //将文件中预测量的数据从某一位置开始读取到inputBuffer中
    void writeToFile(std::string filepath, int predictSize, bool isFirst, Buffer* outputBuffer); //将outputBuffer中的预测量的数据写入到输出文件中
    void updateInputFile(std::string outputPath, std::string inputPath); //update the input data
    void createInitialRuns(std::string filepath, std::string outputPath, int runSize, Buffer* inputBuffer);//generate initial runs
    int getDataSize(std::string filepath);//get the data size in the disk
};