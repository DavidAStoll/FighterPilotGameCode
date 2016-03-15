/*
 * CHeavyBomberLocator.h
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#ifndef CHEAVYBOMBERLOCATOR_H_
#define CHEAVYBOMBERLOCATOR_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CGameObject.h"
#include "includes/GlobalDefines.h"

// CLASS DECLARATION

#define HEAVY_BOMBER_LOCATOR_WIDTH 16
#define HEAVY_BOMBER_LOCATOR_HEIGHT 16
#define HEAVY_BOMBER_LOCATOR_X_OFFSET 120
#define HEAVY_BOMBER_LOCATOR_Y_OFFSET (-8)
#define HEAVY_BOMBER_LOCATOR_FRAMES_UNTIL_CHECK_FOR_CLOSER_TARGET (GFRAMES_PER_SECOND_BACKEND)

/**
 *  CHeavyBomberLocator
 *
 */
class CHeavyBomberLocator
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CHeavyBomberLocator();

	static CHeavyBomberLocator* New(TPointIntFloat* aCenterOfView, TConflictSide aConflictSide);

	static void InitilizeHeavyBomberLocator();
	static void RemoveHeavyBomberLocator();
	void Update();
	void Draw();
	void SaveOnDisk(CFileWriteStream &aOutputStream);
	void LoadFromDisk(CFileReadStream &aOutputStream);

private:

	CHeavyBomberLocator(TPointIntFloat* aCenterOfView, TConflictSide aConflictSide);
	TPointIntFloat GetHeavyBomberLocation();
	void ConstructL();

	static void LoadHeavyBomberLocatorTextures();
	static void UnloadHeavyBomberLocatorTextures();

	TPointIntFloat* iCenterOfView;
	TInt iAngle;
	TInt iCurrentFrame;
	TConflictSide iConflictSide;
	CTextureObject* iTextureObject;
	CGameObject* iCurrentTarget;

};

#endif /* CHEAVYBOMBERLOCATOR_H_ */
