#include <iostream>
#include "LoserTree.hpp"
//constructor
LoserTree::LoserTree(int*data,int size){
    this->size = size;
    this->invalidNum = 0;
    this->loserArray = new Pair[2 * this->size];
    this->winnerArray = new Pair[this->size];
    //initial the loser tree array
    for (int i = 0; i < size;++i){
        loserArray[i + size] = Pair(data[i], i);
    }
    //do modify from bottom to up
    int curIndex = this->size - 1;
    while(curIndex !=0){
        modify(curIndex);
        --curIndex;
    }
    //update final winner
    this->loserArray[0] = this->winnerArray[1];
}
// get the final winner (the min value of the list)
Pair LoserTree::getTop(){
    return this->loserArray[0];
}
// modify the tree node at the index
void LoserTree::modify(int index){
    Pair leftChild ;
    Pair rightChild;
	if (index < this->size / 2){
		leftChild = winnerArray[index * 2];
		rightChild = winnerArray[index * 2 + 1];
	}
	else{
		leftChild = loserArray[index * 2];
		rightChild = loserArray[index * 2 + 1];
	}
    // get the loser and winner 
	Pair loser = (leftChild.first() > rightChild.first()) ? leftChild : rightChild;
	Pair winner = ((loser.first() == leftChild.first())&&(loser.second()==leftChild.second())) ? rightChild : leftChild;
	loserArray[index] = loser;
	winnerArray[index] = winner;
}
// adjust the tree when the list is changed
void LoserTree::adjust(int index,int newValue,bool flag){
    if(flag){
        //if the newValue less than the last
        ++this->invalidNum;
        int innerIndex = this->size + index;
        this->loserArray[innerIndex] = Pair(newValue, index);
        int curIndex = innerIndex / 2;
        Pair leftChild=this->loserArray[curIndex*2];
        Pair rightChild = this->loserArray[curIndex * 2 + 1];
        //the invalid data should lose first round
        this->loserArray[curIndex] = this->loserArray[innerIndex];
        this->winnerArray[curIndex] = ((leftChild.first() == this->loserArray[curIndex].first()) && (leftChild.second() == this->loserArray[curIndex].second())) ? rightChild : leftChild;
        curIndex /= 2;
        //then as normal to update the loser tree
        while(curIndex!=0){
            modify(curIndex);
            //move to its parent
            curIndex /= 2;
        }
        //update final winner
        this->loserArray[0] = this->winnerArray[1];
    }
    else{
        //just as normal
        int innerIndex = index + this->size;
        this->loserArray[innerIndex] = Pair(newValue, index);
        int curIndex = innerIndex / 2;
        while(curIndex!=0){
            modify(curIndex);
            //move to its parent
            curIndex /= 2;
        }
        //update final winner
        this->loserArray[0] = this->winnerArray[1];
    }

}
// to detect if the invalidNum is equal to the data size
bool LoserTree::needToCreateNewRun(){
    return (this->invalidNum == this->size);
}