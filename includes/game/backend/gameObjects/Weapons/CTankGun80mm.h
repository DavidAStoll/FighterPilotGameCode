/*
 ============================================================================
 Name		: TankGun80mm.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CTankGun80mm declaration
 ============================================================================
 */

#ifndef TANKGUN80MM_H
#define TANKGUN80MM_H

// INCLUDES
#include "includes/game/backend/basicClasses/CWeapon.h"

// CLASS DECLARATION

/**
 *  CTankGun80mm
 * 
 */
class CTankGun80mm: public CWeapon
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CTankGun80mm();

	static CTankGun80mm* New(TBool aDisabled, TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffSet, TInt aYOffSet, TBool* aReflected, TConflictSide* aConflictSide);
	virtual void Shoot();

private:

	CTankGun80mm(TBool aDisabled, TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffSet, TInt aYOffSet, TBool* aReflected, TConflictSide* aConflictSide);
};

#endif // TANKGUN80MM_H
