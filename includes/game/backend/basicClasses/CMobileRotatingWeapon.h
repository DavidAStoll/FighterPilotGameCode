/*
 ============================================================================
 Name		: MobileRotatingWeapon.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CMobileRotatingWeapon declaration
 ============================================================================
 */

#ifndef MOBILEROTATINGWEAPON_H
#define MOBILEROTATINGWEAPON_H

// INCLUDES
#include "includes/game/backend/basicClasses/CWeaponBomb.h"
#include "includes/game/backend/basicClasses/CMoveableGameObject.h"

// CLASS DECLARATION

/**
 *  CMobileRotatingWeapon
 * 
 */
class CMobileRotatingWeapon : public CWeaponBomb
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CMobileRotatingWeapon();



	TInt GetMoveableObjectAngle();
	TInt GetCurrentGunAngle();
	const TConflictSide& GetConflictSide();
	virtual void Draw();// some weapons might want to draw them self
	virtual void Update();
	TInt GetMaxRange();
	TInt GetMinRange();
	TInt GetProjectileSpeed();
	TInt GetGunRotationSpeed();
	TInt GetMinAngle();
	TInt GetMaxAngle();
	CMoveableGameObject* GetMoveableObject();
	TBool GetGunIsReflected();
	virtual void ChangeGunAngle(TInt aAngleChange);
	void SaveMobileRotatingWeaponOnDisk(CFileWriteStream &aOutputStream);
	void LoadMobileRotatingWeaponFromDisk(CFileReadStream &aReadStream);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CMobileRotatingWeapon(
												TPointIntFloat* aLocation,TWeaponIdentifier aWeaponId, TInt* aAngel, TInt aNormalPositionAngel,
												TInt aXOffset, TInt aYOffset, TBool* aReflected, TConflictSide* aConflictSide, TIntFloat* aSpeed, TInt aFramesBetweenFire, TInt aMinAngle,
												TInt aMaxAngle, CMoveableGameObject* aMoveableObject, TInt aGunRotationSpeed, TInt aProjectileSpeed, TInt aMinRange, 
												TInt aMaxRange );
	
	TInt iGunRotationSpeed;
	TInt iMinAngle; //need to be specified by owner
	TInt iMaxAngle; //need to be specified by owner
	TInt iProjectileSpeed;
	TInt iMaxRange;
	TInt iMinRange;
	TInt iMinPositionAngle;
	CMoveableGameObject* iMoveableObject;//needed to know how fast and in what direction the plane is moving
	TInt iDefaultOffsetAngleFromPlanePosition; // is the normal difference between the guns orientation and the front of the plane, depends on where the gun is positioned
	TInt iOffSetAngleFromNormalPosition;// if the gun rotates itself, this angel will keep track oh much it rotates
	TInt iRealAngle;//after doing all adjustments this will keep track of the actual angel of the gun
	TPointIntFloat iWeaponCoordinate;
	TBool iIsUpSideDownDummy;
	};

#endif // MOBILEROTATINGWEAPON_H
