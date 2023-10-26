#ifndef PAIR_HPP
#define PAIR_HPP
class Info{
private:
    int value;
    int key;
    bool isValid;

public:
    Info();                   // Default Constructor
    Info(int value, int key,bool isValid); // Constructor
    int getValue();           // get Value of the Info
    int getKey();             // get key of the Info
    bool getIsValid(); //get the info's validity
};

#endif