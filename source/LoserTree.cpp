#include <iostream>
#include "LoserTree.hpp"
// constructor
LoserTree::LoserTree(int *data, int size)
{
    this->size = size;
    this->invalidNum = 0;
    this->loserArray = new Info[2 * this->size];
    this->winnerArray = new Info[this->size];
    // initial the loser tree array
    for (int i = 0; i < size; ++i){
        loserArray[i + size] = Info(data[i], i,true);
    }
    //do modify from bottom to up
    int curIndex = this->size - 1;
    while (curIndex != 0){
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
    if (index < this->size / 2){
        leftChild = winnerArray[index * 2];
        rightChild = winnerArray[index * 2 + 1];
    }
    else{
        leftChild = loserArray[index * 2];
        rightChild = loserArray[index * 2 + 1];
    }
    // get the loser and winner
    Info loser, winner;
    if((leftChild.getIsValid())&&(rightChild.getIsValid())){
           //two data are both valid
        loser = (leftChild.getValue() > rightChild.getValue()) ? leftChild : rightChild;
        winner = ((loser.getValue() == leftChild.getValue()) && (loser.getKey() == leftChild.getKey())) ? rightChild : leftChild;
    }
    else if ((!leftChild.getIsValid()) && (rightChild.getIsValid())){
        //leftchild is invalid
        loser = leftChild;
        winner = rightChild;
    }
    else if((leftChild.getIsValid())&&(!rightChild.getIsValid())){
        //rightchild is invalid
        loser = rightChild;
        winner = leftChild;
    }
    else{
        //leftchild and rightchild are both invalid
        loser = (leftChild.getValue() > rightChild.getValue()) ? leftChild : rightChild;
        winner = ((loser.getValue() == leftChild.getValue()) && (loser.getKey() == leftChild.getKey())) ? rightChild : leftChild;       
    }
    loserArray[index] = loser;
    winnerArray[index] = winner;
}
// adjust the tree when the list is changed
void LoserTree::adjust(int index, int newValue){
    int innerIndex = index + this->size;
    if(newValue<this->loserArray[0].getValue()){
        //then the newdata is invalid
        this->loserArray[innerIndex] = Info(newValue, index, false);
    }
    else{
        //the newdata is valid
        this->loserArray[innerIndex] = Info(newValue, index, true);
    }
    int curIndex = innerIndex / 2;
    //Update the losertree from curIndex to the root 
    while(curIndex!=0){
        this->modify(curIndex);
        //Move to its parent
        curIndex /= 2;
    }
    //update final winner
    this->loserArray[0] = this->winnerArray[1];
}
// to detect if the invalidNum is equal to the data size
bool LoserTree::needToCreateNewRun(){
    return (this->invalidNum == this->size);
}