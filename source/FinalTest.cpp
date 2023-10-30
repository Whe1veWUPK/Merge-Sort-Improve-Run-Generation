#include<chrono>
#include<iostream>
#include<thread>
#include<vector>
#include "LoserTree.hpp"
#include "FileOperator.hpp"
//vector that store the each run's length
std::vector<int>runLength;
//for get the best mergesequence
struct Node {
    int parent;
    int leftChild;
    int rightChild;
    int weight;
};
//Thread 1: read data from the disk
void readFromDisk(Buffer* inputBuffer, FileOperator* fileOperator, int startPosition, std::string filePath) {
    //get inputBuffer's Size 
    int predictSize = inputBuffer->getBufferSize();
    //
    inputBuffer->resize(predictSize);
    //read info from file
    fileOperator->writeToInputBuffer(filePath, predictSize, startPosition, inputBuffer);
}
//Thread 2: generate Run
void generateRun(LoserTree* loserTree, Buffer* inputBuffer, Buffer* outputBuffer,int*overLen) {
    *overLen = -1;
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
            loserTree->setValid();
            //record the overLen
            *overLen = count;
        }
        
    }
    //if the tree has not set the data's validity
    if (*overLen == -1) {
        //count is equal to the bufferSize
        *overLen = count;
    }
}
//Thread 3: write data to the disk
void writeToDisk(Buffer* outputBuffer, FileOperator* FileOperator, bool isFirst, std::string  filePath) {

    //get outputBuffer's size
    int predictSize = outputBuffer->getCurSize();
    if (outputBuffer->Empty()) {
        std::cerr << "The outputBuffer is empty! \n";
        return;
    }
    //write the buffer's data to disk
    FileOperator->writeToFile(filePath, predictSize, isFirst, outputBuffer);
    if (isFirst) {
        isFirst = false;
    }
}

void startSort() {
    // set the bufferSize
    std::cout << "Please input the buffer's size\n";
    int bufferSize = 0;
    std::cin >> bufferSize;
    Buffer* buffers = new Buffer[3];
    for (int i = 0; i < 3; ++i) {
        buffers[i] = Buffer(bufferSize);
    }
    //set the losertree's size
    std::cout << "Please input the loserTree's size \n";
    int treeSize = 0;
    std::cin >> treeSize;
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
    std::cout << "initial buffer already\n";
    while (curLocation < dataSize) {
        //std::cout << "CurLocation is: " << curLocation << "\n";
        //read from the disk
        std::thread t1(readFromDisk,&buffers[(pin+1)%3],&fileOperator,curLocation,filepath);
        //run generation
        overLen = 0;
        std::thread t2(generateRun, loserTree, &buffers[pin], &buffers[(pin+2)%3],&overLen);
        t2.join();
        //std::cout << "Run generated successfully\n";
        //write the output buffer to the disk
        std::thread t3(writeToDisk, &buffers[(pin + 2) % 3], &fileOperator, isFirst, outputPath);
        // if the run is over
        if (overLen<buffers[(pin+2)%3].getCurSize()) {
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
    generateRun(loserTree, &buffers[pin], &buffers[(pin + 2)%3], &overLen);
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

    //record the final run's length
    runLength.emplace_back(curLen);
    //write the final buffer to the disk
    writeToDisk(&finalBuffer, &fileOperator,false,"Output.txt");

   
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
    
    startSort();
    getBestMergeSequence();
    system("pause");
    return 0;
}