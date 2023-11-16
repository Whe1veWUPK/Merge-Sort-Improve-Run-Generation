#include<chrono>
#include<iostream>
#include<thread>
#include<vector>
#include<queue>
#include<deque>
#include<fstream>
#include<cmath>
#include "LoserTree.hpp"
#include "FileOperator.hpp"
#include "Timer.hpp"
//int curRunNum
int currentRunNum = 0;
//vector that store the each run's length
std::vector<int>runLength;
//for get the best mergesequence
struct Node {
    int parent;
    int leftChild;
    int rightChild;
    int weight;
};
//read data from the disk
void readFromDisk(Buffer* inputBuffer, FileOperator* fileOperator, int startPosition, std::string filePath) {
    //get inputBuffer's Size 
    int predictSize = inputBuffer->getBufferSize();
    //
    inputBuffer->resize(predictSize);
    //read info from file
    fileOperator->writeToInputBuffer(filePath, predictSize, startPosition, inputBuffer);
}
//generate Run
void generateRun(LoserTree* loserTree, Buffer* inputBuffer, Buffer* outputBuffer,int*overLen,bool*isReset) {
    *overLen = -1;
    *isReset = false;
    //get two buffers' size
    int bufferSize = inputBuffer->getCurSize();
    //num of valid data
    int count = 0;
    for (int i = 0; i < bufferSize; ++i) {
       //get data from tree
        outputBuffer->append(i, loserTree->getTop().getValue());
        // move data into the tree
        loserTree->adjust(loserTree->getTop().getKey(), inputBuffer->buffer[i]);
        ++count;
        //if all the data in the tree is invalid
        if (loserTree->needToCreateNewRun()) {
            //std::cout << loserTree->getTop().getValue() << " \n";
            loserTree->setValid();
            //record the overLen
            *overLen = count;
            *isReset = true;
        }
        
    }
    //if the tree has not set the data's validity
    if (*overLen == -1) {
        //count is equal to the bufferSize
        *overLen = count;
    }
}
//write data to the disk
void writeToDisk(Buffer* outputBuffer, FileOperator* FileOperator, bool isFirst, std::string  filePath) {

    //get outputBuffer's size
    int predictSize = outputBuffer->getCurSize();
    if (outputBuffer->Empty()) {
        return;
    }
    //write the buffer's data to disk
    FileOperator->writeToFile(filePath, predictSize, isFirst, outputBuffer);
}
//read the run's data that will exhaust first
void readRunData(Buffer*freeBuffer,FileOperator*fileOperator,std::string filePath,std::deque<int>*workingBufferQueue,int numOfMergeWays,int runNum,int*runCurPosition,int*runStartPosition,bool isFirst,bool*isOver,int*curRunNum,int startPos,int dataSize,std::queue<int>*freeIndex,int bufferSize){
    //the process has been done in intial part
    if(isFirst){
        return;
    }
    char runPath[128];
    //find the run that will exhaust first
    int key = -1;
    int minValue = (((1 << 30) - 1) * 2 + 1);
    for (int i = 0; i < numOfMergeWays;++i){
        if(isOver[i]){
            // if the run is over, then just skip
            continue;
        }
        int curSize = freeBuffer[workingBufferQueue[i].back()].getCurSize();
        if(freeBuffer[workingBufferQueue[i].back()].buffer[curSize-1]<minValue){
            key = i;
            minValue = freeBuffer[workingBufferQueue[i].back()].buffer[curSize - 1];
        }
    }
    //if all runs have been read
    if(key==-1){
        return;
    }
    //write data to the buffer
    //get the new Buffer's index
    int newKey = freeIndex->front();
    freeIndex->pop();
    //add the new Key to the working buffer queue
    workingBufferQueue[key].push_back(newKey);
    //update run's cur pos
    if(runCurPosition[startPos+key]+bufferSize<=runLength[startPos+key]){
        sprintf(runPath, "Run\\Run%d.txt", startPos + key);
        fileOperator->writeToInputBuffer(runPath, bufferSize, runCurPosition[startPos+key], &freeBuffer[newKey]);
        runCurPosition[startPos+key] += bufferSize;

    }
    else if((runCurPosition[startPos+key]+bufferSize>runLength[startPos+key])&&(runCurPosition[startPos+key]<runLength[startPos+key])){
        sprintf(runPath, "Run\\Run%d.txt", startPos + key);
        fileOperator->writeToInputBuffer(runPath, runLength[startPos+key] - runCurPosition[startPos+key],runCurPosition[startPos+key],&freeBuffer[newKey]);
        runCurPosition[startPos+key] = runLength[startPos+key];

    }
    //set the over info and curRunNum info
    if(runCurPosition[startPos+key]>=runLength[startPos+key]){
        //the run is over
        if(isOver[key]){
            return;
        }
        else{
            isOver[key] = true;
            --*curRunNum;
        }
    }
    //std::cout << freeBuffer[workingBufferQueue[key].back()].getCurSize() << "\n";
    int curSize=freeBuffer[workingBufferQueue[key].back()].getCurSize();
    if(curSize==0){
       //the run is over
       //1.resize the buffer
        freeBuffer[workingBufferQueue[key].back()].resize(bufferSize);
       //2. pop_back it
        workingBufferQueue[key].pop_back();
       //3  push it to the freeIndex
        freeIndex->push(newKey);
    }
}
//k-way-merge
void startMerge(LoserTree*loserTree,Buffer*outputBuffer,Buffer*freeBuffer,std::deque<int>*workingBufferQueue,int*runNumNotMerged,bool*isOver,std::queue<int>*freeIndex){
    // get bufferSize
    int bufferSize = outputBuffer->getBufferSize();
    //resize outputbuffer
    outputBuffer->resize(bufferSize);
    if(*runNumNotMerged==0){
        //no run,then over 
        return;
    }
    // Do until the outputBuffer becomes full or all runs are over
    for (int i = 0; i < bufferSize;++i){
        if(*runNumNotMerged==0){
            //no run, then return
            return;
        }
        //write the winner into the output buffer
        outputBuffer->append(i, loserTree->getTop().getValue());
        //get the winner's key and adjust the loserTree
        int key = loserTree->getTop().getKey();
        if(workingBufferQueue[key].size()==0&&isOver[key]){
            //if the run is Over
            std::cout << "The queue is empty and the run is over\n";
            loserTree->adjust(-1, false);
            continue;
        }
        else{
            //the run is not Over
            // record the index
            int indexTemp = workingBufferQueue[key].front();
            //store the buffer
            Buffer temp(bufferSize);
            for (int i = 0; i <freeBuffer[indexTemp].getCurSize() ;++i){
                temp.append(i, freeBuffer[indexTemp].buffer[i]);
            }
            //set location 
            temp.setCurLoation(freeBuffer[indexTemp].getCurLocation());

            //std::cout << "The buffer's size is:" << freeBuffer[workingBufferQueue[key].front()].getCurSize() << "\n";


            //move curPos
            freeBuffer[workingBufferQueue[key].front()].moveCurPos();
            if(freeBuffer[workingBufferQueue[key].front()].isOver()){
                //the buffer's data has all been merged
                //1.free the buffer
                freeBuffer[workingBufferQueue[key].front()].resize(bufferSize);

                //2.move to the next buffer in the queue
                workingBufferQueue[key].pop_front();
                if(workingBufferQueue[key].size()==0&&isOver[key]){
                    // the run is over
                    //add invalid value to the loserTree
                    loserTree->adjust(-1,false);
                    std::cout << "The Run " << key << " is Over\n";
                    //change the num
                    --*runNumNotMerged;
                    continue;
                }
                else if(workingBufferQueue[key].size()==0&&!isOver[key]){
                    //the run is not Over
                    //exit the merge, to wait the I/O, recovery the data
                    //1. back the output buffer
                    outputBuffer->backCurPos();
                    //2. recovery the data
                    freeBuffer[indexTemp] = Buffer(bufferSize);
                    for (int i = 0; i < temp.getCurSize();++i){
                        freeBuffer[indexTemp].append(i, temp.buffer[i]);
                    }
                    freeBuffer[indexTemp].setCurLoation(temp.getCurLocation());
                    //std::cout << "the copy buffer " << freeBuffer[indexTemp].getCurSize() << "\n";
                    // 3. recover the queue
                    workingBufferQueue[key].push_back(indexTemp);
                    //4. exit
                    return;
                }
                //3. add new value to the loserTree
                int curLoc = freeBuffer[workingBufferQueue[key].front()].getCurLocation();
                int newValue = freeBuffer[workingBufferQueue[key].front()].buffer[curLoc];
                loserTree->adjust(key, newValue);
                //4. update freeIndex
                freeIndex->push(indexTemp);
            }
            else{
                //the buffer is not over
                //add new value to the loserTree
                int curLoc = freeBuffer[workingBufferQueue[key].front()].getCurLocation();
                int newValue = freeBuffer[workingBufferQueue[key].front()].buffer[curLoc];
                loserTree->adjust(key, newValue);
            }
        }

    }
}
void createRuns(std::string filePath){
    FileOperator file;
    std::fstream ifs;
    ifs.open(filePath, std::ios::in);
    std::string line;
    char dataOutputPath[128];
    int curPos = 0;
    int curNum = 0;
    std::fstream ofs;
    sprintf(dataOutputPath, "Run\\Run%d.txt", curNum);
    ofs.open(dataOutputPath,std::ios::out);
    while(std::getline(ifs,line)){
        if(curPos<runLength[curNum]){
            ofs << line;
            ofs << "\n";
            ++curPos;

        }
        else{
            ofs.close();
            curPos = 0;
            ++curNum;
            sprintf(dataOutputPath, "Run\\Run%d.txt", curNum);
            ofs.open(dataOutputPath,std::ios::out);
            ofs << line;
            ofs << "\n";
            ++curPos;
        }
    }
    ifs.close();
}
void startSort(int bufferSize,int treeSize) {
    FileOperator file;
    int dataNum = file.getDataSize("Input.txt");
    //output the data size
    std::cout << "The data size is " << dataNum << "\n";

    Buffer* buffers = new Buffer[3];
    for (int i = 0; i < 3; ++i) {
        buffers[i] = Buffer(bufferSize);
    }

    FileOperator  fileOperator;
    std::string filepath = "Input.txt";
    std::string outputPath = "Output.txt";
    int dataSize = fileOperator.getDataSize(filepath);
    int curLocation = 0;
    //initial loserTree
    Buffer tempBuffer = Buffer(treeSize);
    fileOperator.writeToInputBuffer(filepath, treeSize, curLocation, &tempBuffer);
    LoserTree* loserTree = new LoserTree(tempBuffer.buffer, treeSize);
    //update curLocation
    curLocation += treeSize;
    bool isFirst = true;
    int pin = 0;
    //initial one buffer
    fileOperator.writeToInputBuffer(filepath,buffers[0].getBufferSize(), curLocation, &buffers[0]);
    //update curLocation
    curLocation += buffers[0].getCurSize();
    int curLen = 0;
    // tag to refer the cur Run whether is over
    int overLen = 0;
    //loop until the whole file has been read
    //std::cout << "initial buffer already\n";
    while (curLocation < dataSize) {
        //std::cout << "CurLocation is: " << curLocation << "\n";
        //read from the disk
        std::thread t1(readFromDisk,&buffers[(pin+1)%3],&fileOperator,curLocation,filepath);
        //run generation
        overLen = 0;
        bool isReset=false;
        std::thread t2(generateRun, loserTree, &buffers[pin], &buffers[(pin+2)%3],&overLen,&isReset);
        t2.join();
        //std::cout << "Run generated successfully\n";
        //write the output buffer to the disk
        std::thread t3(writeToDisk, &buffers[(pin + 2) % 3], &fileOperator, isFirst, outputPath);
        // if the run is over
        if (overLen<buffers[(pin+2)%3].getCurSize()||isReset) {
            //std::cout << overLen << "<"<< buffers[(pin + 2) % 3].getCurSize()<<"\n";

            //compute the curLen
            curLen += overLen;
            //record the cur run's lenth
            //std::cout << "CurLen is:" << curLen << "\n";
            runLength.emplace_back(curLen);
            //reset the curLen
            curLen = buffers[(pin + 2) % 3].getCurSize() - overLen;
            
        }
        else {
            curLen += buffers[(pin + 2) % 3].getCurSize();
        }
        t1.join();
       //update cur location
        curLocation += buffers[(pin + 1) % 3].getCurSize();
        
        t3.join();
        //update First flag
        if (isFirst) {
            isFirst = false;
        }
        //clear buffer
        buffers[pin].resize(buffers[pin].getBufferSize());
        buffers[(pin + 2) % 3].resize(buffers[pin].getBufferSize());
       //update next pin
        pin = (pin + 1) % 3;

    }
    //there is a buffer that has already read data from disk but has not been write to loserTree
    overLen = 0;
    bool isReset = false;
    generateRun(loserTree, &buffers[pin], &buffers[(pin + 2)%3], &overLen,&isReset);
    writeToDisk(&buffers[(pin + 2) % 3], &fileOperator, false, outputPath);
    if (overLen < buffers[(pin + 2) % 3].getCurSize()) {
        //std::cout << overLen << "<" << buffers[(pin + 2) % 3].getCurSize() << "\n";

        //compute the curLen
        curLen += overLen;
        //record the cur run's lenth
        //std::cout << "CurLen is:" << curLen << "\n";
        runLength.emplace_back(curLen);
        //reset the curLen
        curLen = buffers[(pin + 2) % 3].getCurSize() - overLen;

    }
    else {
        curLen += buffers[(pin + 2) % 3].getCurSize();
    }
    //std::cout <<"The loser Tree's size is: " <<loserTree->getSize() << "\n";
    //the loser tree has remaining data
    //input  maxValue  into the tree
    int maxValue = (1<< 30);
    int maxNum = 0;
    Buffer finalBuffer(loserTree->getSize());

    while (maxNum<loserTree->getSize()) {
        finalBuffer.append(maxNum, loserTree->getTop().getValue());
        loserTree->adjust (maxValue,false);
        ++curLen;
        //if all data in the loser tree is invalid
        if (loserTree->needToCreateNewRun()) {
            
            loserTree->setValid();
            //record the run's length
            runLength.emplace_back(curLen);
            
            curLen = 0;
        }
        ++maxNum;
    }
    if(curLen!=0){
       //record the final run's length
       runLength.emplace_back(curLen);
       //write the final buffer to the disk

    }
    writeToDisk(&finalBuffer, &fileOperator,false,"Output.txt");
    createRuns("Output.txt");
}
//get Best Merge Sequence
void getBestMergeSequence() {
    int cost = 0;
    std::cout << "Best merge Sequence are as followed\n";
    //get runLength's size
    int size = runLength.size();
    std::vector<Node> nodes;
    //initial nodes
    for (int i = 0; i < size; ++i) {
        Node node;
        node.weight = runLength[i];
        //-1 means that it has only one node
        node.parent = -1;
        node.leftChild = -1;
        node.rightChild = -1;
        //append the node
        nodes.emplace_back(node);
    }
    //std::cout << "Runs' length are as followed: \n";

    //for (int i = 0; i < size; ++i) {
    //    std::cout << runLength[i] << " ";
    //}
    //std::cout << " \n";
    for (int i = 0; i < size-1; ++i) {
        for (int i = 0; i < nodes.size(); ++i) {
            if (nodes[i].parent == -1) {
                std::cout << nodes[i].weight << " ";
            }

        }
        std::cout << "\n";
        int min1 = (1 << 30);
        int index1 = 0;
        int min2 = (1 << 30);
        int index2 = 0;
        //find two min value
        for (int i = 0; i < nodes.size(); ++i) {
            if (nodes[i].parent == -1 && nodes[i].weight < min1) {
                min1 = nodes[i].weight;
                index1 = i;
            }
        }
        for (int i = 0; i < nodes.size(); ++i) {
            if (nodes[i].parent == -1 && nodes[i].weight < min2 && i != index1) {
                min2 = nodes[i].weight;
                index2 = i;
            }
        }
        //merge and create new node
        std::cout << "Merge " << nodes[index1].weight << " and " << nodes[index2].weight << "\n";
        Node newNode;
        newNode.weight = nodes[index1].weight + nodes[index2].weight;
        cost += newNode.weight;
        newNode.parent = -1;
        newNode.leftChild = index1;
        newNode.rightChild = index2;
        nodes.emplace_back(newNode);
        nodes[index1].parent = nodes.size() - 1;
        nodes[index2].parent = nodes.size() - 1;
    }
    std::cout << "The merge cost is: " << cost << "\n";
}
int main() {
    std::cout<<"Please input the bufferSize\n";
    int bufferSize;
    std::cin >> bufferSize;
    std::cout << "Please input the treeSize\n";
    int treeSize;
    std::cin >> treeSize;
    startSort(bufferSize,treeSize);
    getBestMergeSequence();
    system("pause");
    return 0;
}