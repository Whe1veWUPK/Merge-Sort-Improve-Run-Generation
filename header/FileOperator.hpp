#ifndef FILEOPERATOR_HPP
#define FILEOPERATOR_HPP
#include "Buffer.hpp"
class FileOperator{
public:
    void writeToInputBuffer(std::string filepath,int predictSize,int startPosition,Buffer*inputBuffer);  //将文件中预测量的数据从某一位置开始读取到inputBuffer中
    void writeToFile(std::string filepath,int predictSize,bool isFirst,Buffer*outputBuffer); //将outputBuffer中的预测量的数据写入到输出文件中
    void updateInputFile(std::string outputPath,std::string inputPath); //一轮运行完毕后 将输入文件的数据进行更新
    void createInitialRuns(std::string filepath,std::string outputPath, int runSize,Buffer*inputBuffer);//生成初始的run
    int getDataSize(std::string filepath);//获取文件中的数据总量
};

#endif