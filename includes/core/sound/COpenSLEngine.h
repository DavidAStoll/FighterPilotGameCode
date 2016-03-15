/*
 * COpenSLEngine.h
 *
 *  Created on: May 22, 2012
 *      Author: dstoll
 */

//apprently only one OpenSL engine can exists at a time, thus we need this method to share them with various OpenSL clients

#ifndef COPENSLENGINE_H_
#define COPENSLENGINE_H_

#include <SLES/OpenSLES.h>

class COpenSLEngine
{
public:

	//singelton method
	static SLObjectItf GetOpenSLEngine();

private:
	static SLObjectItf EngineObject;
};

#endif /* COPENSLENGINE_H_ */
