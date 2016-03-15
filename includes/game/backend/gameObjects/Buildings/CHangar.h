/*
 ============================================================================
 Name		: Hangar.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CHangar declaration
 ============================================================================
 */

#ifndef HANGAR_H
#define HANGAR_H

// CLASS DECLARATION

#include "includes/game/backend/basicClasses/CBuilding.h"

/**
 *  CHangar
 * 
 */

class CHangar: public CBuilding
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CHangar();

	/**
	 * Two-phased constructor.
	 */
	static CHangar* New(TPoint aLocation);
	static CHangar* New(TInt aXLocation);

	static void InitilizeHangar();
	static void RemoveHangar();
	TInt GetPlanesLeft();
	TBool IsPlaneStillLeft();
	void RemovePlane();
	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType); // method call from outside to inflict damage on the object, might have special death animation for specific damage
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CHangar(TPoint aLocation, SGameObjectAttributes& aAttributes);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	static void LoadHangarTextures();
	static void UnloadHangarTextures();

	TInt iPlanesLeft;//how many planes are still left in the hangar

};

#endif // HANGAR_H
