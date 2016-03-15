/*
 ============================================================================
 Name		: GravitationalProjectile.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CGravitationalProjectile declaration
 ============================================================================
 */

#ifndef GRAVITATIONALPROJECTILE_H
#define GRAVITATIONALPROJECTILE_H

// INCLUDES
#include "includes/game/backend/basicClasses/CProjectileBomb.h"

// CLASS DECLARATION

/**
 *  CGravitationalProjectile
 * 
 */
class CGravitationalProjectile : public CProjectileBomb
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CGravitationalProjectile();
	CGravitationalProjectile(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aAngel);
	virtual void Update();
	void GravitationalProjectileSaveContentToDisk(CFileWriteStream &aOutputStream);
	void GravitationalProjectileLoadContentFromDisk(CFileReadStream &aReadStream);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:
	//method should be called after projectile orietiend and ready to go
	void UpdateGravitionalStatus();
	TInt iCurrentFrameInGravitionalStatus;
	TInt iFramesUntilProjecticleGetsShiftedDown;

	};

#endif // GRAVITATIONALPROJECTILE_H
