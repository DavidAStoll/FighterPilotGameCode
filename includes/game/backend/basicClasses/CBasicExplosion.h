/*
 * CBasicExplosion.h
 *
 *  Created on: Aug 28, 2012
 *      Author: dstoll
 */

#ifndef CBASICEXPLOSION_H_
#define CBASICEXPLOSION_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CGameObject.h"
#include "includes/game/CFighterPilotThePacificWar.h"

#define BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES (GAMEOBJECT_TYPE_GROUP_BUILDINGS | GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_SHIP | EObjectTypesDestructibleDecor)
//need a specific ones, otherwise we can have a change reaction of tanks blowing up
#define BASIC_EXPLOSION_DEFAULT_OBJECT_TYPES_IT_DAMAGES_FOR_EXPLODING_TANKS (GAMEOBJECT_TYPE_GROUP_BUILDINGS | GAMEOBJECT_TYPE_GROUP_PLANES | EObjectTypesGroundUnit | GAMEOBJECT_TYPE_GROUP_SHIP | EObjectTypesDestructibleDecor)


struct SExplosionAttributes
{
	TInt Damage;
	TInt ArmorPenetration;
	TInt FramesForExplosion;
	TBool KillOnlyDuringFirstFrame;
	TUint ObjectTypesItCanDamage;
};

// CLASS DECLARATION

/**
 *  CBasicExplosion
 *
 */
class CBasicExplosion : public CGameObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CBasicExplosion();
	CBasicExplosion(const TPoint& aLocation, SGameObjectAttributes& aAttributes,	SExplosionAttributes& aExplosionAttributes);
	CBasicExplosion(const TPointIntFloat& aLocation, SGameObjectAttributes& aAttributes,	SExplosionAttributes& aExplosionAttributes);
	virtual void Update();
	virtual void PossibleCollision(CInterval*& aInterval);
	TBool ObjectHasNotBeenDamagedBefore(CGameObject* aGameObjects);
	void BasicExplosionSaveContentToDisk(CFileWriteStream &aOutputStream);
	void BasicExplosionLoadContentFromDisk(CFileReadStream &aReadStream);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:
	TInt iCurrentFrame;
	TInt iCurrentCollisionCheckFrame;
	TInt iDamage;
	TInt iArmorPenetration;
	TInt iFramesForExplosion;
	TUint iObjectTypesItCanDamage; //some might not be able to damage all
	TBool iFirstCall;
	TBool iOnlyKillDuringFirstFrame;
	CPointerArray<CGameObject>* iObjectsAlreadyDamaged; //keeps track of what objects this explosion already damage, so it doesn't count twice. Might happens if on edge of an interval

	};

#endif /* CBASICEXPLOSION_H_ */
