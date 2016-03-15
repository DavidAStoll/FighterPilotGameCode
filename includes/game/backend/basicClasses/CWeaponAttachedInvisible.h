/*
 * CWeaponAttachedInvisible.h
 *
 *  Created on: May 3, 2013
 *      Author: dstoll
 */

#ifndef CWEAPONATTACHEDINVISIBLE_H_
#define CWEAPONATTACHEDINVISIBLE_H_

#include "includes/game/backend/basicClasses/CWeaponAttached.h"

class CWeaponAttachedInvisible : public CWeaponAttached
{
public:
	virtual ~CWeaponAttachedInvisible();
	virtual void Draw();// some weapons might want to draw them self
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);


protected:
	CWeaponAttachedInvisible(TInt aAmmunition, TWeaponIdentifier aWeaponIdentifier, TBool aHasLimitedAmmunition, TPointIntFloat* aLocation, TInt* aAngel, TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TInt aFramesBetweenFire, TBool aDoNotDraw, TIntFloat* aSpeed,
													 TInt aFramesForFireAnimation);
	void WeaponAttachedInvisibleObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
	void WeaponAttachedInvisibleObjectLoadContentFromDisk(CFileReadStream &aReadStream);
	TInt iFrameInFireAnimation;
	TInt iTotalFramesInFireAnimation;
};

#endif /* CWEAPONATTACHEDINVISIBLE_H_ */
