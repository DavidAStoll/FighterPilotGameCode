/*
 ============================================================================
 Name		: GrenadeLauncher.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGrenadeLauncher declaration
 ============================================================================
 */

#ifndef GRENADELAUNCHER_H
#define GRENADELAUNCHER_H

// INCLUDES
#include "includes/game/backend/basicClasses/CWeapon.h"

// CLASS DECLARATION

/**
 *  CGrenadeLauncher
 * 
 */
class CGrenadeLauncher : public CWeapon
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGrenadeLauncher();

	/**
	 * Two-phased constructor.
	 */
	static CGrenadeLauncher* New(TBool aDisabled,TPointIntFloat* aLocation,TInt* aAngel,TInt aXOffSet,TInt aYOffSet,TBool* aReflected,TConflictSide* aConflictSide);
	virtual void Shoot();
	
private:

	CGrenadeLauncher(TBool aDisabled,TPointIntFloat* aLocation,TInt* aAngel,TInt aXOffSet,TInt aYOffSet,TBool* aReflected,TConflictSide* aConflictSide);
	};

#endif // GRENADELAUNCHER_H
