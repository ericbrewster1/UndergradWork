#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

class Counter
{
public:
    static vector<int> digitizer(int number); //convert int to vector of ints for counter of bombs
    static vector<int> displayTimer(double number); //display timer (similar to digitizer)
};