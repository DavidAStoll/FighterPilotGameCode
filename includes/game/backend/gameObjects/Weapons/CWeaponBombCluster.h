/*
 ============================================================================
 Name		: WeaponBombCluster.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CWeaponBombCluster declaration
 ============================================================================
 */

#ifndef WEAPONBOMBCLUSTER_H
#define WEAPONBOMBCLUSTER_H


// CLASS DECLARATION
#include "includes/game/backend/basicClasses/CWeaponBomb.h"

/**
 *  CWeaponBombCluster
 * 
 */
class CWeaponBombCluster : public CWeaponBomb
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CWeaponBombCluster();

	/**
	 * Two-phased constructor.
	 */
	static CWeaponBombCluster* New(TInt aAmmunition,TPointIntFloat* aLocation,TInt* aAngel,TBool* aUpSideDown,TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide,
            TBool aDoNotDraw,TIntFloat* aObjectSpeed,TInt aTimerFrames);
        virtual void Shoot();
        virtual void Explode();
        virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
        virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CWeaponBombCluster(TInt aAmmunition,TPointIntFloat* aLocation,TInt* aAngel,TBool* aUpSideDown,TInt aXOffset, TInt aYOffset,TBool* aReflected,TConflictSide* aConflictSide,
            TBool aDoNotDraw,TIntFloat* aObjectSpeed,TInt aTimerFrames);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
	TInt iTimerFrames;

	};

#endif // WEAPONBOMBCLUSTER_H
