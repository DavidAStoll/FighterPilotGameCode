/*
 * CHitBox.h
 *
 *  Created on: May 27, 2012
 *      Author: dstoll
 */

#ifndef CHITBOX_H_
#define CHITBOX_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/utility/CFileStream.h"

#define HITBOX_NUMBER_OF_PRIMITIVE_POINTS 5

// CLASS DECLARATION

/**
 *  CHitBox
 *  This class will be used to calculate intersections, by using various Points to form a bounding shape
 *  The number of points is arbartary from shape to shape but must be specified at construnction time
 *
 *
 *
 */

enum TPointMovementType
{
	EPointMovementTypeConnectionStartPoint = 1, //when this point is encountered the hitbox will be extended to projected hitbox
	EPointMovementTypeConnectionEndPoint = 2, //when this point is encoutered the hitbox will only place the point in the originial hitbox
	EPointMovementTypeNone = 4, // for points that are neither Start or End Points
};

struct SBoundingCollisionPoint
{
	TInt XOffset;
	TInt YOffset;
	TPointMovementType iMovementType;
};

class CHitBox
	{
public:

	~CHitBox();
	//advanced HitBox that can rotate
	static CHitBox* New(TPointIntFloat* aLocation,TInt aNumberOfPoints,TInt aNumberOfMovingPoints,TIntFloat*aXSpeed,TIntFloat*aYSpeed,TBool* aReflected,TInt* aAngel,SBoundingCollisionPoint* aPoints);
	//primitive HitBox, can't rotate (it can rotate but the Hitbox will not be accurate)
	static CHitBox* New(TPointIntFloat* aLocation, TBool* aReflected, const TSize& aSizeOfHitBox);
	//checks if any of the given lines intersect with each other
	static TBool AdvancedIntersectionCalculation(SLinearEquation* aLineEq1, TPointIntFloat** aPoints1, TInt aNumberOfPoints1,
                                               SLinearEquation* aLineEq2, TPointIntFloat** aPoints2, TInt aNumberOfPoints2);
	//checks for intersections returns NULL if no intersection takes places or the Point of intersection
	static TPointIntFloat* AdvancedIntersectionCalculationWithColisionPoint(SLinearEquation* aLineEq1, TPointIntFloat** aPoints1, TInt aNumberOfPoints1,
                                                                          SLinearEquation* aLineEq2, TPointIntFloat** aPoints2, TInt aNumberOfPoints2);
	//Assign a new Angle reference for this object
	void AssignAngle(TInt* aAngle);
	//Assign New Collision Points
	void AssignCollisionPoints(SBoundingCollisionPoint* aCollisionPoints);
	//draw the Fast Intersection Rectangle Box
	void DrawSimpleHitBoxL();
	//draws the hit box using all points defining the hitbox
	void DrawAdvancedHitBoxL();
	//retrieves relativePoints to the Object consindering it is a advanced hitbox
	TPointIntFloat** GetAdvancedPointsLC();
	//return a simple rectangle
	const TRectIntFloat& GetFastIntersectionRect();
	//return number of points that this HitBox is using to create a bounding box
	TInt& GetNumberOfPoints();
	//gets all linear equations connectining the lines to form a polygon
	static SLinearEquation* GetLinearEquationsLC(TPointIntFloat** aPoints, TInt aNumberOfPoints);
	//retrieves relativePoints to the Object consindering it is a primitvie hitbox
	TPointIntFloat** GetPrimitivePointsLC();
	//checks if two Hitboxes intersect
	TBool IntersectionL(CHitBox* aHitBox);
	TPointIntFloat* IntersectionWithCollisionPointL(CHitBox* aHitBox);
	//replace state of HitBox with the one from Disk
	void LoadFromDisk(CFileReadStream& aInputStream);
	//returns true if this HitBox is a Primitve HitBox
	TBool IsPrimitiveHitBox();
	//save HitBox state to disk
	void SaveOnDisk(CFileWriteStream& aOutputStream);
	//replace current Primitive HitBox with a new one
	void SetNewPrimitiveHitBox(const TSize& aSizeOfHitBox);

protected:

	CHitBox(TPointIntFloat* aLocation,TInt aNumberOfPoints,TInt aNumberOfMovingPoints,TIntFloat*aXSpeed,TIntFloat*aYSpeed,TBool* aReflected,TInt* aAngel,SBoundingCollisionPoint* aPoints);
	CHitBox(TPointIntFloat* aLocation,TBool* aReflected, const TSize& aSizeOfHitBox);
	void Construct();

	//checks if any points are inside a primitive HitBox
	TBool AdvancedPointsAreInsidePrimitiveHitBox(const TInt aNumberOfPoints,TPointIntFloat ** aAdvancedPoints,const TRectIntFloat& aSizeOfHitBox);
	//returns all Collission Points that counts in the angle of the HitBox as well
	TPointIntFloat* GetRelativeCollisionPointL(SBoundingCollisionPoint* aPoint);
	//returns all Collision Points without counting in the Angle of the HitBox
	TPointIntFloat* GetSimpleRelativeCollisionPointL(SBoundingCollisionPoint* aPoint);

  TInt iNumberOfPointsNormal;
  TInt iNumberOfPointsMovingHitBox;
  TBool iPrimitiveHitBox;
  TInt iLastFrameCalled;
  TRectIntFloat iFastIntersectionRect;
  TSizeIntFloat iBoundingSize;//used for primitive objects
  TPointIntFloat* iCoordinates;//references to the objects location, to find out where the bounding box is located in world coordinates
  TBool* iReflected;
  TInt* iAngel;
  SBoundingCollisionPoint* iPoints;//used for none advanced bounding objects
  TIntFloat* iXSpeedOfObject;
  TIntFloat* iYSpeedOfObject;
	};

#endif /* CHITBOX_H_ */
