/*
 * CPlaneWheels.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CPLANEWHEELS_H_
#define CPLANEWHEELS_H_

#include "includes/game/backend/basicClasses/CGameObject.h"
#include "includes/game/backend/basicClasses/CLandingSurface.h"

class CAircraft;
// CLASS DECLARATION

/**
 *  This object must be attached to plane inorder to function since its only function is to bumb up the plane if it hits landing surface and maybe reposition
 *  plane so that it looks like it would be rolling on the landing surface
 *
 */
class CPlaneWheels
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CPlaneWheels();

	static CPlaneWheels* New(CHitBox* aHitBox, CAircraft* aAircraft);
	virtual void PossibleCollision(CInterval*& aInterval);
	void PlaneDied(); //called by owning plane if it has died to give it chance to do some clean up
	void SaveOnDisk(CFileWriteStream &aOutputStream);
	void LoadFromDisk(CFileReadStream &aOutputStream);
	CHitBox* iHitBox;

protected:

	CAircraft* iAircraft;
	CLandingSurface* iLandingSurfaceLandedOn;// points to the landing Surface is currently in contact with this wheel

	CPlaneWheels(CHitBox* aHitBox, CAircraft* aAircraft);
	void InitiateLanding(CLandingSurface* aLandingSurface);
	void ConstructL();

};

#endif /* CPLANEWHEELS_H_ */
