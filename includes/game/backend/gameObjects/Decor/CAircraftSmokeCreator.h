/*
 * CAircraftSmoke.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CAIRCRAFTSMOKE_H_
#define CAIRCRAFTSMOKE_H_

#include "includes/game/backend/basicClasses/CWeapon.h"

// CLASS DECLARATION

/**
 *  CAircraftSmoke
 *
 */
class CAircraftSmokeCreator : public CWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAircraftSmokeCreator();

	static CAircraftSmokeCreator* New(TBool aBehindObject, CAircraft* aOwner, TInt aXOffset, TInt aYOffset, TInt aFramesUntilSmokeDisappears);
	virtual void Update();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);


protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CAircraftSmokeCreator(TBool aBehindObject, CAircraft* aOwner, TInt aXOffset, TInt aYOffset, TInt aFramesUntilSmokeDisappears);

	void Construct();

	TBool iBehindObject;
	TInt iFramesUntilSmokeDisappears;
	CAircraft* iOwner;
	TColour iCrashedColour;
	TColour iSmokeColour;
	TInt iHealthOfPlaneLeft;

	};

#endif /* CAIRCRAFTSMOKE_H_ */
