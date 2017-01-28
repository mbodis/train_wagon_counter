
#include "../controllers/WagonCounter.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;


WagonCounter::WagonCounter(){
}

void WagonCounter::setScanArea(int mStartCol, int mColSize){
    
    if (!initialized){
        this->startCol = mStartCol;
        this->endCol = mStartCol + mColSize;
        this->colSize = mColSize;  
        initialized = true;      
    }
}

bool WagonCounter::validateLimis(Mat *binMat){
    if (!binMat->data){
        cout << "WagonCounter validateLimis Mat is EMPTY !" << endl;
        return false;
    }
    
    setScanArea(binMat->cols - 90, 89);
    
    if ( (binMat->cols < this->startCol) 
        || (binMat->cols < this->endCol)){
        cout << "WagonCounter start/end limit is higher than Mat size" << endl;
        return false;
    }
    
    return true;
}

void WagonCounter::evaluate(Mat *binMatInput, Mat *colMatOutput){

    Mat overlay;
    double alpha = 0.3;
    colMatOutput->copyTo(overlay);

    // has Mat content 
    if (!validateLimis(binMatInput)) return;
    
    // count cols, min, max, avg
    preCalculation(binMatInput);
    
    //*** apporaches ***//
    
    // approach 1 - if avg is below THRESHOLD + lock for (colSize)
    if (min > 0 && max > 0){
        approach1(diffMM);
    }
        
    // output
    stringstream sWagons, state1, state2, state3, state4;
	sWagons << wagons1;
	state1 << "min: " << min;
	state2 << "max: " << max;
	state3 << "avg: " << avg;
	state4 << "diff: " << diffMM;
    cout << "min: " << min << " minCol: " << minColumn << " max: " << max << " avg: " << avg << " diffMM: " << diffMM << endl;
    cv::rectangle(overlay, Point(startCol, 0), Point(binMatInput->cols, binMatInput->rows), colorBlack, CV_FILLED);
    addWeighted(overlay, alpha, *colMatOutput, 1 - alpha, 0, *colMatOutput);

    alpha = 0.7;
    colMatOutput->copyTo(overlay);
    for (int i = startCol; i < binMatInput->cols; i++) {
        if (startRow[i] == 0) continue;
 		cv::rectangle(overlay, Point(i, startRow[i]), Point(i, binMatInput->rows), colorWhite, CV_FILLED);
    }
    addWeighted(overlay, alpha, *colMatOutput, 1 - alpha, 0, *colMatOutput);

    if (locked && minColumn != -1 && wagons1 > 0 && lockedCounter > 15){
		cv::rectangle(*colMatOutput, Point(minColumn-5, startRow[minColumn]), Point(minColumn+5, binMatInput->rows), colorRed, CV_FILLED);
	}

    cv::rectangle(*colMatOutput, Point(startCol, min-2), Point(endCol, min+2), colorRed, CV_FILLED);
    cv::rectangle(*colMatOutput, Point(startCol, max-2), Point(endCol, max+2), colorGreen, CV_FILLED);
    cv::rectangle(*colMatOutput, Point(startCol, avg-2), Point(endCol, avg+2), colorBlue, CV_FILLED);

    cv::putText(*colMatOutput, string("").append(sWagons.str()), cvPoint((startCol+endCol)/2, 30),
				FONT_HERSHEY_COMPLEX_SMALL, 0.90, colorWhite, 1, CV_AA);

    cv::putText(*colMatOutput, state1.str(), cvPoint(5, 35),
				FONT_HERSHEY_COMPLEX_SMALL, 0.5, colorWhite, 1, CV_AA);
    cv::putText(*colMatOutput, state2.str(), cvPoint(5, 50),
    				FONT_HERSHEY_COMPLEX_SMALL, 0.5, colorWhite, 1, CV_AA);
    cv::putText(*colMatOutput, state3.str(), cvPoint(5, 65),
    				FONT_HERSHEY_COMPLEX_SMALL, 0.5, colorWhite, 1, CV_AA);
    cv::putText(*colMatOutput, state4.str(), cvPoint(5, 80),
    				FONT_HERSHEY_COMPLEX_SMALL, 0.5, colorWhite, 1, CV_AA);



}

void WagonCounter::preCalculation(Mat *binMatInput){
    columnCount = new int[binMatInput->cols];
    startRow = new int[binMatInput->cols];
	for (int col = 0; col < binMatInput->cols; col++) {
		columnCount[col] = countNonZero(binMatInput->col(col));
        
        startRow[col] = 0;
        for (int row = 0; row < binMatInput->rows; row++) {
            
            if (startRow[col] > 0) continue;
            
            if (binMatInput->at<bool>(row, col) == 1) {
                startRow[col] = row;
            }
                
        }
	}
    min = getMin(startRow, startCol, endCol);
    max = getMax(startRow, startCol, endCol);
    minColumn = getMinCol(startRow, startCol, endCol);
    avg = getAvg(startRow, startCol, endCol);
    diffMM = max - min;
}

int WagonCounter::getMin(int* columnCount, int from, int to){
    int min = 99999;
    for (int i = from; i < to; i++) {
        min = min < columnCount[i] ? min : columnCount[i];
    }
    return min;
}

int WagonCounter::getMax(int* columnCount, int from, int to){
    int max = 0;
    for (int i = from; i < to; i++) {
        max = max > columnCount[i] ? max : columnCount[i];
    }
    return max;
}

int WagonCounter::getMinCol(int* columnCount, int from, int to){
    int max = 0;
    int col = -1;
    for (int i = from; i < to; i++) {
    	if (max < columnCount[i]){
			max = columnCount[i];
			col = i;
    	}
    }
    return col;
}

int WagonCounter::getAvg(int* columnCount, int from, int to){
    
    int avg = 0;
    for (int i = from; i < to; i++) {
        avg += columnCount[i] ;
    }
    
    return (avg / (to-from));
}

void WagonCounter::approach1(int diff){

    if (locked){
        lockedCounter--;
        if (lockedCounter <= 0){
            locked = false;
        }
        return;
    }
    
    if (diff > 70){
        if (!hasStarted){
            cout << " -----------START---------" << endl;
            hasStarted = true;
            startLock();
            return;
        }
        cout << " -----------VAGON---------" << endl;
        wagons1 ++;
        startLock();
    }else{
    	minColumn = -1;
    }
}


void WagonCounter::startLock(){
    locked = true;
    lockedCounter = 20;
}
