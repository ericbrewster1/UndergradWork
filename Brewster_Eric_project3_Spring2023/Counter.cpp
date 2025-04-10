#include "Counter.h"

vector<int> Counter::digitizer(int number)
{ //takes in an integer, returns a vector of integers
    vector<int> nums;
    if (number < -99)  //maximum number of negative mines
    {
        nums.push_back(10);
        nums.push_back(9);
        nums.push_back(9);
    }
    else if (number < -9)
    {
        nums.push_back(10);
        int temp = (number * -1) / 10;
        nums.push_back(temp);
        temp = (number * -1) % 10;
        nums.push_back(temp);
    }
    else if (number < 0)
    {
        nums.push_back(10);
        nums.push_back(0);
        nums.push_back(number * -1);
    }
    else if (number < 10)
    {
        nums.push_back(0);
        nums.push_back(0);
        nums.push_back(number);
    }
    else if (number < 100)
    {
        nums.push_back(0);
        int temp = number / 10;
        nums.push_back(temp);
        temp = number % 10;
        nums.push_back(temp);
    }
    else if (number < 1000) {
        int temp = number / 100;
        nums.push_back(temp);
        temp = number % 100;
        temp /= 10;
        nums.push_back(temp);
        temp = number % 10;
        nums.push_back(temp);
    }
    return nums;
}

vector<int> Counter::displayTimer(double number)
{
    vector<int> nums;
    int minutes = 0;
    int seconds = 0;
    if(number < 60)
    {
        minutes = 0;
        seconds = int(number);
    }
    else
    {
        while(number > 60)
        {
            number -= 60;
            minutes += 1;
            seconds = int(number);
        }
    }
    if(minutes < 10)
    {
        nums.push_back(0);
        nums.push_back(minutes);
    }
    else
    {
        nums.push_back(minutes / 10);
        nums.push_back(minutes % 10);
    }
    if(seconds < 10)
    {
        nums.push_back(0);
        nums.push_back(seconds);
    }
    else
    {
        nums.push_back(seconds / 10);
        nums.push_back(seconds % 10);
    }
    return nums;
}

