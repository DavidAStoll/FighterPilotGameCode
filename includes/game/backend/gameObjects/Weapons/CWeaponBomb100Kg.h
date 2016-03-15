/*
 ============================================================================
 Name		: WeaponBomb100Kg.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWeaponBomb100Kg declaration
 ============================================================================
 */

#ifndef WEAPONBOMB100KG_H
#define WEAPONBOMB100KG_H

// CLASS DECLARATION

#include "includes/game/backend/basicClasses/CWeaponBomb.h"

/**
 *  CWeaponBomb100Kg
 * 
 */
class CWeaponBomb100Kg: public CWeaponBomb
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponBomb100Kg();

	/**
	 * Two-phased constructor.
	 */
	static CWeaponBomb100Kg* New(TInt aAmmunition, const TPointIntFloat* aLocation, const TInt* aAngel, const TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide, TBool aDoNotDraw, const TIntFloat* aObjectSpeed);
	virtual TIntFloat GetXMiddlePointOfWeapon();
	virtual void Shoot();
	virtual void Explode();
	virtual void Draw();// need to override default behaviour since we have another weapon
	virtual void Update();// need to override default behaviour since we have another weapon
	virtual void ResetAmmunitionToMaxValue();
	virtual void SetAmmunitionToValue(TInt aAmount);//resets the ammunition to its default
	virtual void SetDisabled(TBool aValue);//disables or enables the gun
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CWeaponBomb100Kg(TInt aAmmunition, const TPointIntFloat* aLocation, const TInt* aAngel, const TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide, TBool aDoNotDraw, const TIntFloat* aObjectSpeed);
	CWeaponBomb100Kg(TInt aAmmunition, const TPointIntFloat* aLocation, const TInt* aAngel, const TBool* aUpSideDown, TInt aXOffset, TInt aYOffset, const TBool* aReflected, const TConflictSide* aConflictSide, TBool aDoNotDraw, const TIntFloat* aObjectSpeed, TBool aCreateAnotherWeapon);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	TBool iCreateAnotherWeapon;
	CWeaponBomb100Kg* iExtraWeapon;

};

#endif // WEAPONBOMB100KG_H
