/*
 ============================================================================
 Name		: Rifle.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright HQ Interactive Inc.
 Description : CRifle declaration
 ============================================================================
 */

#ifndef Rifle_H
#define Rifle_H

// INCLUDES
#include "includes/game/backend/basicClasses/CWeapon.h"
// CLASS DECLARATION

/**
 *  CRifle
 * 
 */
class CRifle : public CWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CRifle();

        static CRifle* New(TBool aDisabled,TPointIntFloat* aLocation,TInt* aAngel,TInt aXOffSet,TInt aYOffSet,TBool* aReflected,TConflictSide* aConflictSide);
        virtual void Shoot();

private:

        CRifle(TBool aDisabled,TPointIntFloat* aLocation,TInt* aAngel,TInt aXOffSet,TInt aYOffSet,TBool* aReflected,TConflictSide* aConflictSide);


	};

#endif // Rifle_H
