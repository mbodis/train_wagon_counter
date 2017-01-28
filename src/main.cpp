/*
 *
 *
 * setup: set following
 *      EXAMPLE_CONFIG - json file required
 *      PRINT_MODE - debug/presentation
 *      INPUT_MODE - img/folder/video/url
 *
 */
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

#include "application/config/ConfigExample.h"
#include "system/logic/input/ProcessingFacade.h"
#include "application/controllers/WagonAnalyser.h"

/*
 * reload application constants
 */
static char* EXAMPLE_CONFIG = "src/application/config/json/example1.json";

/*
 * setup output mode
 * [ PRINT_MODE_DEBUG, PRINT_MODE_PRESENTATION ]
 */
static int PRINT_MODE = PRINT_MODE_DEBUG;

/*
 * setup input mode
 * [INPUT_MODE_IMG_FOLDER, INPUT_MODE_VIDEO_RT, INPUT_MODE_VIDEO_FRAME, INPUT_MODE_URL]
 */
static int INPUT_MODE = INPUT_MODE_VIDEO_FRAME;


/*
 * NOTE: do not modify !
 */
int main(int argc, char **argv) {

	ConfigExample mConfigExample(EXAMPLE_CONFIG, INPUT_MODE, PRINT_MODE);
	WagonAnalyser mWagonAnalyser(&mConfigExample, INPUT_MODE, PRINT_MODE);
    ProcessingFacade mProcessingFacade(&mConfigExample, &mWagonAnalyser);
    mProcessingFacade.runAnalyse();

	return 0;
}
