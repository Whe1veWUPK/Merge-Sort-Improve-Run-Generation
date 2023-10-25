#include <iostream>
#include "Pair.hpp"
// Default Constructor
Pair::Pair()
{
    this->value = 0;
    this->key = -1;
}
// Constructor with parameters
Pair::Pair(int value, int key)
{
    this->value = value;
    this->key = key;
}
// get The value of the Pair
int Pair::first()
{
    return this->value;
}
// get the key of the Pair
int Pair::second()
{
    return this->key;
}
