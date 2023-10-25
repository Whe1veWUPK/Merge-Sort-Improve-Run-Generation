#ifndef PAIR_HPP
#define PAIR_HPP
class Pair{
private:
    int value;
    int key;

public:
    Pair();                   // Default Constructor
    Pair(int value, int key); // Constructor
    int first();              // get Value of the Pair
    int second();             // get key of the Pair
};

#endif