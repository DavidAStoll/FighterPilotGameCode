/*
 ============================================================================
 Name		: AmericanSquad.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CAmericanSquad declaration
 ============================================================================
 */

#ifndef AMERICANSQUAD_H
#define AMERICANSQUAD_H

// INCLUDES
#include "includes/core/standardLibrary/StandardLibrary.h"

class CGroundUnitOverlordAI;

enum TAmericanSquadTypes
{
	EAmericanSquadTypeSoldierSquad,EAmericanSquadTypeSoldierAATankSquad,EAmericanSquadTypeSoldierAndTankSquad,EAmericanSquadTypeTankSquad
};
// CLASS DECLARATION

/**
 *  CAmericanSquad
 * 
 */
class CAmericanSquad
	{
public:
	
	static CGroundUnitOverlordAI* CreateAmericanSoldierAATankSquad(TInt aXLocation, TBool aReflected);
	static CGroundUnitOverlordAI* CreateAmericanSoldierSquad(TInt aXLocation, TBool aReflected);
	static CGroundUnitOverlordAI* CreateAmericanSoldierAndTankSquad(TInt aXLocation, TBool aReflected);
	static CGroundUnitOverlordAI* CreateAmericanTankSquad(TInt aXLocation, TBool aReflected);
	static TInt GetWidthOfAmericanSoldierAATankSquad();
	static TInt GetWidthOfAmericanSoldierSquad(); //width 400
	static TInt GetWidthOfAmericanSoldierAndTankSquad(); //width 244
	static TInt GetWidthOfAmericanTankSquad();


	};

#endif // AMERICANSQUAD_H
