/*
 ============================================================================
 Name		: Explosive7InchGun.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CExplosive7InchGun declaration
 ============================================================================
 */

#ifndef EXPLOSIVE7INCHGUN_H
#define EXPLOSIVE7INCHGUN_H

// INCLUDES
#include "includes/game/backend/basicClasses/CWeapon.h"

// CLASS DECLARATION

/**
 *  CExplosive7InchGun
 * 
 */
class CExplosive7InchGun : public CWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosive7InchGun();

	/**
	 * Two-phased constructor.
	 */
	static CExplosive7InchGun* New(TBool aDisabled, const TPointIntFloat* aLocation, const TInt* aAngel,TInt aXOffSet,TInt aYOffSet, const TBool* aReflected, const TConflictSide* aConflictSide);
	virtual void ShootWithSpeed(TIntFloat aSpeed);
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CExplosive7InchGun(TBool aDisabled, const TPointIntFloat* aLocation, const TInt* aAngel,TInt aXOffSet,TInt aYOffSet, const TBool* aReflected, const TConflictSide* aConflictSide);
	};

#endif // EXPLOSIVE7INCHGUN_H
