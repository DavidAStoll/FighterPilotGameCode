/*
 ============================================================================
 Name		: WeaponTorpedo.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWeaponTorpedo declaration
 ============================================================================
 */

#ifndef WEAPONTORPEDO_H
#define WEAPONTORPEDO_H


#include "includes/game/backend/basicClasses/CWeaponBomb.h"

// CLASS DECLARATION

/**
 *  CWeaponTorpedo
 * 
 */
class CWeaponTorpedo : public CWeaponBomb
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponTorpedo();

	/**
	 * Two-phased constructor.
	 */
	static CWeaponTorpedo* New(TInt aAmmunition,TPointIntFloat* aLocation,TInt* aAngel,TBool* aUpSideDown,TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide,
            TBool aDoNotDraw,TIntFloat* aObjectSpeed);
        virtual void Shoot();
        virtual void Explode();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CWeaponTorpedo(TInt aAmmunition,TPointIntFloat* aLocation,TInt* aAngel,TBool* aUpSideDown,TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide,
            TBool aDoNotDraw,TIntFloat* aObjectSpeed);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	};

#endif // WEAPONTORPEDO_H
