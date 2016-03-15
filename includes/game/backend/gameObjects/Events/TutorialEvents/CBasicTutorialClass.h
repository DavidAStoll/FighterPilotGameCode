/*
 ============================================================================
 Name		: BasicTutorialClass.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CBasicTutorialClass declaration
 ============================================================================
 */

#ifndef BASICTUTORIALCLASS_H
#define BASICTUTORIALCLASS_H

// INCLUDES
#include "includes/game/backend/basicClasses/CEvent.h"
#include "includes/game/backend/basicClasses/CBasicTutorialMap.h"

// CLASS DECLARATION

/**
 *  CBasicTutorialClass
 * 
 */
class CBasicTutorialClass : public CEvent
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBasicTutorialClass();
	CBasicTutorialClass(TEventIdentifier aIdentifier, TInt aSecsUntilMessageCall, TTutorialMethodCallIds aMethodCall);
	virtual TBool CheckIfEventConditionIsMet(); //checks if the Event Condition has been met
	virtual TBool TutorialEventLogic() = 0;//the implementing child class should have its logic in here
	void BasicTutorialSaveOnDisk(CFileWriteStream &aOutputStream);
	void BasicTutorialLoadFromDisk(CFileReadStream &aReadStream);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);


protected:
	
	TBool iReadyToMakeMethodCall;
	TInt iCurrentFrame;
	TInt iFramesUntilMethodCall;
	TTutorialMethodCallIds iMethodCall;


	};

#endif // BASICTUTORIALCLASS_H
