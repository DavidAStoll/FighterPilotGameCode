/*
 ============================================================================
 Name		: StandardRocketWeapon.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CStandardRocketWeapon declaration
 ============================================================================
 */

#ifndef STANDARDROCKETWEAPON_H
#define STANDARDROCKETWEAPON_H

#include "includes/game/backend/basicClasses/CWeaponBomb.h"

// CLASS DECLARATION

/**
 *  CStandardRocketWeapon
 * 
 */
class CStandardRocketWeapon : public CWeaponBomb
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CStandardRocketWeapon();

	/**
	 * Two-phased constructor.
	 */
	static CStandardRocketWeapon* New(TInt aAmmunition,TPointIntFloat* aLocation,TInt* aAngel,TBool* aUpSideDown,TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide,
            TBool aDoNotDraw,TIntFloat* aObjectSpeed,CGameObject* aCreator);

	virtual void Shoot();
	virtual void Explode();
	static void InitilizeStandardRocketWeapon();
	static void RemoveStandardRocketWeapon(); 

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CStandardRocketWeapon(TInt aAmmunition,TPointIntFloat* aLocation,TInt* aAngel,TBool* aUpSideDown,TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide,
            TBool aDoNotDraw,TIntFloat* aObjectSpeed,CGameObject* aCreator);
	
	static void LoadStandardRocketWeaponTextures();
	static void UnloadStandardRocketWeaponTextures();
	void ConstructL();
	
	CGameObject* iOwnerObject;

	};

#endif // STANDARDROCKETWEAPON_H
