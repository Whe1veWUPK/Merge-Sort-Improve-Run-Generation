#pragma once
#include "Info.hpp"
class LoserTree
{
public:
    LoserTree(int* data, int size);                  // constructor of LoserTree
    Info* winnerArray;                               // store winner nodes
    Info* loserArray;                                //  store loser tree
    Info getTop();                                   // get the final winner (the min value)
    void adjust(int index, int newValue); // adjust the tree when the value of the list is changed
    void adjust(int newValue, bool isValid);//adjust the tree when the value of the list is changed (for final clear the tree)
    bool needToCreateNewRun();                       // tell you if you should create the new run
    void setValid();//set all data in the tree to valid
    int getSize();//get the loserTree's size
    int getInvalidNum();//get the invalid data's num

private:
    int size;       // the number of the leaves
    int invalidNum; // the number of the invalid value (need to build a new sequence)

    void modify(int index); // modify the index of the nodes
};