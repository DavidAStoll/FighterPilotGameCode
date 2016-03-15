/*
 * CHitBox.cpp
 *
 *  Created on: May 27, 2012
 *      Author: dstoll
 */

#include "includes/core/collision/CHitBox.h"
#include "includes/core/CGame.h"

CHitBox::CHitBox(TPointIntFloat* aLocation, TInt aNumberOfPoints, TInt aNumberOfMovingPoints, TIntFloat*aXSpeed, TIntFloat*aYSpeed, TBool* aReflected, TInt* aAngel, SBoundingCollisionPoint* aPoints)
{
	iCoordinates = aLocation;
	iNumberOfPointsNormal = aNumberOfPoints;
	iNumberOfPointsMovingHitBox = aNumberOfMovingPoints;
	iPoints = aPoints;
	iPrimitiveHitBox = false;
	iReflected = aReflected;
	iAngel = aAngel;
	iXSpeedOfObject = aXSpeed;
	iYSpeedOfObject = aYSpeed;
	iLastFrameCalled = -100; //since there are no negative frames, it will be updated on the first call
	iBoundingSize.iHeight = TIntFloat::Convert(1);//not used
	iBoundingSize.iWidth = TIntFloat::Convert(1); //not used
}

CHitBox::CHitBox(TPointIntFloat* aLocation, TBool* aReflected, const TSize& aSizeOfHitBox)
{
	iCoordinates = aLocation;
	iNumberOfPointsNormal = HITBOX_NUMBER_OF_PRIMITIVE_POINTS;
	iNumberOfPointsMovingHitBox = HITBOX_NUMBER_OF_PRIMITIVE_POINTS;
	iBoundingSize.iHeight = TIntFloat::Convert(aSizeOfHitBox.iHeight);
	iBoundingSize.iWidth = TIntFloat::Convert(aSizeOfHitBox.iWidth);
	iPrimitiveHitBox = true;
	iReflected = aReflected;
	iLastFrameCalled = -100; //since there are no negative frames, it will be updated on the first call
	iAngel = NULL;
	iPoints = NULL;
	iXSpeedOfObject = NULL;
	iYSpeedOfObject = NULL;
}

CHitBox::~CHitBox()
{
}

CHitBox* CHitBox::New(TPointIntFloat* aLocation,TInt aNumberOfPoints,TInt aNumberOfMovingPoints,TIntFloat*aXSpeed,TIntFloat*aYSpeed,TBool* aReflected,TInt* aAngel,SBoundingCollisionPoint* aPoints)
{
	CHitBox* lSelf = new CHitBox(aLocation, aNumberOfPoints, aNumberOfMovingPoints, aXSpeed, aYSpeed, aReflected, aAngel, aPoints);
	lSelf->Construct();
	return lSelf;
}

CHitBox* CHitBox::New(TPointIntFloat* aLocation, TBool* aReflected, const TSize& aSizeOfHitBox)
{
	CHitBox* lSelf = new CHitBox(aLocation, aReflected, aSizeOfHitBox);
	lSelf->Construct();
	return lSelf;
}

void CHitBox::Construct()
{
	//nothing to do here
}

//----------------------------------- functions ---------------------------------------//

TBool CHitBox::AdvancedIntersectionCalculation(SLinearEquation* aLineEq1, TPointIntFloat** aPoints1, TInt aNumberOfPoints1, SLinearEquation* aLineEq2, TPointIntFloat** aPoints2, TInt aNumberOfPoints2)
{
	for(TInt lCurrentLine1Point = 0; lCurrentLine1Point < aNumberOfPoints1 - 1; lCurrentLine1Point++)
		for(TInt lCurrentLine2Point = 0; lCurrentLine2Point < aNumberOfPoints2 - 1; lCurrentLine2Point++)
		{
			if(CMath::LinesIntersect(aLineEq1[lCurrentLine1Point], *aPoints1[lCurrentLine1Point], *aPoints1[lCurrentLine1Point + 1], aLineEq2[lCurrentLine2Point], *aPoints2[lCurrentLine2Point], *aPoints2[lCurrentLine2Point + 1]))
			{
				return true;
			}
		}
	return false;//no intersection happened
}

TPointIntFloat* CHitBox::AdvancedIntersectionCalculationWithColisionPoint(SLinearEquation* aLineEq1, TPointIntFloat** aPoints1, TInt aNumberOfPoints1, SLinearEquation* aLineEq2, TPointIntFloat** aPoints2, TInt aNumberOfPoints2)
{
	for(TInt lCurrentLine1Point = 0; lCurrentLine1Point < aNumberOfPoints1 - 1; lCurrentLine1Point++)
		for(TInt lCurrentLine2Point = 0; lCurrentLine2Point < aNumberOfPoints2 - 1; lCurrentLine2Point++)
		{
			TPointIntFloat* lCollisionPoint = NULL;
			lCollisionPoint = CMath::LinesIntersectWithIntersectionPoint(aLineEq1[lCurrentLine1Point], *aPoints1[lCurrentLine1Point], *aPoints1[lCurrentLine1Point + 1], aLineEq2[lCurrentLine2Point], *aPoints2[lCurrentLine2Point], *aPoints2[lCurrentLine2Point + 1]);
			if(lCollisionPoint)
			{
				return lCollisionPoint;
			}
		}
	return NULL;//no intersection happened
}

void CHitBox::AssignAngle(TInt* aAngle)
{
	iAngel = aAngle;

	//since changed
	iLastFrameCalled = -100;
}

void CHitBox::AssignCollisionPoints(SBoundingCollisionPoint* aCollisionPoints)
{
	iPoints = aCollisionPoints;

	//since changed
	iLastFrameCalled = -100;
}

void CHitBox::DrawSimpleHitBoxL()
{
	TRectIntFloat lFastRect = GetFastIntersectionRect();
	CGame::Game->iGraphicsEngine->DrawRect(lFastRect);
}

void CHitBox::DrawAdvancedHitBoxL()
{
	if(iPrimitiveHitBox)
	{
		TPointIntFloat ** lBoundingPoints = GetPrimitivePointsLC();
		//draw lines
		for(TInt lCurrentLine = 0; lCurrentLine < 4; lCurrentLine++)
			CGame::Game->iGraphicsEngine->DrawLine(*lBoundingPoints[lCurrentLine], *lBoundingPoints[lCurrentLine + 1]);

		for(TInt lPoint = 0; lPoint < HITBOX_NUMBER_OF_PRIMITIVE_POINTS; lPoint++)
			delete lBoundingPoints[lPoint];
		delete[] lBoundingPoints;
	}else
	{
		TPointIntFloat ** lBoundingPoints = GetAdvancedPointsLC();
		//draw all lines
		for(TInt lCurrentLine = 0; lCurrentLine < GetNumberOfPoints() - 1; lCurrentLine++)
			CGame::Game->iGraphicsEngine->DrawLine(*lBoundingPoints[lCurrentLine], *lBoundingPoints[lCurrentLine + 1]);

		for(TInt lPoint = 0; lPoint < GetNumberOfPoints(); lPoint++)
			delete lBoundingPoints[lPoint];
		delete[] lBoundingPoints;
	}
}

TPointIntFloat** CHitBox::GetAdvancedPointsLC()
{
	//get relative points for advanced HitBox
	TPointIntFloat ** lAdvancedPoints = NULL;

	if(iPrimitiveHitBox)
	{
		lAdvancedPoints = GetPrimitivePointsLC();
		return lAdvancedPoints;
	}

	//check if we are moving
	if(*iXSpeedOfObject == 0 && *iYSpeedOfObject == 0)
	{
		lAdvancedPoints = new TPointIntFloat*[iNumberOfPointsNormal];
		for(TInt lNormalPointIndex = 0; lNormalPointIndex < iNumberOfPointsNormal; lNormalPointIndex++)
			lAdvancedPoints[lNormalPointIndex] = GetRelativeCollisionPointL(&iPoints[lNormalPointIndex]);
	}else
	{
		lAdvancedPoints = new TPointIntFloat*[iNumberOfPointsMovingHitBox];
		TInt lAdvancedPointIndex = 0;
		TBool lAvancedPointMode = false;

		for(TInt lNormalPointIndex = 0; lNormalPointIndex < iNumberOfPointsNormal; lNormalPointIndex++)
		{
			//points are now located on the projected hitbox
			if(iPoints[lNormalPointIndex].iMovementType & EPointMovementTypeConnectionStartPoint)
			{
				lAvancedPointMode = true;
				lAdvancedPoints[lAdvancedPointIndex] = GetRelativeCollisionPointL(&iPoints[lNormalPointIndex]);

				lAdvancedPointIndex++;
				lAdvancedPoints[lAdvancedPointIndex] = GetRelativeCollisionPointL(&iPoints[lNormalPointIndex]);//first advanced point
				lAdvancedPoints[lAdvancedPointIndex]->iX += *iXSpeedOfObject;
				lAdvancedPoints[lAdvancedPointIndex]->iY += *iYSpeedOfObject;
			}
			//points are now located on the originial hitbox
			else if(iPoints[lNormalPointIndex].iMovementType & EPointMovementTypeConnectionEndPoint)
			{
				if(lAvancedPointMode)
				{
					lAvancedPointMode = false;
					lAdvancedPoints[lAdvancedPointIndex] = GetRelativeCollisionPointL(&iPoints[lNormalPointIndex]);
					lAdvancedPoints[lAdvancedPointIndex]->iX += *iXSpeedOfObject;
					lAdvancedPoints[lAdvancedPointIndex]->iY += *iYSpeedOfObject;

					lAdvancedPointIndex++;
					lAdvancedPoints[lAdvancedPointIndex] = GetRelativeCollisionPointL(&iPoints[lNormalPointIndex]);
				}else
				{
					lAdvancedPoints[lAdvancedPointIndex] = GetRelativeCollisionPointL(&iPoints[lNormalPointIndex]);
				}
			}else //either on the projected or original hitbox depending on state
			{
				if(lAvancedPointMode)
				{
					lAdvancedPoints[lAdvancedPointIndex] = GetRelativeCollisionPointL(&iPoints[lNormalPointIndex]);
					lAdvancedPoints[lAdvancedPointIndex]->iX += *iXSpeedOfObject;
					lAdvancedPoints[lAdvancedPointIndex]->iY += *iYSpeedOfObject;
				}else
					lAdvancedPoints[lAdvancedPointIndex] = GetRelativeCollisionPointL(&iPoints[lNormalPointIndex]);
			}
			lAdvancedPointIndex++;
		}
	}
	return lAdvancedPoints;
}

const TRectIntFloat& CHitBox::GetFastIntersectionRect()
{
	//check if we already computed the fast intersection Rect in this frame, to avoid recomputation
	//FOR SOME REASON THIS DOES NOT WORK PROPERLY. SOMETIMES THE HITBOX DOES NOT HAVE THE COORDINATE ANYMORE
	//I SUSPECT THAT IT GET CHANGED SOMEWHERE IN THE CODE
//	if(CGame::Game->GetFrame() == iLastFrameCalled)
//	{
//		TInt lGameFrame = CGame::Game->GetFrame();
//		TInt lHitBoxFrame = iLastFrameCalled;
//
//		return iFastIntersectionRect;
//	}

	if(iPrimitiveHitBox)
	{
		if(*iReflected)
		{
			iFastIntersectionRect = TRectIntFloat(iCoordinates->iX - iBoundingSize.iWidth, iCoordinates->iY + iBoundingSize.iHeight,//top left corner
					iCoordinates->iX, iCoordinates->iY);//bottom right corner
		}else
		{
			iFastIntersectionRect = TRectIntFloat(iCoordinates->iX, iCoordinates->iY + iBoundingSize.iHeight,//top left corner
					iCoordinates->iX + iBoundingSize.iWidth, iCoordinates->iY);//bottom right corner
		}
	}
	else
	{
		TPointIntFloat ** lBoundingPoints = GetAdvancedPointsLC();

		//Now find smallest X,Y and biggest X,Y
		TIntFloat lSmallestX, lSmallestY, lBiggestY, lBiggestX;
		lSmallestX = lBoundingPoints[0]->iX;
		lBiggestX = lBoundingPoints[0]->iX;
		lSmallestY = lBoundingPoints[0]->iY;
		lBiggestY = lBoundingPoints[0]->iY;

		for(TInt lCurrentBoundingPoint = 1; lCurrentBoundingPoint < GetNumberOfPoints(); lCurrentBoundingPoint++)
		{
			if(lSmallestX > lBoundingPoints[lCurrentBoundingPoint]->iX)
				lSmallestX = lBoundingPoints[lCurrentBoundingPoint]->iX;
			if(lSmallestY > lBoundingPoints[lCurrentBoundingPoint]->iY)
				lSmallestY = lBoundingPoints[lCurrentBoundingPoint]->iY;
			if(lBiggestX < lBoundingPoints[lCurrentBoundingPoint]->iX)
				lBiggestX = lBoundingPoints[lCurrentBoundingPoint]->iX;
			if(lBiggestY < lBoundingPoints[lCurrentBoundingPoint]->iY)
				lBiggestY = lBoundingPoints[lCurrentBoundingPoint]->iY;
		}
		for(TInt lPoint = 0; lPoint < GetNumberOfPoints(); lPoint++)
			delete lBoundingPoints[lPoint];
		delete[] lBoundingPoints;
		iFastIntersectionRect = TRectIntFloat(lSmallestX, lBiggestY,//left top point
				lBiggestX, lSmallestY);//right bottom point
	}

	//update current frame timer
	//iLastFrameCalled = CGame::Game->GetFrame();
	return iFastIntersectionRect;
}

TInt& CHitBox::GetNumberOfPoints()
{
	if(iPrimitiveHitBox)
	{
		return iNumberOfPointsNormal;
	}

	if(*iXSpeedOfObject == 0 && *iYSpeedOfObject == 0)
		return iNumberOfPointsNormal;
	else
		return iNumberOfPointsMovingHitBox;
}

SLinearEquation* CHitBox::GetLinearEquationsLC(TPointIntFloat** aPoints, TInt aNumberOfPoints)
{
	SLinearEquation * lLinearEqs = new SLinearEquation[aNumberOfPoints - 1];

	//get all Linear equations formed by the given points
	for(TInt lCurrentPrimitivePoint = 0; lCurrentPrimitivePoint < aNumberOfPoints - 1; lCurrentPrimitivePoint++)
	{
		lLinearEqs[lCurrentPrimitivePoint] = CMath::GetLinearEquation(*aPoints[lCurrentPrimitivePoint], *aPoints[lCurrentPrimitivePoint + 1]);
	}
	return lLinearEqs;
}

TPointIntFloat** CHitBox::GetPrimitivePointsLC()
{
	TPointIntFloat ** lPrimitivePoints = new TPointIntFloat*[HITBOX_NUMBER_OF_PRIMITIVE_POINTS];
	SBoundingCollisionPoint lTemp;

	//can't have straight Y lines due to intersection calculation
	lTemp.XOffset = -5;
	lTemp.YOffset = 0;//bottom left
	lPrimitivePoints[0] = GetSimpleRelativeCollisionPointL(&lTemp);

	lTemp.XOffset = 3;
	lTemp.YOffset = (iBoundingSize.iHeight).GetIntInBaseInt();//top left
	lPrimitivePoints[1] = GetSimpleRelativeCollisionPointL(&lTemp);

	lTemp.XOffset = (iBoundingSize.iWidth - 5).GetIntInBaseInt();
	lTemp.YOffset = (iBoundingSize.iHeight).GetIntInBaseInt();//top right
	lPrimitivePoints[2] = GetSimpleRelativeCollisionPointL(&lTemp);

	lTemp.XOffset = (iBoundingSize.iWidth + 1).GetIntInBaseInt();
	lTemp.YOffset = 0;
	lPrimitivePoints[3] = GetSimpleRelativeCollisionPointL(&lTemp);

	lTemp.XOffset = 0;
	lTemp.YOffset = 3;//repeat last point
	lPrimitivePoints[4] = GetSimpleRelativeCollisionPointL(&lTemp);

	return lPrimitivePoints;
}

TBool CHitBox::IntersectionL(CHitBox* aHitBox)
{
	if(GetFastIntersectionRect().Intersects(aHitBox->GetFastIntersectionRect()))
	{//do more expensive calculation if it really intersects
		if(iPrimitiveHitBox && aHitBox->IsPrimitiveHitBox())
		{
			//both are primitive, since we already did the required calculations we can return true
			return true;
		}else if(iPrimitiveHitBox && !aHitBox->IsPrimitiveHitBox())//only one primitive HitBox, aHitBox is not primitive
		{
			//get relative points for primitive HitBox
			TPointIntFloat ** lPrimitivePoints = GetPrimitivePointsLC();
			//get relative points for advanced HitBox
			TPointIntFloat ** lAdvancedPoints = aHitBox->GetAdvancedPointsLC();
			//get Primitive Linear equations
			SLinearEquation * lPrimitiveLinearEq = CHitBox::GetLinearEquationsLC(lPrimitivePoints, HITBOX_NUMBER_OF_PRIMITIVE_POINTS);
			//get advanced linear equations
			SLinearEquation * lAdvancedLinearEq = CHitBox::GetLinearEquationsLC(lAdvancedPoints, aHitBox->GetNumberOfPoints());
			//have required data, check if any Lines intersect
			TBool lIntersection = CHitBox::AdvancedIntersectionCalculation(lPrimitiveLinearEq, lPrimitivePoints, HITBOX_NUMBER_OF_PRIMITIVE_POINTS, lAdvancedLinearEq, lAdvancedPoints, aHitBox->GetNumberOfPoints());

			if(!lIntersection) // do one more check to make sure the advanced box is not inside the primitive box
				lIntersection = AdvancedPointsAreInsidePrimitiveHitBox(aHitBox->GetNumberOfPoints(), lAdvancedPoints, GetFastIntersectionRect());

			//clean up
			delete[] lAdvancedLinearEq;
			delete[] lPrimitiveLinearEq;
			for(TInt lCurObj = 0; lCurObj < aHitBox->GetNumberOfPoints(); lCurObj++)
				delete lAdvancedPoints[lCurObj];
			delete[] lAdvancedPoints;
			for(TInt lCurObj = 0; lCurObj < HITBOX_NUMBER_OF_PRIMITIVE_POINTS; lCurObj++)
				delete lPrimitivePoints[lCurObj];
			delete[] lPrimitivePoints;
			//return result
			return lIntersection;
		}else if(!iPrimitiveHitBox && aHitBox->IsPrimitiveHitBox())//only one primitive HitBox, aHitBox is primitive
		{
			//get relative points for primitive HitBox
			TPointIntFloat ** lPrimitivePoints = aHitBox->GetPrimitivePointsLC();
			//get relative points for advanced HitBox
			TPointIntFloat ** lAdvancedPoints = GetAdvancedPointsLC();
			//get Primitive Linear equations
			SLinearEquation * lPrimitiveLinearEq = CHitBox::GetLinearEquationsLC(lPrimitivePoints, HITBOX_NUMBER_OF_PRIMITIVE_POINTS);
			//get advanced linear equations
			SLinearEquation * lAdvancedLinearEq = CHitBox::GetLinearEquationsLC(lAdvancedPoints, GetNumberOfPoints());
			//have required data, check if any Lines intersect
			TBool lIntersection = CHitBox::AdvancedIntersectionCalculation(lPrimitiveLinearEq, lPrimitivePoints, HITBOX_NUMBER_OF_PRIMITIVE_POINTS, lAdvancedLinearEq, lAdvancedPoints, GetNumberOfPoints());

			if(!lIntersection) // do one more check to make sure the advanced box is not inside the primitive box
				lIntersection = AdvancedPointsAreInsidePrimitiveHitBox(GetNumberOfPoints(), lAdvancedPoints, aHitBox->GetFastIntersectionRect());

			//clean up
			delete[] lAdvancedLinearEq;
			delete[] lPrimitiveLinearEq;
			for(TInt lCurObj = 0; lCurObj < GetNumberOfPoints(); lCurObj++)
				delete lAdvancedPoints[lCurObj];
			delete[] lAdvancedPoints;
			for(TInt lCurObj = 0; lCurObj < HITBOX_NUMBER_OF_PRIMITIVE_POINTS; lCurObj++)
				delete lPrimitivePoints[lCurObj];
			delete[] lPrimitivePoints;
			//return result
			return lIntersection;
		}else // both are complex objects
		{
			TPointIntFloat ** lAvancedPoints1 = aHitBox->GetAdvancedPointsLC();
			//get relative points for advanced HitBox
			TPointIntFloat ** lAdvancedPoints2 = GetAdvancedPointsLC();
			//get Primitive Linear equations
			SLinearEquation * lAdvancedLinearEq1 = CHitBox::GetLinearEquationsLC(lAvancedPoints1, aHitBox->GetNumberOfPoints());
			//get advanced linear equations
			SLinearEquation * lAdvancedLinearEq2 = CHitBox::GetLinearEquationsLC(lAdvancedPoints2, GetNumberOfPoints());
			//have required data, check if any Lines intersect
			TBool lIntersection = CHitBox::AdvancedIntersectionCalculation(lAdvancedLinearEq1, lAvancedPoints1, aHitBox->GetNumberOfPoints(), lAdvancedLinearEq2, lAdvancedPoints2, GetNumberOfPoints());
			//clean up
			delete[] lAdvancedLinearEq2;
			;
			delete[] lAdvancedLinearEq1;
			for(TInt lCurObj = 0; lCurObj < GetNumberOfPoints(); lCurObj++)
				delete lAdvancedPoints2[lCurObj];
			delete[] lAdvancedPoints2;
			for(TInt lCurObj = 0; lCurObj < aHitBox->GetNumberOfPoints(); lCurObj++)
				delete lAvancedPoints1[lCurObj];
			delete[] lAvancedPoints1;
			//return result
			return lIntersection;
		}
	}else
		return false;
}

TPointIntFloat* CHitBox::IntersectionWithCollisionPointL(CHitBox* aHitBox)
{
	TPointIntFloat* lCollisionPoint = NULL;

	if(GetFastIntersectionRect().Intersects(aHitBox->GetFastIntersectionRect()))
	{//do more expensive calculation if it really intersects
		if(iPrimitiveHitBox && aHitBox->IsPrimitiveHitBox())
		{
			//get relative points for primitive HitBox
			TPointIntFloat ** lPrimitivePoints1 = GetPrimitivePointsLC();
			//get relative points for primitive HitBox
			TPointIntFloat ** lPrimitivePoints2 = aHitBox->GetPrimitivePointsLC();
			//get Primitive Linear equations
			SLinearEquation * lPrimitiveLinearEq1 = CHitBox::GetLinearEquationsLC(lPrimitivePoints1, HITBOX_NUMBER_OF_PRIMITIVE_POINTS);
			//get Primitive Linear equations
			SLinearEquation * lPrimitiveLinearEq2 = CHitBox::GetLinearEquationsLC(lPrimitivePoints2, HITBOX_NUMBER_OF_PRIMITIVE_POINTS);

			//have required data, check if any Lines intersect
			lCollisionPoint = CHitBox::AdvancedIntersectionCalculationWithColisionPoint(lPrimitiveLinearEq1, lPrimitivePoints1, HITBOX_NUMBER_OF_PRIMITIVE_POINTS, lPrimitiveLinearEq2, lPrimitivePoints2, HITBOX_NUMBER_OF_PRIMITIVE_POINTS);
			//clean up
			delete[] lPrimitiveLinearEq1;
			delete[] lPrimitiveLinearEq2;
			for(TInt lCurObj = 0; lCurObj < HITBOX_NUMBER_OF_PRIMITIVE_POINTS; lCurObj++)
				delete lPrimitivePoints1[lCurObj];
			delete[] lPrimitiveLinearEq1;
			for(TInt lCurObj = 0; lCurObj < HITBOX_NUMBER_OF_PRIMITIVE_POINTS; lCurObj++)
				delete lPrimitivePoints2[lCurObj];
			delete[] lPrimitiveLinearEq2;
		}else if(iPrimitiveHitBox && !aHitBox->IsPrimitiveHitBox())//only one primitive HitBox, aHitBox is not primitive
		{
			//get relative points for primitive HitBox
			TPointIntFloat ** lPrimitivePoints = GetPrimitivePointsLC();
			//get relative points for advanced HitBox
			TPointIntFloat ** lAdvancedPoints = aHitBox->GetAdvancedPointsLC();
			//get Primitive Linear equations
			SLinearEquation * lPrimitiveLinearEq = CHitBox::GetLinearEquationsLC(lPrimitivePoints, HITBOX_NUMBER_OF_PRIMITIVE_POINTS);
			//get advanced linear equations
			SLinearEquation * lAdvancedLinearEq = CHitBox::GetLinearEquationsLC(lAdvancedPoints, aHitBox->GetNumberOfPoints());
			//have required data, check if any Lines intersect
			lCollisionPoint = CHitBox::AdvancedIntersectionCalculationWithColisionPoint(lPrimitiveLinearEq, lPrimitivePoints, HITBOX_NUMBER_OF_PRIMITIVE_POINTS, lAdvancedLinearEq, lAdvancedPoints, aHitBox->GetNumberOfPoints());
			//clean up
			delete[] lAdvancedLinearEq;
			delete[] lPrimitiveLinearEq;
			for(TInt lCurObj = 0; lCurObj < aHitBox->GetNumberOfPoints(); lCurObj++)
				delete lAdvancedPoints[lCurObj];
			delete[] lAdvancedPoints;
			for(TInt lCurObj = 0; lCurObj < HITBOX_NUMBER_OF_PRIMITIVE_POINTS; lCurObj++)
				delete lPrimitivePoints[lCurObj];
			delete[] lPrimitivePoints;
		}else if(!iPrimitiveHitBox && aHitBox->IsPrimitiveHitBox())//only one primitive HitBox, aHitBox is primitive
		{
			//get relative points for primitive HitBox
			TPointIntFloat ** lPrimitivePoints = aHitBox->GetPrimitivePointsLC();
			//get relative points for advanced HitBox
			TPointIntFloat ** lAdvancedPoints = GetAdvancedPointsLC();
			//get Primitive Linear equations
			SLinearEquation * lPrimitiveLinearEq = CHitBox::GetLinearEquationsLC(lPrimitivePoints, HITBOX_NUMBER_OF_PRIMITIVE_POINTS);
			//get advanced linear equations
			SLinearEquation * lAdvancedLinearEq = CHitBox::GetLinearEquationsLC(lAdvancedPoints, GetNumberOfPoints());
			//have required data, check if any Lines intersect
			lCollisionPoint = CHitBox::AdvancedIntersectionCalculationWithColisionPoint(lPrimitiveLinearEq, lPrimitivePoints, HITBOX_NUMBER_OF_PRIMITIVE_POINTS, lAdvancedLinearEq, lAdvancedPoints, GetNumberOfPoints());
			//clean up
			delete[] lAdvancedLinearEq;
			delete[] lPrimitiveLinearEq;
			for(TInt lCurObj = 0; lCurObj < GetNumberOfPoints(); lCurObj++)
				delete lAdvancedPoints[lCurObj];
			delete[] lAdvancedPoints;
			for(TInt lCurObj = 0; lCurObj < HITBOX_NUMBER_OF_PRIMITIVE_POINTS; lCurObj++)
				delete lPrimitivePoints[lCurObj];
			delete[] lPrimitivePoints;
		}else // both are complex objects
		{
			TPointIntFloat ** lAvancedPoints1 = aHitBox->GetAdvancedPointsLC();
			//get relative points for advanced HitBox
			TPointIntFloat ** lAdvancedPoints2 = GetAdvancedPointsLC();
			//get Primitive Linear equations
			SLinearEquation * lAdvancedLinearEq1 = CHitBox::GetLinearEquationsLC(lAvancedPoints1, aHitBox->GetNumberOfPoints());
			//get advanced linear equations
			SLinearEquation * lAdvancedLinearEq2 = CHitBox::GetLinearEquationsLC(lAdvancedPoints2, GetNumberOfPoints());
			//have required data, check if any Lines intersect
			lCollisionPoint = CHitBox::AdvancedIntersectionCalculationWithColisionPoint(lAdvancedLinearEq1, lAvancedPoints1, aHitBox->GetNumberOfPoints(), lAdvancedLinearEq2, lAdvancedPoints2, GetNumberOfPoints());
			//clean up
			delete[] lAdvancedLinearEq2;
			delete[] lAdvancedLinearEq1;
			for(TInt lCurObj = 0; lCurObj < GetNumberOfPoints(); lCurObj++)
				delete lAdvancedPoints2[lCurObj];
			delete[] lAdvancedPoints2;
			for(TInt lCurObj = 0; lCurObj < aHitBox->GetNumberOfPoints(); lCurObj++)
				delete lAvancedPoints1[lCurObj];
			delete[] lAvancedPoints1;
		}
	}

	return lCollisionPoint;
}

TBool CHitBox::IsPrimitiveHitBox()
{
	return iPrimitiveHitBox;
}

void CHitBox::SetNewPrimitiveHitBox(const TSize& aHitBox)
{
	iBoundingSize.iHeight = TIntFloat::Convert(aHitBox.iHeight);
	iBoundingSize.iWidth = TIntFloat::Convert(aHitBox.iWidth);

	//since changed
	iLastFrameCalled = -100;
}

TBool CHitBox::AdvancedPointsAreInsidePrimitiveHitBox(const TInt aNumberOfPoints, TPointIntFloat ** aAdvancedPoints, const TRectIntFloat& aHitBoxRect)
{
	for(TInt lCurrentPoint = 0; lCurrentPoint < aNumberOfPoints; lCurrentPoint++)
	{
		if(CMath::PointIsInsideRectangleOpenGlCoordinateSystem(*aAdvancedPoints[lCurrentPoint], aHitBoxRect))
			return true;//only one point must be inside the primitive Rectangle
	}
	return false; // no point is inside the primtive HitBox
}

TPointIntFloat* CHitBox::GetRelativeCollisionPointL(SBoundingCollisionPoint* aPoint)
{
	TPointIntFloat* lResult = new TPointIntFloat();

	if(*iReflected)
	{
		*lResult = *iCoordinates;
		lResult->iX += (CMath::GraphicsCosTable(*iAngel) * (- aPoint->XOffset)) - (CMath::GraphicsSinTable(*iAngel) * aPoint->YOffset);
		lResult->iY += (CMath::GraphicsCosTable(*iAngel) * (+ aPoint->YOffset)) - (CMath::GraphicsSinTable(*iAngel) * aPoint->XOffset);
		return lResult;
	}else
	{
		*lResult = *iCoordinates;
		lResult->iX += (CMath::GraphicsCosTable(*iAngel) * ( - aPoint->XOffset)) + (CMath::GraphicsSinTable(*iAngel) * aPoint->YOffset);
		lResult->iY += (CMath::GraphicsCosTable(*iAngel) * ( - aPoint->YOffset)) - (CMath::GraphicsSinTable(*iAngel) * aPoint->XOffset);
		return lResult;
	}
}

TPointIntFloat* CHitBox::GetSimpleRelativeCollisionPointL(SBoundingCollisionPoint* aPoint)
{
	TPointIntFloat* lResult = new TPointIntFloat();
	if(*iReflected)
	{
		*lResult = *iCoordinates;
		lResult->iX -= aPoint->XOffset;
		lResult->iY += aPoint->YOffset;
		return lResult;
	}else
	{
		*lResult = *iCoordinates;
		lResult->iX += aPoint->XOffset;
		lResult->iY += aPoint->YOffset;
		return lResult;
	}
}

void CHitBox::LoadFromDisk(CFileReadStream &aInputStream)
{
	iPrimitiveHitBox = aInputStream.ReadInt32();
	iNumberOfPointsNormal = aInputStream.ReadInt32();
	iNumberOfPointsMovingHitBox = aInputStream.ReadInt32();
	iLastFrameCalled = aInputStream.ReadInt32();
	iFastIntersectionRect.iBr.iX = aInputStream.ReadIntFloat();
	iFastIntersectionRect.iBr.iY = aInputStream.ReadIntFloat();
	iFastIntersectionRect.iTl.iX = aInputStream.ReadIntFloat();
	iFastIntersectionRect.iTl.iY = aInputStream.ReadIntFloat();
	iBoundingSize.iWidth = aInputStream.ReadIntFloat();
	iBoundingSize.iHeight = aInputStream.ReadIntFloat();
}

void CHitBox::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	aOutputStream.WriteInt32(iPrimitiveHitBox);
	aOutputStream.WriteInt32(iNumberOfPointsNormal);
	aOutputStream.WriteInt32(iNumberOfPointsMovingHitBox);
	aOutputStream.WriteInt32(iLastFrameCalled);
	aOutputStream.WriteIntFloat(iFastIntersectionRect.iBr.iX);
	aOutputStream.WriteIntFloat(iFastIntersectionRect.iBr.iY);
	aOutputStream.WriteIntFloat(iFastIntersectionRect.iTl.iX);
	aOutputStream.WriteIntFloat(iFastIntersectionRect.iTl.iY);
	aOutputStream.WriteIntFloat(iBoundingSize.iWidth);
	aOutputStream.WriteIntFloat(iBoundingSize.iHeight);
}
