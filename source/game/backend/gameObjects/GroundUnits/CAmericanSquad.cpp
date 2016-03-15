/*
 ============================================================================
 Name		: AmericanSquad.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanSquad implementation
 ============================================================================
 */

#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/CFighterPilotThePacificWar.h"
#include "includes/game/backend/gameObjects/GroundUnits/CBasicSquad.h"

CGroundUnitOverlordAI* CAmericanSquad::CreateAmericanTankSquad(TInt aXLocation, TBool aReflected)
{		
	CGroundUnitOverlordAI* lAmericanOverlord = CGroundUnitOverlordAI::New(EConflictSideAmerican);
	
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanAAShermanTank,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanShermanTank,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanAAShermanTank,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanShermanTank,aXLocation,aReflected);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lAmericanOverlord);
	
	return lAmericanOverlord;
}

CGroundUnitOverlordAI* CAmericanSquad::CreateAmericanSoldierAATankSquad(TInt aXLocation, TBool aReflected)
{		
	CGroundUnitOverlordAI* lAmericanOverlord = CGroundUnitOverlordAI::New(EConflictSideAmerican);
	
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanAAShermanTank,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lAmericanOverlord);
	
	return lAmericanOverlord;
}

CGroundUnitOverlordAI* CAmericanSquad::CreateAmericanSoldierSquad(TInt aXLocation, TBool aReflected)
{		
	CGroundUnitOverlordAI* lAmericanOverlord = CGroundUnitOverlordAI::New(EConflictSideAmerican);
	
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lAmericanOverlord);
	
	return lAmericanOverlord;
}

CGroundUnitOverlordAI* CAmericanSquad::CreateAmericanSoldierAndTankSquad(TInt aXLocation, TBool aReflected)
{		
	CGroundUnitOverlordAI* lAmericanOverlord = CGroundUnitOverlordAI::New(EConflictSideAmerican);
	
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lAmericanOverlord,EGameObjectIdentifierAmericanShermanTank,aXLocation,aReflected);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lAmericanOverlord);
	
	return lAmericanOverlord;
}

TInt CAmericanSquad::GetWidthOfAmericanSoldierAATankSquad()
{
	TInt lTotalWidth = 0;
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanAAShermanTank);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	return lTotalWidth;
}

TInt CAmericanSquad::GetWidthOfAmericanSoldierSquad()
{
	TInt lTotalWidth = 0;
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);	
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);	
	return lTotalWidth;
}

TInt CAmericanSquad::GetWidthOfAmericanSoldierAndTankSquad() //width 224
{
	TInt lTotalWidth = 0;
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanShermanTank);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanRifleman);
	return lTotalWidth;
}

TInt CAmericanSquad::GetWidthOfAmericanTankSquad()
{
	TInt lTotalWidth = 0;
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanAAShermanTank);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanShermanTank);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanAAShermanTank);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierAmericanShermanTank);
	return lTotalWidth;
}
