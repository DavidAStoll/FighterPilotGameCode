/*
 * CProjectile.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CPROJECTILE_H_
#define CPROJECTILE_H_

#include "includes/game/backend/basicClasses/CMoveableGameObject.h"
#include "includes/core/sound/CSoundObject.h"

// CLASS DECLARATION

/**
 *  CProjectile
 *
 */

class CProjectile: public CMoveableGameObject
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CProjectile();

	virtual void Update(); //runs the Game Object's logic code to update its member data
	virtual void Destruct(); //called when range is less than 0,might cause an explosion or just remove the object
	virtual void Die(TDamageType aDamageType);
	virtual void Move();
	void SetPixelsPerMoveX(TIntFloat aValue);
	void SetPixelsPerMoveY(TIntFloat aValue);
	void ProjectileObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
	void ProjectileObjectLoadContentFromDisk(CFileReadStream &aReadStream);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	TInt iRange;//how long the projectile can fly
	/**
	 * Constructor for performing 1st stage construction
	 */
	CProjectile(CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer, TPoint aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aRange, TInt aAngle);
	//for hight precision
	CProjectile(CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer, TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aRange, TInt aAngle);
	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	void AdjustCoordinatesAndTexture();
};


#endif /* CPROJECTILE_H_ */
