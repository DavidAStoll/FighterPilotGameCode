/*
 ============================================================================
 Name		: BasicSquad.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright � 2011 HQ Interactive Inc.
 Description : CBasicSquad declaration
 ============================================================================
 */

#ifndef BASICSQUAD_H
#define BASICSQUAD_H

// INCLUDES
#include "includes/game/backend/basicClasses/CGameObject.h"

class CGroundUnitOverlordAI;



// CLASS DECLARATION

/**
 *  CBasicSquad
 * 
 */
class CBasicSquad 
	{
public:

static void AddGroundUnit(CGroundUnitOverlordAI* aOverlord,TGameObjectIdentifier aId,TInt& aXPosition,TBool aReflected);
static TInt GetTotalWidthOfUnit(TGameObjectIdentifier aId);
	};

#endif // BASICSQUAD_H
