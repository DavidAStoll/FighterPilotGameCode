/*
 ============================================================================
 Name		: Betty.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CBetty declaration
 ============================================================================
 */

#ifndef BETTY_H
#define BETTY_H

// INCLUDES
#include "includes/game/backend/basicClasses/CAircraft.h"
#include "includes/game/backend/gameObjects/AI/CHeavyBomberAI.h"

// CLASS DECLARATION

class CBackGunMachineGun8mm;
class CAircraftSmokeCreator;
class CHeavyBomberAI;
class CWeaponBomb;

class CBetty: public CAircraft
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBetty();

	static CBetty* New(TBool aReflected, TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation);

	static void InitilizeBetty();
	static void RemoveBetty();
	void AssignBombingMission(SHeavyBombingMission aMission);
	void RemoveBombs(); //in case we want that the betty has no bombs left
	virtual void UpdateAircraft();
	virtual void Draw();
	virtual void Fire1();
	virtual void Fire2();
	virtual TBool Fire1HasAmmunitionLeft();
	virtual TBool Fire2HasAmmunitionLeft();
	virtual CWeapon* GetPrimaryWeapon();
	virtual CWeapon* GetSecondaryWeapon();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CBetty(TIntFloat aCurrentAcceleration, TBool aFlying, TPoint aLocation, SGameObjectAttributes& aAttributes);
	void ConstructL();

	virtual void PlaneCrash();//causes secondary weapon to explode
	//functions intilize global data shared by all dauntless objects, such Bounding Collision points and textures
	static void LoadBettyTexturesL();
	static void UnloadBettyTextures();
	static void CreateBettyCollisionPointsL();
	static void RemoveBettyCollisionPoints();

	static SBoundingCollisionPoint* CollisionPoints;//holds the bounding points used for collision
	CBackGunMachineGun8mm* iFrontCannon;
	CBackGunMachineGun8mm* iTopCannon;
	CBackGunMachineGun8mm* iBackCannon;
	CBackGunMachineGun8mm* iBottomCannon;
	CAircraftSmokeCreator* iSmokeCreatorEngine1;
	CAircraftSmokeCreator* iSmokeCreatorEngine2;
	CWeaponBomb* iFirstHeavyBombWeapon;
	CWeaponBomb* iFirstMediumBombWeapon;
	CWeaponBomb* iSecondHeavyBombWeapon;
	CWeaponBomb* iSecondMediumBombWeapon;
	CHeavyBomberAI* iBombingAI;
};

#endif // BETTY_H
