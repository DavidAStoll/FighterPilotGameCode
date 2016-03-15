/*
 ============================================================================
 Name		: JapaneseSquad.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CJapaneseSquad declaration
 ============================================================================
 */

#ifndef JAPANESESQUAD_H
#define JAPANESESQUAD_H

// INCLUDES
#include "includes/core/standardLibrary/StandardLibrary.h"

class CGroundUnitOverlordAI;

// CLASS DECLARATION

/**
 *  CJapaneseSquad
 * 
 */

enum TJapaneseSquadTypes
{
	EJapaneseSquadTypeSoldierSquad,EJapaneseSquadTypeSoldierAATankSquad,EJapaneseSquadTypeSoldierAndTankSquad,EJapaneseSquadTypeTankSquad
};

class CJapaneseSquad
	{
public:
	
	static CGroundUnitOverlordAI* CreateJapaneseSoldierAATankSquad(TInt aXLocation, TBool aReflected);
	static CGroundUnitOverlordAI* CreateJapaneseSoldierSquad(TInt aXLocation, TBool aReflected);
	static CGroundUnitOverlordAI* CreateJapaneseSoldierAndTankSquad(TInt aXLocation, TBool aReflected);
	static CGroundUnitOverlordAI* CreateJapaneseTankSquad(TInt aXLocation, TBool aReflected);
	static TInt GetWidthOfJapaneseSoldierAATankSquad();
	static TInt GetWidthOfJapaneseSoldierSquad();
	static TInt GetWidthOfJapaneseSoldierAndTankSquad();
	static TInt GetWidthOfJapaneseTankSquad();
	};

#endif // JAPANESESQUAD_H
