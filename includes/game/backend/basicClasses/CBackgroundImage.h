/*
 * CBackgroundImage.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CBACKGROUNDIMAGE_H_
#define CBACKGROUNDIMAGE_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CDrawObject.h"

// CLASS DECLARATION
#define MAX_HEIGHT_OF_BITMAP 1024

/**
 *  CBackgroundImage
 *
 */
class CBackgroundImage : public CDrawObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CBackgroundImage();

	/**
	 * Two-phased constructor.
	 */
	static CBackgroundImage* New();
	void UpdatePosition(TPointIntFloat aPosition, TBool aIsInGroundView,TInt aCurrentAircraftSpeed);

	virtual void Draw();
	static void InitilizeBackgroundImage();
	static void RemoveBackgroundImage();
	void UpdateForZoomFactor();

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CBackgroundImage(TInt aZCor,CTextureObject* aTextureObject,CAnimationPlayer* aAnimationPlayer);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	static void LoadBackgroundImageTextures();
	static void UnloadBackgroundImageTextures();

	TIntFloat iBackgroundTextureWidth;
	TIntFloat iBackgroundTextureHeight;
	TFloat iBackgroundUpperTValue;

};

#endif /* CBACKGROUNDIMAGE_H_ */
