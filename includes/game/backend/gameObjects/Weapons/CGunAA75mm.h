/*
 ============================================================================
 Name		: GunAA75mm.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CGunAA75mm declaration
 ============================================================================
 */

#ifndef GUNAA75MM_H
#define GUNAA75MM_H

// CLASS DECLARATION
#include "includes/game/backend/basicClasses/CWeapon.h"

/**
 *  CGunAA75mm
 * 
 */
class CGunAA75mm : public CWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGunAA75mm();

	/**
	 * Two-phased constructor.
	 */
	static CGunAA75mm* New(const TPointIntFloat* aLocation, const TInt* aAngel,TInt aXOffSet,TInt aYOffSet, const TBool* aReflected, const TConflictSide* aConflictSide);
	virtual void Shoot(TInt aRange);
	virtual void Shoot(TInt aRange,TInt aAngleOffset);

protected:

	CGunAA75mm(const TPointIntFloat* aLocation,const TInt* aAngel,TInt aXOffSet,TInt aYOffSet, const TBool* aReflected, const TConflictSide* aConflictSide);
	};

#endif // GUNAA75MM_H
