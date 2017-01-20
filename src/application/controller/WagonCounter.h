
#ifndef WAGONCOUNTER_H
#define WAGONCOUNTER_H

#include "DebugFrames.h"
#include "WagonCounter.h"
#include "../../system/logic/input/keyboard/InputFacade.h"
#include "../../system/features/BinaryFilter.h"
#include "../config/ConfigExample.h"

/*
 * logic for counting wagons 
 */
class WagonCounter{
    
private:
    int colSize; // number of columns to scan
    int startCol;
    int endCol;
    bool initialized = false;
    bool hasStarted = false;
    bool hasEnded = false;
    
    
    // pre calculation  
    int* columnCount;
    int* startRow;
    int min, minColumn, max, avg, diffMM;
    
    
    // approach 1
    int lockedCounter = 0;
    bool locked = false;
    int wagons1 = 0;
    
    
public:
    WagonCounter();
    void setScanArea(int, int);
    bool validateLimis(Mat*);
    void evaluate(Mat*, Mat*);
    int getMin(int*, int, int);
    int getMinCol(int*, int, int);
    int getMax(int*, int, int);
    int getAvg(int*, int, int);
    void incWagon();
    void startLock();
    
    void preCalculation(Mat*);
    
    void approach1(int);
    void approach2(int*);
    
};


#endif //WAGONCOUNTER_H
