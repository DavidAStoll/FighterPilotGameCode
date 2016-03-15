/*
 ============================================================================
 Name		: Airport.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CAirport declaration
 ============================================================================
 */

#ifndef AIRPORT_H
#define AIRPORT_H

// CLASS DECLARATION

#include "includes/game/backend/basicClasses/CBuilding.h"

class CLandingSurface;
class CHangar;
class CFlagPole;

/**
 *  CAirport
 * 
 */
class CAirport: public CBuilding
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAirport();

	/**
	 * Two-phased constructor.
	 */
	static CAirport* CreateLoadAirport();//only used as a temp object, used to create an object for loading puposes
	static CAirport* New(TPoint aLocation, TConflictSide aConflictSide);
	static CAirport* New(TInt aXLocation, TConflictSide aConflictSide);

	virtual void Update(); //runs the Game Object's logic code to update its member data
	void SetTargetPriority(TUint aTargetPriority);
	void SetTarget(CGameObject* aTarget);
	void SetSpawnPlanes(TBool aValue); //enable or disable the spawning of planes for this airport
	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType); // method call from outside to inflict damage on the object, might have special death animation for specific damage
	CAircraft* CreateNewAircraft(); //creates a new aircraft and positions it on the deck of the carrier
	virtual void PossibleCollision(CInterval*& aInterval);
	static void InitilizeAirport();
	static void RemoveAirport();
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop

	/**
	 * Constructor for performing 1st stage construction
	 */
	CAirport(TPoint aLocation, SGameObjectAttributes& aAttributes);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	void ConstructWithoutObjectCreationL();

	static void LoadAirportTextures();
	static void UnloadAirportTextures();

	CLandingSurface* iLandingSurface;
	CHangar* iHangar1;//holds planes
	CHangar* iHangar2;//holds planes
	CFlagPole* iFlagPole;
	CAircraft* iAircraft; //pointer to the current owned aircaft
	CGameObject* iTarget;
	TInt iFramesLeftUntilSpawnPlane;
	TUint iTargetPriority;
	TBool iSpawnPlanes;

};

#endif // AIRPORT_H
