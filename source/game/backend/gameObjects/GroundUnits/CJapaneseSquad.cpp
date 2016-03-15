/*
 ============================================================================
 Name		: JapaneseSquad.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseSquad implementation
 ============================================================================
 */
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"
#include "includes/game/backend/gameObjects/GroundUnits/CBasicSquad.h"
#include "includes/game/CFighterPilotThePacificWar.h"


CGroundUnitOverlordAI* CJapaneseSquad::CreateJapaneseTankSquad(TInt aXLocation, TBool aReflected)
{		
	CGroundUnitOverlordAI* lJapaneseOverlord = CGroundUnitOverlordAI::New(EConflictSideJapanese);
	
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseAA97ChiHaTank,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapanese97ChiHaTank,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseAA97ChiHaTank,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapanese97ChiHaTank,aXLocation,aReflected);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lJapaneseOverlord);
	
	return lJapaneseOverlord;
}

CGroundUnitOverlordAI* CJapaneseSquad::CreateJapaneseSoldierAATankSquad(TInt aXLocation, TBool aReflected)
{		
	CGroundUnitOverlordAI* lJapaneseOverlord = CGroundUnitOverlordAI::New(EConflictSideJapanese);
	
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseAA97ChiHaTank,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lJapaneseOverlord);
	
	return lJapaneseOverlord;
}

CGroundUnitOverlordAI* CJapaneseSquad::CreateJapaneseSoldierSquad(TInt aXLocation, TBool aReflected)
{		
	CGroundUnitOverlordAI* lJapaneseOverlord = CGroundUnitOverlordAI::New(EConflictSideJapanese);
	
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lJapaneseOverlord);
	
	return lJapaneseOverlord;
}

CGroundUnitOverlordAI* CJapaneseSquad::CreateJapaneseSoldierAndTankSquad(TInt aXLocation, TBool aReflected)
{		
	CGroundUnitOverlordAI* lJapaneseOverlord = CGroundUnitOverlordAI::New(EConflictSideJapanese);
	
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRocketman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapaneseRifleman,aXLocation,aReflected);
	CBasicSquad::AddGroundUnit(lJapaneseOverlord,EGameObjectIdentifierJapanese97ChiHaTank,aXLocation,aReflected);
	CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->AddMoveableObject(lJapaneseOverlord);
	
	return lJapaneseOverlord;
}

TInt CJapaneseSquad::GetWidthOfJapaneseSoldierAATankSquad()
{
	TInt lTotalWidth = 0;
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseAA97ChiHaTank);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	return lTotalWidth;
}

TInt CJapaneseSquad::GetWidthOfJapaneseSoldierSquad()
{
	TInt lTotalWidth = 0;
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);	
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);	
	return lTotalWidth;
}

TInt CJapaneseSquad::GetWidthOfJapaneseSoldierAndTankSquad()
{
	TInt lTotalWidth = 0;
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapanese97ChiHaTank);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRocketman);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseRifleman);
	return lTotalWidth;
}

TInt CJapaneseSquad::GetWidthOfJapaneseTankSquad()
{
	TInt lTotalWidth = 0;
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseAA97ChiHaTank);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapanese97ChiHaTank);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapaneseAA97ChiHaTank);
	lTotalWidth += CBasicSquad::GetTotalWidthOfUnit(EGameObjectIdentifierJapanese97ChiHaTank);
	return lTotalWidth;
}
