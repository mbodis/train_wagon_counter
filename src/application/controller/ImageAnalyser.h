
#ifndef IMAGEANALYSER_H
#define IMAGEANALYSER_H


#include "DebugFrames.h"
#include "WagonAnalyser.h"
#include "../../system/logic/input/keyboard/InputFacade.h"
#include "../config/ConfigExample.h"

/*
 * main logic 
 */
class ImageAnalyser {
private:
    int inputMode, printMode;
    
    DebugFrames* mDebugFrames;
    WagonAnalyser* mWagonAnalyser;
    InputFacade* facade;
    
public:
    ImageAnalyser(ConfigExample *config, int inputMode, int printMode, InputFacade *facade);

    bool analyse(Mat, int videoTime);
    bool keyaboardInput(Mat*);

};


#endif //IMAGEANALYSER_H
