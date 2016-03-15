/*
 * CWeaponAttached.h
 *
 *  Created on: May 3, 2013
 *      Author: dstoll
 */

#ifndef CWEAPONATTACHED_H_
#define CWEAPONATTACHED_H_

#include "includes/game/backend/basicClasses/CWeapon.h"

class CWeaponAttached: public CWeapon
{
public:

	virtual void Draw();// some weapons might want to draw them self
	virtual void Update();//some weapons might need an update function
	virtual TIntFloat GetXMiddlePointOfWeapon();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

	virtual ~CWeaponAttached();

protected:

	CWeaponAttached(TInt aAmmunition, TWeaponIdentifier aWeaponIdentifier, TBool aHasLimitedAmmunition, const TPointIntFloat* aLocation, const TInt* aAngel, TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide, TInt aFramesBetweenFire, TBool aDoNotDraw, const TIntFloat* aSpeed);
	void WeaponAttachedObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
	void WeaponAttachedObjectLoadContentFromDisk(CFileReadStream &aReadStream);

	CTextureObject* iTextureObject;
	CAnimationPlayer* iAnimationPlayer;
	const TIntFloat* iSpeed;
	TBool iDoNotDraw;
};

#endif /* CWEAPONATTACHED_H_ */
