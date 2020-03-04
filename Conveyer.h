

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

    int myTimer;
    int stager;//Stage counter
    int firstNum;
    int secondNum;
    int resOfProd;

    bool isProductTime;
    bool isFinish;
    bool isSaveTime;

    vector<int> first;
    vector<int> second;
    vector<pair<int, double>> outputVec;

    mutex blockCurrent;
    mutex correctPrint;
    mutex stepPrint;

    void createCurrent(int index);
    void makeProduct();
    void productStep(unsigned int secondP, unsigned int &result);
    void printStep(int digit, int stepInPrint, int sum, int one, int two);
    void saveResult();
    void printOutput();
    void printInput();

    static void printBinary(int numInTen);

};

#endif //CONVEER_CONVEYER_H
