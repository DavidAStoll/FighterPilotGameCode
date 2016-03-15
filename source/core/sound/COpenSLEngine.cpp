/*
 * COpenSLEngine.cpp
 *
 *  Created on: May 22, 2012
 *      Author: dstoll
 */
#include <assert.h>
#include "includes/core/sound/COpenSLEngine.h"
#include "includes/core/standardLibrary/BasicTypes.h"


SLObjectItf COpenSLEngine::EngineObject = NULL;

SLObjectItf COpenSLEngine::GetOpenSLEngine()
{
	if(COpenSLEngine::EngineObject == NULL) //will only the first time this method is called
	{
		SLresult lResult;
		slCreateEngine(&COpenSLEngine::EngineObject, 0, NULL, 0, NULL, NULL);
		lResult = (*COpenSLEngine::EngineObject)->Realize(COpenSLEngine::EngineObject, SL_BOOLEAN_FALSE);
		assert(SL_RESULT_SUCCESS == lResult);
	}

	return COpenSLEngine::EngineObject;
}
