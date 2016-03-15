/*
 * CWeaponBomb.h
 *
 *  Created on: Aug 27, 2012
 *      Author: dstoll
 */

#ifndef CWEAPONBOMB_H_
#define CWEAPONBOMB_H_

#include "includes/game/backend/basicClasses/CWeaponAttached.h"

// CLASS DECLARATION

/**
 *  CWeaponBomb
 *
 */
class CWeaponBomb: public CWeaponAttached
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CWeaponBomb();

	virtual void Draw();// some weapons might want to draw them self
	virtual void Explode();//this method might be called to let the weapon explode if enough ammunition is there, must be implemented by child
	void WeaponBombObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
	void WeaponBombObjectLoadContentFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CWeaponBomb(TInt aAmmunition, TWeaponIdentifier aWeaponIdentifier, const TPointIntFloat* aLocation, const TInt* aAngel, const TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aFramesBetweenFire, TBool aDoNotDraw, const TIntFloat* aSpeed);

	const TBool* iIsUpSideDown;//some weapons like bombs, can't fire if the plane is upside down
};


#endif /* CWEAPONBOMB_H_ */
