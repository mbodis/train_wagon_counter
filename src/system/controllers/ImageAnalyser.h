
#ifndef IMAGEANALYSER_H
#define IMAGEANALYSER_H


#include "DebugFrames.h"


/*
 * main logic 
 */
class ImageAnalyser {
private:
    int inputMode, printMode;
    
public:
    DebugFrames* mDebugFrames;
    ImageAnalyser(ConfigExample *config, int inputMode, int printMode);

    bool analyse(Mat, int videoTime);
    bool keyaboardInput(Mat*);
    void initCustom();
    virtual void executeCustomLogic(Mat, int);

};


#endif //IMAGEANALYSER_H
