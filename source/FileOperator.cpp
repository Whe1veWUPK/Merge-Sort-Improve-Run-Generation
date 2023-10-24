#include<iostream>
#include<fstream>
#include<sstream>
#include "FileOperator.hpp"

void FileOperator::writeToInputBuffer(std::string filepath,int predictSize,int startPostion,Buffer*inputBuffer){
    // 将预测大小的数据量写入到 给定的 inputBuffer中
    int bufferSize = inputBuffer->getBufferSize();
    // 如果预测大小与实际提供的 buffer大小不同  则输出错误信息
    if(bufferSize<predictSize){
        std::cerr << "The input buffer size is not enough !"
                  << "\n";
        std::cerr  << "Predict Size is: " << predictSize << " Current Size is: " << bufferSize << "\n";
    }
    std::fstream fin(filepath,std::ios::in);
    // 如果文件无法打开则输出错误信息
    if(!fin.is_open()){
        std::cerr << "The file: " << filepath << " can not be opened"
                  << "\n";
    }
    std::string line;
    int curPostion = 0;
    int readSize = 0;
    while(std::getline(fin,line)){
        std::istringstream iss(line);
        if(curPostion<startPostion){
            ++curPostion;
        }
        else{
            if(readSize>=predictSize){
                //如果已经读到预期Size的大小 则直接退出
                break;
            }
            else{
                int n;
                iss >> n;
                inputBuffer->append(readSize,n);
                ++readSize;
            }
        }
    }
    fin.close();
}
void FileOperator::writeToFile(std::string filepath,int predictSize,bool isFirst,Buffer*outputBuffer){
    int bufferSize = outputBuffer->getBufferSize();
    // 如果预测大小与实际提供的 buffer大小不同  则输出错误信息
    if(bufferSize>predictSize){
        std::cerr << "The output buffer size is bigger than predict !"
                  << "\n";
        std::cerr << "Predict Size is: " << predictSize << " Current Size is: " << bufferSize << "\n";
    }
    std::fstream fout;
    if(isFirst){
        //如果是某一轮次的第一个 outputBuffer输出 则重写文档
        fout.open(filepath, std::ios::out);
    }
    else{
        //如果不是 则续写文档
        fout.open(filepath, std::ios::out | std::ios::app);
    }
    // 如果文件无法打开则输出错误信息
    if(!fout.is_open()){
        std::cerr << "The file: " << filepath << " can not be opened"
                  << "\n"; 
    }
    std::ostringstream oss;
    for (int i = 0; i < predictSize;++i){
        oss << outputBuffer->buffer[i] << "\n";
    }

    std::string outString = oss.str();
    //将信息输出到文件中
    fout << outString;
    fout.close();
}

void FileOperator::updateInputFile(std::string outputPath,std::string inputPath){
    //当一轮次结束后 将input文档进行更新
    std::fstream fin(outputPath, std::ios::in);
    std::fstream fout(inputPath, std::ios::out);
    if(!fin.is_open()){
        std::cerr << "The File: " << outputPath << " can not be opened"
                  << "\n";
        
    }
    if(!fout.is_open()){
        std::cerr << "The File: " << inputPath << " can not be opened"
                  << "\n";        
    }

    std::string line;
    while(std::getline(fin, line)){
        fout << (line+"\n");
    }
   
    fin.close();
    fout.close();
}
void FileOperator::createInitialRuns(std::string filepath,std::string outputPath,int runSize,Buffer*inputBuffer){
    int curPos = 0;
    int dataSize = this->getDataSize(filepath);
    bool isFirst = true;
    while(curPos<dataSize){
        //读取到 inputBuffer 里面
        this->writeToInputBuffer(filepath, runSize, curPos, inputBuffer);
        //更新坐标
        curPos += inputBuffer->getCurSize();
        //排序
        inputBuffer->quickSort(0, inputBuffer->getCurSize() - 1);
        //写到文件中去
        this->writeToFile(outputPath, inputBuffer->getCurSize(), isFirst, inputBuffer);
        //更新isFirst
        isFirst = false;
        //重置buffer
        inputBuffer->resize(runSize);
    }
    //更新文档
    this->updateInputFile(outputPath, filepath);
}
int FileOperator::getDataSize(std::string filepath){
    std::fstream fin(filepath, std::ios::in);
    if(!fin.is_open()){
        std::cerr << "The File: " << filepath << " can not be opened"
                  << "\n";
    }
    std::string line;
    
    
    int size = 0;
    
    while(std::getline(fin, line)){
        ++size;
    }
    fin.close();
    return size;
}