/*
 ============================================================================
 Name		: GunAA88mm.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CGunAA88mm declaration
 ============================================================================
 */

#ifndef GUNAA88MM_H
#define GUNAA88MM_H

#include "includes/game/backend/basicClasses/CWeapon.h"

// CLASS DECLARATION

/**
 *  CGunAA88mm
 * 
 */
class CGunAA88mm : public CWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGunAA88mm();

	/**
	 * Two-phased constructor.
	 */
	static CGunAA88mm* New(TPointIntFloat* aLocation,TInt* aAngel,TInt aXOffSet,TInt aYOffSet,TBool* aReflected,TConflictSide* aConflictSide);
	virtual void Shoot(TInt aRange);
	virtual void Shoot(TInt aRange,TInt aAngleOffset);
	
protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CGunAA88mm(TPointIntFloat* aLocation,TInt* aAngel,TInt aXOffSet,TInt aYOffSet,TBool* aReflected,TConflictSide* aConflictSide);
	};

#endif // GUNAA88MM_H
