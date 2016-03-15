/*
 ============================================================================
 Name		: MobileAAGun.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CMobileAAGun declaration
 ============================================================================
 */

#ifndef MOBILEAAGUN_H
#define MOBILEAAGUN_H

// INCLUDES
#include "includes/game/backend/basicClasses/CMobileRotatingWeapon.h"
#include "includes/game/backend/gameObjects/AI/CBackCannonAI.h"

// CLASS DECLARATION

/**
 *  CMobileAAGun
 * 
 */
class CMobileAAGun : public CMobileRotatingWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CMobileAAGun();
	
	virtual void Update();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:
	
	CBackCannonAI* iBackCannonAI;//controls the cannon

	/**
	 * Constructor for performing 1st stage construction
	 */
	CMobileAAGun(
			TPointIntFloat* aLocation,TWeaponIdentifier aWeaponId, TInt* aAngel, TInt aNormalPositionAngel,
			TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aFramesBetweenFire, TInt aMinAngle,
			TInt aMaxAngle, CMoveableGameObject* aMoveableObject, TInt aGunRotationSpeed, TInt aProjectileSpeed, TInt aMinRange, 
			TInt aMaxRange);

	void SaveMobileAAGunOnDisk(CFileWriteStream &aOutputStream);
	void LoadMobileAAGunFromDisk(CFileReadStream &aReadStream);

	};

#endif // MOBILEAAGUN_H
