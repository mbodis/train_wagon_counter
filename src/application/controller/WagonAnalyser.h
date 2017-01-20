
#ifndef WAGONANALYSER_H
#define WAGONANALYSER_H


#include "DebugFrames.h"
#include "WagonCounter.h"
#include "../../system/logic/input/keyboard/InputFacade.h"
#include "../../system/features/BinaryFilter.h"
#include "../config/ConfigExample.h"

/*
 * custom application logic 
 */
class WagonAnalyser{
    
private:
    
    int v_ts = 0;
    bool resiseInput = true;

    double resizeRatio = 3;
    int resizedWidth = -1;
    int resizedHeight = -1;
    
    // BGR original frame
    Mat colorFrame, colorFrame2;
    // grayscale frame
    Mat grayFrame;
    
    // static background img
    Mat staticBgImg;
    // difference between (staticBgImg and input grayscale) , two last input grayscale images
    Mat diffBg, diffLast;
    
    // or logic on diffBg and diffLast
    Mat orMat;
    
    // closing on binaryImage
    Mat morphBg, morphLast, morphOr;
    
    // last grayscale frame
    Mat lastGrayFrame;
    
    Mat filtered;

    DebugFrames* mDebugFrames;
    BinaryFilter* mBinaryFilter;
    WagonCounter* mWagonCounter;
    
public:
    
    WagonAnalyser(DebugFrames*);
    
    void analyse(Mat, int);
    
    // GUI trackball slider
    const int slider_max = 255;
    int sliderBg = 75;
    int slidersLast = 6;
    void initTrackball();
    
    void initBackground();
    
    Mat* getColorFrame();
	Mat* getGrayFrame();

    void setFrame(Mat, int);
    void showImg(Mat, String);
    bool keyaboardInput();
    
    //logic
    
    void saveInputFrame(Mat, int);
    void compateTwoBinFrames(Mat*, Mat*, Mat*, int);
    void saveLastFrame();
    void morphOperation(Mat*, Mat*);
    void logicOr();
    void countWagons(Mat*, Mat*);
    void imageFiltering(Mat*, Mat*);
    
    void showOutput();

    // this is not necessary
    static void on_trackbar( int val, void* ){
        //cout<< "v: "<< val << endl;
    }
};


#endif //WAGONANALYSER_H
