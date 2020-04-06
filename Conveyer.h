

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

#ifndef CONVEER_CONVEYER_H
#define CONVEER_CONVEYER_H

class Conveyer{
public:
    void calculate(vector<int> first, vector<int> second);

    static void printBinary(int numInTen);
private:
    /**
     * Maximum number of digits in binary numbers that can be in that programm
     */
    static const int MAX_DIGIT_INDEX = 5;
    static const int MAX_RESULT_DIGIT_INDEX = 11;
    /**
     * Border of calculation to this conveyer
     */
    static const int MAX_NUM_TO_CALC = 64;
    static const int MIN_NUM_TO_CALC = 32;

    int myTimer;
    int stager;//Stage counter

    vector<int> first;
    vector<int> second;
    vector<pair<int, int>> outputVec;

    mutex corPrint;

    void checkVectors(int index);

    void addToResult(int digit, int index);
    void printStep(int digit, int index);

    void printOutput();
    void printInput();

};

#endif //CONVEER_CONVEYER_H
