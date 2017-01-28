

#include "../controllers/WagonAnalyser.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../../system/helper/TimeHelper.h"
#include "../controllers/WagonCounter.h"


using namespace cv;
using namespace std;


void WagonAnalyser::executeCustomLogic(Mat frame, int videoTime){
	analyse(frame, videoTime);
}
void WagonAnalyser::initBackground(){
    // read background   
    staticBgImg = imread(mDebugFrames->c->STATIC_BACKGROUND_IMG, 1);
	if (!staticBgImg.data) {
		cout << "Error loading img - staticBgImg path: "<< mDebugFrames->c->STATIC_BACKGROUND_IMG << endl;
	}
    cout << "loading img - staticBgImg path: "<< mDebugFrames->c->STATIC_BACKGROUND_IMG << endl;
    cvtColor(staticBgImg, staticBgImg, CV_BGR2GRAY); 
    if (resiseInput){
        resizedWidth = staticBgImg.cols/resizeRatio;
        resizedHeight = staticBgImg.rows/resizeRatio;
        resize(staticBgImg, staticBgImg, Size(resizedWidth, resizedHeight));
    }
}

void WagonAnalyser::initTrackball(){
    namedWindow("tools", 1);
    createTrackbar( "th background", "tools", &sliderBg, slider_max, on_trackbar);
    createTrackbar( "th last two frames", "tools", &slidersLast, slider_max, on_trackbar);
}

void WagonAnalyser::analyse(Mat frame, int videoTime){

    // save frame
    saveInputFrame(frame, videoTime);
    
	// compare with background
    compateTwoBinFrames(&staticBgImg, &grayFrame, &diffBg, sliderBg);
    // compare with last frame
    compateTwoBinFrames(&lastGrayFrame, &grayFrame, &diffLast, slidersLast);
    
    // merge with or logic
    logicOr();
    
    // do some morph operation
    morphOperation(&diffBg, &morphBg);
    morphOperation(&diffLast, &morphLast);
    morphOperation(&orMat, &morphOr);
    
    // fill holes remove small parts
    imageFiltering(&morphOr, &filtered);
    
    // filter to binary output
    countWagons(&filtered, &colorFrame2);

    // debug
    mDebugFrames->drawStatus(&colorFrame, v_ts);
    
    // show output
    showOutput();
    saveLastFrame();
}

void WagonAnalyser::saveInputFrame(Mat frame, int videoTime){
    if (resizedWidth == -1 || resizedHeight == -1){
        resizedWidth = frame.cols/resizeRatio;
        resizedHeight = frame.rows/resizeRatio;
    }
    
    if (resiseInput){
        resize(frame, frame, Size(resizedWidth, resizedHeight));
    }
    
    this->colorFrame = frame;
    this->colorFrame2 = frame.clone();
    this->v_ts = videoTime;
    cvtColor(colorFrame, grayFrame, CV_BGR2GRAY);
    equalizeHist(grayFrame, grayFrame);
    
    // save img size and setup font size
    mDebugFrames->c->initFontSize(&frame);
}

void WagonAnalyser::compateTwoBinFrames(Mat *binMat1, Mat *binMat2, Mat *binOutMat, int sliderVal){
    if (!binMat1->data || !binMat2->data){
        cout << "compateTwoBinFrames binMat1 or grayFrame is EMPTY !" << endl;   
        return;
    }
    
    absdiff(*binMat1, *binMat2, *binOutMat);
    *binOutMat = *binOutMat > sliderVal;
}

void WagonAnalyser::saveLastFrame(){
    lastGrayFrame = grayFrame.clone();
}

void WagonAnalyser::morphOperation(Mat *binInMat, Mat *binOutMat){
    if (!binInMat->data){
        cout << "morphOperation input Mat is EMPTY !" << endl;
        return;
    }
    
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
    Mat r;
    morphologyEx(*binInMat, *binOutMat, MORPH_OPEN, element);
}

void WagonAnalyser::logicOr(){
    if (!diffBg.data || !diffLast.data){
        cout << "diffBg or diffLast is empty" << endl;
        return;
    }
    
    bitwise_or(diffBg, diffLast, orMat);        
}

void WagonAnalyser::imageFiltering(Mat *inBinMat, Mat *outBinMat){
    if(!inBinMat->data){
        cout << "morphOr is empty" << endl;
        return;
    }
    
    *outBinMat = inBinMat->clone();
    mBinaryFilter->fillHoles (outBinMat, outBinMat, 3.0);
    mBinaryFilter->removeNoise(outBinMat, outBinMat, 2);
    mBinaryFilter->removeNoise(outBinMat, outBinMat, 1);
    mBinaryFilter->removeNoise(outBinMat, outBinMat, 0.5);
    mBinaryFilter->fillVerticalHoles (outBinMat, outBinMat);
}

void WagonAnalyser::countWagons(Mat *frameInput, Mat *frameOutput){
    mWagonCounter->evaluate(frameInput, frameOutput);
}

void WagonAnalyser::showOutput(){
//    this->showImg(colorFrame, "original color");
    this->showImg(colorFrame2, "wagon analyzer");
//    this->showImg(grayFrame, "grayScale");
//    this->showImg(diffBg, "difference with staic");
//    this->showImg(diffLast, "difference with last");
    this->showImg(orMat, "or diff mat");
//    this->showImg(morphBg, "difference with staic - closing");
//    this->showImg(morphLast, "difference with last - closing");
    this->showImg(morphOr, "or diff mat - closing");
    this->showImg(filtered, "fill holes / filtered ");
}

void WagonAnalyser::showImg(Mat frame, String frameName){
	if (frame.dims != 0) {
		imshow(frameName, frame);
	} else {
		cout << frameName << " is EMPTY !" << endl;
	}
}
