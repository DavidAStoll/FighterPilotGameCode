/*
 ============================================================================
 Name		: WeaponBomb500Kg.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWeaponBomb500Kg declaration
 ============================================================================
 */

#ifndef WEAPONBOMB500KG_H
#define WEAPONBOMB500KG_H


#include "includes/game/backend/basicClasses/CWeaponBomb.h"

// CLASS DECLARATION

/**
 *  CWeaponBomb500Kg
 * 
 */
class CWeaponBomb500Kg : public CWeaponBomb
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponBomb500Kg();

        /**
         * Two-phased constructor.
         */
        static CWeaponBomb500Kg* New(TInt aAmmunition,TPointIntFloat* aLocation,TInt* aAngel,TBool* aUpSideDown,TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide,
            TBool aDoNotDraw,TIntFloat* aObjectSpeed);
        virtual void Shoot();
        virtual void Explode();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
  CWeaponBomb500Kg(TInt aAmmunition,TPointIntFloat* aLocation,TInt* aAngel,TBool* aUpSideDown,TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide,
              TBool aDoNotDraw,TIntFloat* aObjectSpeed);
	void ConstructL();

	};

#endif // WEAPONBOMB500KG_H
