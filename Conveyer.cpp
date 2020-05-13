
#include "Conveyer.h"

/**
 * Major function that
 * calculate values of two vectors, and displays result
 *
 * @param first first vector to calculate
 * @param second second vector to calculate
 */
void Conveyer::calculate(vector<int> first, vector<int> second) {
    this->first = first;
    this->second = second;

    printInput();

    /**
     * Initialization
     */
    myTimer = 0;
    stager = 0;
    int sizeOfVectors = first.size();
    for(int i = 0; i < sizeOfVectors; i++) checkVectors(i);
    outputVec.resize(sizeOfVectors, make_pair(0, 0));
    int startPoint = MAX_DIGIT_INDEX;

    while (stager < sizeOfVectors + MAX_DIGIT_INDEX + 1) {
        vector<thread> threads;
        myTimer++;
        cout << "\nTact - " << stager + 1 << endl;

        int index = stager;
        for (int digit = startPoint; digit >= 0; digit--) {
            if (index >= 0 && index < sizeOfVectors) {
                threads.emplace_back(&Conveyer::addToResult, this, digit, index);
            }
            index--;
        }

        for (thread &i : threads) {
            i.join();
        }

        stager++;
    }
    printOutput();
}

/**
 * Creates current pair of numbers from vectors, and save it in object
 *
 * @param index index of numbers in vectors
 */
void Conveyer::checkVectors(int index) {
    bool isInVector = index >= 0 && index < first.size();
    if (isInVector) {

        bool isCorrectFirst = first[index] >= MIN_NUM_TO_CALC && first[index] < MAX_NUM_TO_CALC;
        bool isCorrectSecond = second[index] >= MIN_NUM_TO_CALC && second[index] < MAX_NUM_TO_CALC;

        if (!isCorrectFirst) {
            first[index] = MIN_NUM_TO_CALC;
        }
        if (!isCorrectSecond) {
            second[index] = MIN_NUM_TO_CALC;
        }
    }
}

/**
 * Calculates one step of binary multiplication
 *
 * @param secondP second factor of multiplication
 * @param result saves results of previous steps
 */
void Conveyer::addToResult(int digit, int index) {
    bool balance = false;
    int result = outputVec[index].first;
    int secondP = second[index];
    int firstP = first[index];

    if (firstP & (1 << digit)) {
        secondP = secondP << digit;
        for (int i = 0; i <= MAX_RESULT_DIGIT_INDEX; i++) {
            bool ifOne = result & (1 << i);//Check is 1 in Ith digit of result
            bool oneOne = secondP & (1 << i) && result & (1 << i);//1 and 1 ?
            bool oneZero = secondP & (1 << i) || result & (1 << i);// 1-0 or 0-1 ?
            bool zeroZero = true;//is 0-0 ?
            /**
             * Calculates according to BOOL variables
             */
            if (oneOne) {
                if (!balance) {
                    balance = true;
                    result = result ^ (1 << i);
                }
            } else if (oneZero) {
                if (balance) {
                    if (ifOne)
                        result = result ^ (1 << i);
                } else {
                    if (!ifOne)
                        result = result ^ (1 << i);
                }
            } else if (zeroZero) {
                if (balance) {
                    result = result ^ (1 << i);
                    balance = false;
                }
            }
        }
    }
    outputVec[index].first = result;
    outputVec[index].second = myTimer;
    corPrint.lock();
    printStep(digit, index);
    corPrint.unlock();
}

/**
 * Display to console binary number
 *
 * @param numInTen number in denary number system
 */
void Conveyer::printBinary(int numInTen) {
    for (int i = MAX_RESULT_DIGIT_INDEX; i >= 0; i--) {
        if (numInTen & (1 << i)) {
            cout << "1";
        } else cout << "0";
        if (i % 4 == 0)cout << "-";
    }
    cout << endl;
}

void Conveyer::printStep(int digit, int index) {
    cout << "----------------------------------------\n";
    cout << "Calculations at digit <" << digit << ">" << " at index" << index <<  "\n";
    first[index] & (1 << digit) ? printBinary(second[index]<<digit) : printBinary(0);
    printBinary(outputVec[index].first);
    cout << "----------------------------------------\n";
}

/**
 * Prints input vectors
 */
void Conveyer::printInput() {
    cout << "Vectors\n";
    for (int i = 0; i < first.size(); i++) {
        cout << "[" << i << "] " << first[i] << " " << second[i] << endl;
    }
}

/**
 * Prints result vector, and time to it calculation
 */
void Conveyer::printOutput() {
    cout << "\nOutput result\n";
    for (int i = 0; i < first.size(); i++) {
        cout << "[" << i << "] " << outputVec[i].first
             << " Tacts - " << outputVec[i].second << endl;
    }
}

