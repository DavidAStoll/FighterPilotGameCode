/*
 * CSmallPlaneLocator.h
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#ifndef CSMALLPLANELOCATOR_H_
#define CSMALLPLANELOCATOR_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CGameObject.h"
#include "includes/GlobalDefines.h"

// CLASS DECLARATION
#define SMALL_PLANE_LOCATOR_WIDTH 16
#define SMALL_PLANE_LOCATOR_HEIGHT 16
#define SMALL_PLANE_LOCATOR_X_OFFSET 120
#define SMALL_PLANE_LOCATOR_Y_OFFSET (-8)
#define SMALL_PLANE_LOCATOR_FRAMES_UNTIL_CHECK_FOR_CLOSER_TARGET (GFRAMES_PER_SECOND_BACKEND)
/**
 *  CSmallPlaneLocator
 *
 */
class CSmallPlaneLocator
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSmallPlaneLocator();

	static CSmallPlaneLocator* New(TPointIntFloat* aCenterOfView, TConflictSide aConflictSide);

	static void InitilizeSmallPlaneLocator();
	static void RemoveSmallPlaneLocator();
	void Update();
	void Draw();
	void SaveOnDisk(CFileWriteStream &aOutputStream);
	void LoadFromDisk(CFileReadStream &aOutputStream);

private:

	CSmallPlaneLocator(TPointIntFloat* aCenterOfView, TConflictSide aConflictSide);
	TPointIntFloat GetSmallPlaneLocation();
	void ConstructL();

	static void LoadSmallPlaneLocatorTextures();
	static void UnloadSmallPlaneLocatorTextures();

	TPointIntFloat* iCenterOfView;
	TInt iAngle;
	TInt iCurrentFrame;
	TConflictSide iConflictSide;
	CTextureObject* iTextureObject;
	CGameObject* iCurrentTarget;
	};

#endif /* CSMALLPLANELOCATOR_H_ */
