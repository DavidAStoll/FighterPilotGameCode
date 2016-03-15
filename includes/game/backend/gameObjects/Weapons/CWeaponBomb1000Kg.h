/*
 * CWeaponBomb1000Kg.h
 *
 *  Created on: Aug 28, 2012
 *      Author: dstoll
 */

#ifndef CWEAPONBOMB1000KG_H_
#define CWEAPONBOMB1000KG_H_

#include "includes/game/backend/basicClasses/CWeaponBomb.h"

// CLASS DECLARATION

/**
 *  CWeaponBomb1000Kg
 *
 */
class CWeaponBomb1000Kg : public CWeaponBomb
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CWeaponBomb1000Kg();

	/**
	 * Two-phased constructor.
	 */
	static CWeaponBomb1000Kg* New(TInt aAmmunition,TPointIntFloat* aLocation,TInt* aAngel,TBool* aUpSideDown,TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide,
	    TBool aDoNotDraw,TIntFloat* aObjectSpeed);
	virtual void Shoot();
	virtual void Explode();



protected:


	CWeaponBomb1000Kg(TInt aAmmunition,TPointIntFloat* aLocation,TInt* aAngel,TBool* aUpSideDown,TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide,
            TBool aDoNotDraw,TIntFloat* aObjectSpeed);
	void ConstructL();

	};

#endif /* CWEAPONBOMB1000KG_H_ */
