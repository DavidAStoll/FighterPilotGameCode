/*
 * CWinIfAllJapaneseMoveableObjectsAreDestroyed.h
 *
 *  Created on: Nov 9, 2012
 *      Author: dstoll
 */

#ifndef CWINIFALLJAPANESEMOVEABLEOBJECTSAREDESTROYED_H_
#define CWINIFALLJAPANESEMOVEABLEOBJECTSAREDESTROYED_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"

class CWinIfAllJapaneseMoveableObjectsAreDestroyed : public CEvent
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CWinIfAllJapaneseMoveableObjectsAreDestroyed();
	CWinIfAllJapaneseMoveableObjectsAreDestroyed(TInt aBonusScore);


	virtual void TriggerEvent(); //calls the method the event will cause, should only be called if the event's conditions have been met
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	TInt iBonusScore;
};

#endif /* CWINIFALLJAPANESEMOVEABLEOBJECTSAREDESTROYED_H_ */
