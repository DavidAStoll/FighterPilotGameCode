/*
 ============================================================================
 Name		: Explosive75mmShell.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosive75mmShell declaration
 ============================================================================
 */

#ifndef EXPLOSIVE75MMSHELL_H
#define EXPLOSIVE75MMSHELL_H

#include "includes/game/backend/basicClasses/CProjectile.h"

// CLASS DECLARATION

/**
 *  CExplosive75mmShell
 * 
 */
class CExplosive75mmShell: public CProjectile
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosive75mmShell();

	/**
	 * Two-phased constructor.
	 */
	static CExplosive75mmShell* New(TPointIntFloat aLocation, TInt aAngel, TBool aReflected, TInt aRange);
	static void InitilizeExplosive75mmShell();
	static void RemoveExplosive75mmShell();
	virtual void Destruct(); //called when range is less than 0,might cause an explosion or just remove the object

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CExplosive75mmShell(TPoint aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TInt aRange);
	CExplosive75mmShell(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TInt aAngel, TInt aRange);
	static void LoadExplosive75mmShellTextures();
	static void UnloadExplosive75mmShellTextures();
	void ConstructL();

};

#endif // EXPLOSIVE75MMSHELL_H
