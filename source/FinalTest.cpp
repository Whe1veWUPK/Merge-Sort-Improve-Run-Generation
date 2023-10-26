#include<chrono>
#include<iostream>
#include "mingw.thread.h"
#include "LoserTree.hpp"
#include "FileOperator.hpp"
//Thread 1: read data from the disk
void readFromDisk(Buffer*inputBuffer,FileOperator*fileOperator,int startPosition,std::string filePath){
    //get inputBuffer's Size 
    int predictSize = inputBuffer->getBufferSize();
    //
    inputBuffer->resize(predictSize);
    //read info from file
    fileOperator->writeToInputBuffer(filePath, predictSize, startPosition, inputBuffer);
}
//Thread 2: generate Run
void generateRun(LoserTree*loserTree,Buffer*inputBuffer,Buffer*outputBuffer){
    
}
//Thread 3: write date to the disk
void writeToDisk(Buffer*outputBuffer,FileOperator*FileOperator,bool isFirst,int curRunLength,std::string filePath){
    //get outputBuffer's size
    int predictSize = outputBuffer->getBufferSize();
    if(outputBuffer->Empty()){
        std::cerr << "The outputBuffer is empty! \n";
        return;
    }
    //write the buffer's data to disk
    FileOperator->writeToFile(filePath, predictSize, isFirst, outputBuffer);
}

void startSort(){
    // set the bufferSize
    std::cout << "Please input the buffer's size\n";
    int bufferSize = 0;
    std::cin >> bufferSize;
    Buffer *inputBuffer = new Buffer[2];
    Buffer *outputBuffer = new Buffer[2];
    for (int i = 0; i < 2; ++i)
    {
        inputBuffer[i] = Buffer(bufferSize);
        outputBuffer[i] = Buffer(bufferSize);
    }
    //set the losertree's size
    std::cout << "Please input the loserTree's size \n";
    int treeSize = 0;
    std::cin >> treeSize;
    FileOperator *fileOperator;
    int dataSize = fileOperator->getDataSize("Input.txt");
    int curLocation = 0;
    //initial loserTree
    Buffer tempBuffer = Buffer(treeSize);
    fileOperator->writeToInputBuffer("Input.txt", treeSize, curLocation, &tempBuffer);
    LoserTree *loserTree = new LoserTree(tempBuffer.buffer, treeSize);
    //update curLocation
    curLocation += treeSize;
    

}
int main(){
    
    system("pause");
    return 0;
}