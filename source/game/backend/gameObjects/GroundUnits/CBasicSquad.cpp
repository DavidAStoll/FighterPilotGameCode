/*
 ============================================================================
 Name		: BasicSquad.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CBasicSquad implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GroundUnits/CBasicSquad.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"


void CBasicSquad::AddGroundUnit(CGroundUnitOverlordAI* aOverlord,TGameObjectIdentifier aId,TInt& aXPosition,TBool aReflected)
{
	TInt lSoldierWidthOffsetBefore;
	TInt lTankWidthOffsetBefore;
	TInt lSoldierWidthOffsetAfter;
	TInt lTankWidthOffsetAfter;
	TInt lNormalWidthOffset;
	if(aReflected)
	{
		//looking to the right, need to add
		lSoldierWidthOffsetBefore = 0;
		lTankWidthOffsetBefore = 0;
		lSoldierWidthOffsetAfter = AMERICAN_ROCKETMAN_WIDTH; //all soldiers have same width offset
		lTankWidthOffsetAfter = AMERICAN_SHERMAN_TANK_WIDTH; //all tanks have  same width offset
		lNormalWidthOffset = BASIC_GROUND_UNIT_DEFAULT_SQUAD_DISTANCE_BETWEEN_UNITS;
	}
	else
	{
		//looking to the left, need to substract
		lSoldierWidthOffsetBefore = -AMERICAN_ROCKETMAN_WIDTH; //all soldiers have same width offset
		lTankWidthOffsetBefore = -AMERICAN_SHERMAN_TANK_WIDTH; //all tanks have  same width offset
		lSoldierWidthOffsetAfter = 0; //all soldiers have same width offset
		lTankWidthOffsetAfter = 0; //all tanks have  same width offset
		lNormalWidthOffset = -BASIC_GROUND_UNIT_DEFAULT_SQUAD_DISTANCE_BETWEEN_UNITS;
	}
	
	switch(aId)
	{
		//------------------------ American Ground Units ----------------------------------------//
		
		case EGameObjectIdentifierAmericanRifleman:
		{
			aXPosition += lSoldierWidthOffsetBefore;
			CGroundUnit* lCurrentGroundUnit = CAmericanRifleman::New(aXPosition, TIntFloat::Convert(0),aReflected);
			aOverlord->AddGroundUnit(lCurrentGroundUnit);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lCurrentGroundUnit);
			aXPosition += lSoldierWidthOffsetAfter;
			aXPosition += lNormalWidthOffset;
			return;
		}
		case EGameObjectIdentifierAmericanRocketman:
		{
			aXPosition += lSoldierWidthOffsetBefore;
			CGroundUnit* lCurrentGroundUnit = CAmericanRocketman::New(aXPosition,TIntFloat::Convert(0),aReflected);
			aOverlord->AddGroundUnit(lCurrentGroundUnit);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lCurrentGroundUnit);
			aXPosition += lSoldierWidthOffsetAfter;
			aXPosition += lNormalWidthOffset;
			return;
		}
		case EGameObjectIdentifierAmericanShermanTank:
		{
			aXPosition += lTankWidthOffsetBefore;
			CGroundUnit* lCurrentGroundUnit = CAmericanShermanTank::New(aXPosition,TIntFloat::Convert(0),aReflected);
			aOverlord->AddGroundUnit(lCurrentGroundUnit);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lCurrentGroundUnit);
			aXPosition += lTankWidthOffsetAfter;
			aXPosition += lNormalWidthOffset;
			return;
		}
		case EGameObjectIdentifierAmericanAAShermanTank:
		{
			aXPosition += lTankWidthOffsetBefore;
			CGroundUnit* lCurrentGroundUnit = CAmericanAAShermanTank::New(aXPosition,TIntFloat::Convert(0),aReflected);
			aOverlord->AddGroundUnit(lCurrentGroundUnit);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lCurrentGroundUnit);
			aXPosition += lTankWidthOffsetAfter;
			aXPosition += lNormalWidthOffset;
			return;
		}
		
		//---------------------------- Japanese Ground Units ---------------------------------//
		
		case EGameObjectIdentifierJapaneseRifleman:
		{
			aXPosition += lSoldierWidthOffsetBefore;
			CGroundUnit* lCurrentGroundUnit = CJapaneseRifleman::New(aXPosition,TIntFloat::Convert(0),aReflected);
			aOverlord->AddGroundUnit(lCurrentGroundUnit);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lCurrentGroundUnit);
			aXPosition += lSoldierWidthOffsetAfter;
			aXPosition += lNormalWidthOffset;
			return;
		}
		case EGameObjectIdentifierJapaneseRocketman:
		{
			aXPosition += lSoldierWidthOffsetBefore;
			CGroundUnit* lCurrentGroundUnit = CJapaneseRocketman::New(aXPosition,TIntFloat::Convert(0),aReflected);
			aOverlord->AddGroundUnit(lCurrentGroundUnit);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lCurrentGroundUnit);
			aXPosition += lSoldierWidthOffsetAfter;
			aXPosition += lNormalWidthOffset;
			return;
		}
		case EGameObjectIdentifierJapanese97ChiHaTank:
		{
			aXPosition += lTankWidthOffsetBefore;
			CGroundUnit* lCurrentGroundUnit = CJapanese97ChiHaTank::New(aXPosition,TIntFloat::Convert(0),aReflected);
			aOverlord->AddGroundUnit(lCurrentGroundUnit);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lCurrentGroundUnit);
			aXPosition += lTankWidthOffsetAfter;
			aXPosition += lNormalWidthOffset;
			return;
		}
		case EGameObjectIdentifierJapaneseAA97ChiHaTank:
		{
			aXPosition += lTankWidthOffsetBefore;
			CGroundUnit* lCurrentGroundUnit = CJapaneseAA97ChiHaTank::New(aXPosition,TIntFloat::Convert(0),aReflected);
			aOverlord->AddGroundUnit(lCurrentGroundUnit);
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lCurrentGroundUnit);
			aXPosition += lTankWidthOffsetAfter;
			aXPosition += lNormalWidthOffset;
			return;
		}
		default:
			return;//don't add anything
	}
}

TInt CBasicSquad::GetTotalWidthOfUnit(TGameObjectIdentifier aId)
{
	TInt lTotalOffset = BASIC_GROUND_UNIT_DEFAULT_SQUAD_DISTANCE_BETWEEN_UNITS;
	TInt lSoldierWidth = AMERICAN_ROCKETMAN_WIDTH; //all soldiers have same width offset
	TInt lTankWidth = AMERICAN_SHERMAN_TANK_WIDTH; //all tanks have  same width offset
	
	switch(aId)
	{
		//------------------------ American Ground Units ----------------------------------------//
		
		case EGameObjectIdentifierAmericanRifleman:
		case EGameObjectIdentifierAmericanRocketman:
		case EGameObjectIdentifierJapaneseRifleman:
		case EGameObjectIdentifierJapaneseRocketman:
		{
			lTotalOffset += lSoldierWidth;
			break;
		}
		
		case EGameObjectIdentifierAmericanShermanTank:
		case EGameObjectIdentifierAmericanAAShermanTank:
		case EGameObjectIdentifierJapanese97ChiHaTank:
		case EGameObjectIdentifierJapaneseAA97ChiHaTank:
		{
			lTotalOffset += lTankWidth;
			break;
		}
		
		default:
			break;//don't add anything
	}
	return lTotalOffset;
}


