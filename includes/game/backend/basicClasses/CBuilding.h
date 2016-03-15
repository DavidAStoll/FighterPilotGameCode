/*
 * CBuilding.h
 *
 *  Created on: Aug 25, 2012
 *      Author: dstoll
 */

#ifndef CBUILDING_H_
#define CBUILDING_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CGameObject.h"

// CLASS DECLARATION

/**
 *  CBuilding
 *
 */
class CBuilding : public CGameObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CBuilding();
	TBool GetBuildingGotCaptured();
	void SetBuildingGotCaptured(TBool aValue);
	void UpdateStatsIfBuildingWasSuccessfullyCaptured();
	//saves the member data of the gameObject
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	void BuildingSaveContentToDisk(CFileWriteStream &aOutputStream);
	void BuildingLoadContentFromDisk(CFileReadStream &aReadStream);

protected:
	virtual void Die(TDamageType aType);
	void BuildingBasicDieRoutine();

	/**
	 * Constructor for performing 1st stage construction
	 */
	CBuilding(TInt aZCor,CTextureObject* aTextureObject,CAnimationPlayer* aAnimationPlayer,//inheritance
            TPoint aLocation,SGameObjectAttributes& aAttributes);

	//returns NULL if it was not captured, otherwise the object that captured it
	CGameObject* CheckIfObjectGotCaptured(CInterval*& aInterval);
	TBool iGotCaptured;

	};

#endif /* CBUILDING_H_ */
