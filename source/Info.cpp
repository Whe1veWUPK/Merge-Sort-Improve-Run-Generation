#include <iostream>
#include "Info.hpp"
// Default Constructor
Info::Info(){
    this->value = 0;
    this->key = -1;
}
// Constructor with parameters
Info::Info(int value, int key,bool isValid){
    this->value = value;
    this->key = key;
    this->isValid = isValid;
}
// get The value of the Pair
int Info::getValue(){
    return this->value;
}
// get the key of the Pair
int Info::getKey(){
    return this->key;
}

//get the validity of the info 
bool Info::getIsValid(){
    return this->isValid;
}