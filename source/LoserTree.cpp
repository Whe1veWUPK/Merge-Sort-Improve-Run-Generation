#include <iostream>
#include "LoserTree.hpp"
// constructor
LoserTree::LoserTree(int* data, int size)
{ 
    if(size%2!=0){
        this->size = size + 1;
    }
    else{
        this->size = size;
    }
    int maxValue = (((1 << 30) - 1) * 2+1);
    this->invalidNum = 0;
    this->loserArray = new Info[2 * this->size];
    this->winnerArray = new Info[this->size];
    // initial the loser tree array
    if(this->size==size){
       for (int i = 0; i < this->size; ++i) {
         loserArray[i + size] = Info(data[i], i, true);
       }
    }
    else{
       for (int i = 0; i < this->size;++i){
         if(i!=this->size-1){
         loserArray[i + this->size] = Info(data[i], i, true);
         }
         else{
         loserArray[i + this->size] = Info(maxValue, i, false);
         }
       }
    }
    //do modify from bottom to up
    int curIndex = this->size - 1;
    while (curIndex != 0) {
        modify(curIndex);
        --curIndex;
    }
    // update final winner
    this->loserArray[0] = this->winnerArray[1];
}
// get the final winner (the min value of the list)
Info LoserTree::getTop()
{
    return this->loserArray[0];
}
// modify the tree node at the index
void LoserTree::modify(int index)
{
    Info leftChild;
    Info rightChild;
    if (index < this->size / 2) {
        leftChild = winnerArray[index * 2];
        rightChild = winnerArray[index * 2 + 1];
    }
    else {
        leftChild = loserArray[index * 2];
        rightChild = loserArray[index * 2 + 1];
    }
    // get the loser and winner
    Info loser, winner;
    if ((leftChild.getIsValid()) && (rightChild.getIsValid())) {
        //two data are both valid
        loser = (leftChild.getValue() > rightChild.getValue()) ? leftChild : rightChild;
        winner = ((loser.getValue() == leftChild.getValue()) && (loser.getKey() == leftChild.getKey())) ? rightChild : leftChild;
    }
    else if ((!leftChild.getIsValid()) && (rightChild.getIsValid())) {
        //leftchild is invalid
        loser = leftChild;
        winner = rightChild;
    }
    else if ((leftChild.getIsValid()) && (!rightChild.getIsValid())) {
        //rightchild is invalid
        loser = rightChild;
        winner = leftChild;
    }
    else {
        //leftchild and rightchild are both invalid
        loser = (leftChild.getValue() > rightChild.getValue()) ? leftChild : rightChild;
        winner = ((loser.getValue() == leftChild.getValue()) && (loser.getKey() == leftChild.getKey())) ? rightChild : leftChild;
    }
    loserArray[index] = loser;
    winnerArray[index] = winner;
}
// adjust the tree when the list is changed
void LoserTree::adjust(int index, int newValue) {
    int innerIndex = index + this->size;
    if (newValue < this->loserArray[0].getValue()) {
        //then the newdata is invalid
        this->loserArray[innerIndex] = Info(newValue, index, false);
        //update invalidNum
        ++this->invalidNum;
    }
    else {
        //the newdata is valid
        this->loserArray[innerIndex] = Info(newValue, index, true);
    }
    int curIndex = innerIndex / 2;
    //Update the losertree from curIndex to the root 
    while (curIndex != 0) {
        this->modify(curIndex);
        //Move to its parent
        curIndex /= 2;
    }
    //update final winner
    this->loserArray[0] = this->winnerArray[1];
}
// to detect if the invalidNum is equal to the data size
bool LoserTree::needToCreateNewRun() {
    return (this->invalidNum == this->size);
}
//set all the data in the tree to valid
void LoserTree::setValid() {
    for (int i = 0; i < this->size; ++i) {
        this->winnerArray[i].setValid();
        this->loserArray[i].setValid();
        this->loserArray[i + this->size].setValid();
    }
    //update the invalidNum
    this->invalidNum = 0;
}
//get LoserTree's Size
int LoserTree::getSize() {
    return this->size;
}
void LoserTree::adjust(int newValue, bool isValid) {
    Info info(newValue, this->getTop().getKey(), isValid);
    int innerIndex = this->size + this->getTop().getKey();
    //Update inner value
    this->loserArray[innerIndex] = info;
    int curIndex = innerIndex / 2;
    
    if (!isValid) {
        //if the value is invalid, update the invalidNum
        ++this->invalidNum;
    }
    //Update the losertree from curIndex to the root 
    while (curIndex != 0) {
        this->modify(curIndex);
        //Move to its parent
        curIndex /= 2;
    }
    //update final winner
    this->loserArray[0] = this->winnerArray[1];
}
//get invalid data's num
int LoserTree::getInvalidNum() {
    return this->invalidNum;
}