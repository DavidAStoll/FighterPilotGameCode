/*
 ============================================================================
 Name		: Explosive14InchGun.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosive14InchGun declaration
 ============================================================================
 */

#ifndef EXPLOSIVE14INCHGUN_H
#define EXPLOSIVE14INCHGUN_H

// INCLUDES
#include "includes/game/backend/basicClasses/CWeapon.h"

// CLASS DECLARATION

/**
 *  CExplosive14InchGun
 * 
 */
class CExplosive14InchGun : public CWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosive14InchGun();
	static CExplosive14InchGun* New(TBool aDisabled, const TPointIntFloat* aLocation, const TInt* aAngel,TInt aXOffSet,TInt aYOffSet, const TBool* aReflected, const TConflictSide* aConflictSide);
	virtual void ShootWithSpeed(TIntFloat aSpeed);

private:

	CExplosive14InchGun(TBool aDisabled, const TPointIntFloat* aLocation, const TInt* aAngel,TInt aXOffSet,TInt aYOffSet, const TBool* aReflected, const TConflictSide* aConflictSide);



	};

#endif // EXPLOSIVE14INCHGUN_H
