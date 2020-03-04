
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
    isFinish = false;
    isProductTime = false;
    isSaveTime = false;
    myTimer = 0;
    stager = 0;

    correctPrint.lock();
    while(!isFinish){
        myTimer++;
        cout << "\nTact - " << stager+1 << endl;
        thread one(&Conveyer::saveResult, this);
        thread two(&Conveyer::makeProduct, this);
        thread three(&Conveyer::createCurrent, this, stager);
        one.join();
        two.join();
        three.join();
        stager++;
    }
    correctPrint.unlock();
    printOutput();
}

/**
 * Creates current pair of numbers from vectors, and save it in object
 *
 * @param index index of numbers in vectors
 */
void Conveyer::createCurrent(int index) {
    bool isInVector = index >= 0 && index < first.size();
    if(isInVector) {
        stepPrint.lock();
        cout << "Get numbers\n";
        stepPrint.unlock();

        blockCurrent.lock();
        bool isCorrectFirst = first[index] >= 0 && first[index] < MAX_NUM_TO_CALC;
        isCorrectFirst ? firstNum = first[index] : firstNum = 1;
        bool isCorrectSecond = second[index] >= 0 && second[index] < MAX_NUM_TO_CALC;
        isCorrectSecond ? secondNum = second[index] : secondNum = 1;
        blockCurrent.unlock();

        isProductTime = true;
    }else{
        isProductTime = false;
    }
}

/**
 * carry main calculation on every stage
 *
 * @see calculate()
 * @see productStep()
 */
void Conveyer::makeProduct() {
    blockCurrent.lock();
    if(isProductTime){
        int pStager = stager;
        stepPrint.lock();
        cout << "Index of elements " << pStager-1 << endl;
        stepPrint.unlock();
        isSaveTime = true;
        unsigned int firstP = firstNum;  //Make copy (Because of threads)
        unsigned int secondP = secondNum;//Make copy (Because of threads)
        blockCurrent.unlock();

        unsigned int result = 0;
        int size;
        for(int i = MAX_DIGIT_INDEX; i >= 0; i--)
            if(firstP & (1<<i)) {
                size = i;
                break;
            }
        /**
         * Cycle use productStep() to make calculations
         */
        for(int digit = size; digit >= 0; digit--){
            if(firstP & (1 << digit)){
                productStep(secondP, result);
            }else{
                result = result<<1;
            }

            int stepForPrint = size - digit;//make correct index to display
            stepPrint.lock();
            printStep(digit, stepForPrint, result, firstP, secondP);
            stepPrint.unlock();
        }
        result = result >> 1;
        resOfProd = result;
    }else blockCurrent.unlock();
}

/**
 * Calculates one step of binary multiplication
 *
 * @param secondP second factor of multiplication
 * @param result saves results of previous steps
 */
void Conveyer::productStep(unsigned int secondP, unsigned int &result) {
    bool balance = false;
    for(int i = 0; i <= MAX_RESULT_DIGIT_INDEX; i++){
        bool ifOne = result & (1<<i);//Check is 1 in Ith digit of result
        bool oneOne = secondP & (1<<i) && result & (1<<i);//1 and 1 ?
        bool oneZero = secondP & (1<<i) || result & (1<<i);// 1-0 or 0-1 ?
        bool zeroZero = true;//is 0-0 ?
        /**
         * Calculates according to BOOL variables
         */
        if(oneOne){
            if(!balance){
                balance = true;
                result = result ^ (1<<i);
            }
        }else if(oneZero){
            if(balance){
                if(ifOne)
                    result = result ^ (1<<i);
            }else{
                if(!ifOne)
                    result = result ^ (1<<i);
            }
        }else if (zeroZero){
            if(balance){
                result = result ^ (1<<i);
                balance = false;
            }
        }
    }
    //Shifts the result
    result = result << 1;
}

/**
 * Display to console binary number
 *
 * @param numInTen number in denary number system
 */
void Conveyer::printBinary(int numInTen) {
    for(int i = MAX_RESULT_DIGIT_INDEX; i >= 0; i--){
        if(numInTen & (1<<i)) {
            cout << "1";
        }else cout << "0";
        if(i % 4 == 0)cout << "-";
    }
    cout << endl;
}

/**
 * Prints result of concrete digit of multiplication
 *
 * @param digit digit in first number of multiplication
 * @param stepInPrint step of multiplication
 * @param sum subtotal to print
 * @param one correct copy of first number of multiplication (Because of threads)
 * @param two correct copy of second number of multiplication (Because of threads)
 */
void Conveyer::printStep(int digit, int stepInPrint, int sum, int one, int two) {
    cout << "Step <" << stepInPrint << "> \n";
    one & (1 << digit) ? printBinary(two) : printBinary(0);
    printBinary(sum);
    cout << "----------------------------------------\n";
}

/**
 * Saves result number to vector on every stage
 */
inline void Conveyer::saveResult() {
    if(isSaveTime) {
        outputVec.push_back(pair(resOfProd, myTimer));
        cout << "Save result\n";
        if (outputVec.size() == first.size()) {
            isFinish = true;
        }
    }
}

/**
 * Prints input vectors
 */
void Conveyer::printInput() {
    correctPrint.lock();
    cout << "Vectors\n";
    for(int i = 0; i < first.size(); i++){
        cout << "[" << i << "] " << first[i] << " " << second[i] << endl;
    }
    correctPrint.unlock();
}

/**
 * Prints result vector, and time to it calculation
 */
void Conveyer::printOutput() {
    cout << "\nOutput result\n";
    for(int i = 0; i < first.size(); i++){
        cout << "[" << i << "] " << outputVec[i].first
            << " Tacts - " <<  outputVec[i].second << endl;
    }
}

