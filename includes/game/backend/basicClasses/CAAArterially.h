/*
 ============================================================================
 Name		: Arterially.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CArterially declaration
 ============================================================================
 */

#ifndef AAARTERIALLY_H
#define AAARTERIALLY_H

// INCLUDES
#include "includes/game/backend/basicClasses/CBuilding.h"
#include "includes/game/backend/gameObjects/AI/CAAArteriallyAI.h"

// CLASS DECLARATION

/**
 *  CAAArterially
 * 
 */
class CAAArterially : public CBuilding
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CAAArterially();
	
	void ChangeGunAngel(TInt aChange);
	TInt GetGunRotationSpeed();
	TInt GetCurrentGunAngel();
	TInt GetRange();
	TInt GetProjectileSpeed();
	TInt GetMinRange();
	TInt GetGunMinAngel();
	TPointIntFloat&  GetGunCoordinates();
	TInt GetProjectileExplosionWidth();//returns 0 if the projectile doesn't explode
	
	virtual void RepositionGun();
	virtual void Shoot(TInt aRange);
	void AAArteriallyObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
	void AAArteriallyObjectLoadContentFromDisk(CFileReadStream &aReadStream);

	

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CAAArterially(TPoint aLocation,SGameObjectAttributes& aAttributes,TInt aRange,TInt aMinRange,TInt aMinAngel,TInt aGunRotationSpeed,TInt aProjectileSpeed,TInt aProjectileExplosionWidth);
	//the range of the Weapon it can engage enemies
	TInt iRange;
	TInt iMinRange;
	TInt iGunRotationSpeed;
	//these twp values are used for Aiming
	TPointIntFloat iGunCoordinates;
	TInt iGunAngel;
	TInt iMinAngel;// the Minimum angel the gun must have, can't go below this angel
	TInt iProjectileSpeed;
	TInt iProjectileExplosionWidth;
	CAAArteriallyAI* iAI;
	

	};

#endif // AAAAArterially_H
