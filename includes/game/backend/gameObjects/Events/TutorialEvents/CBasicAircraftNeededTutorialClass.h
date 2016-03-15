/*
 ============================================================================
 Name		: BasicAircraftNeededTutorialClass.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CBasicAircraftNeededTutorialClass declaration
 ============================================================================
 
 The whole purpose of this class is to an aircraft pointer and handling the saving and loading for this pointer, since a lot of tutorials depend
 on the user aircraft. Doing it this way, I don't need to repeat this code all over the place
 */

#ifndef BASICAIRCRAFTNEEDEDTUTORIALCLASS_H
#define BASICAIRCRAFTNEEDEDTUTORIALCLASS_H

// INCLUDES
#include "includes/game/backend/gameObjects/Events/TutorialEvents/CBasicTutorialClass.h"
#include "includes/game/backend/basicClasses/CAircraft.h"

// CLASS DECLARATION


/**
 *  CBasicAircraftNeededTutorialClass
 * 
 */
class CBasicAircraftNeededTutorialClass : public CBasicTutorialClass
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBasicAircraftNeededTutorialClass();
	CBasicAircraftNeededTutorialClass(TEventIdentifier EEventIdentifierSpeedUpTutorial, CAircraft* aAircraft, TInt aSecsOfCallDelay, TTutorialMethodCallIds aMethodCallIfSuccess);
	void BasicAircraftNeededTutorialSaveOnDisk(CFileWriteStream &aOutputStream);
	void BasicAircraftNeededTutorialLoadFromDisk(CFileReadStream &aReadStream);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	
protected:

	CAircraft* iAircraft;

	};

#endif // BASICAIRCRAFTNEEDEDTUTORIALCLASS_H
