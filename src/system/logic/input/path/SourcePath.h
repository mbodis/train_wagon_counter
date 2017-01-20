/*
 * SourcePath.h
 *
 *  Created on: Jul 14, 2016
 *      Author: mbodis
 */

#ifndef SRC_LOGIC_FRAME_PROCESSING_SOURCEPATH_H_
#define SRC_LOGIC_FRAME_PROCESSING_SOURCEPATH_H_

#include <iostream>

using namespace std;

class SourcePath{
public:

	string path;

	SourcePath(string pathInput){
            this->path = pathInput;
    }

	virtual int getSourceType(){ throw "Use subclass"; };
	virtual ~SourcePath(){ throw "Use subclass"; };
};


#endif /* SRC_LOGIC_FRAME_PROCESSING_SOURCEPATH_H_ */
