/*
 * CLandingSurface.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CLANDINGSURFACE_H_
#define CLANDINGSURFACE_H_

#include "includes/game/backend/basicClasses/CGameObject.h"
class CAircraft;

// CLASS DECLARATION

/**
 *  CLandingSurface
 *  //this class is basically  just a hitbox but put into a gameObject so that collision detection and deletion works properly
 *  it will have no TextureObject since it has no texture assocaited with itself
 *
 */
class CLandingSurface : public CGameObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CLandingSurface();

	/**
	 * Two-phased constructor.
	 */
	static CLandingSurface* New(TPoint aLocation,TConflictSide aSide,const TIntFloat& aWidth,const TIntFloat& aHeight);
	static CLandingSurface* New(TPoint aLocation,CTextureObject* aTextureObject,TInt aZValue,TConflictSide aSide,const TIntFloat& aWidth,const TIntFloat& aHeight);

	TIntFloat GetLandingSurfaceHeightPosition();//is the sum of the Height of the Landing surface plus the Y position of the Landing  surface
	void SetLandingSurfaceDestroyed(); //called if it should be possible anymore to land on this landing surface
	void UpdateXLocation(TIntFloat aChange);
	void AddPlaneLandedOnSurface(CAircraft* aPlane);
	void RemovePlaneLandedOnSurface(CAircraft* aPlane);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLandingSurface(TPoint aLocation,SGameObjectAttributes& aAttributes,const TIntFloat& aWidth,const TIntFloat& aHeight);
	CLandingSurface(TPoint aLocation,CTextureObject* aTextureObject,TInt aZValue,SGameObjectAttributes& aAttributes,const TIntFloat& aWidth,const TIntFloat& aHeight);
	void ConstructL();

	CPointerArray<CAircraft>* iPlanesLandedOnSurface;
	TIntFloat iWidthOfSurface;
	TIntFloat iHeightOfSurface;
	};

#endif /* CLANDINGSURFACE_H_ */
