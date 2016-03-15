/*
 ============================================================================
 Name		: FlagPole.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CFlagPole declaration
 ============================================================================
 */

#ifndef FLAGPOLE_H
#define FLAGPOLE_H

// INCLUDES
#include "includes/game/backend/basicClasses/CBuilding.h"
// CLASS DECLARATION

/**
 *  CFlagPole
 * 
 */
class CFlagPole: public CBuilding
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CFlagPole();

	/**
	 * Two-phased constructor.
	 */
	static CFlagPole* New(TPoint aLocation, TConflictSide aConflictSide);
	static CFlagPole* New(TInt aXLocation, TConflictSide aConflictSide);

	static void InitilizeFlagPole();
	static void RemoveFlagPole();
	void ChangeFlagConflictSide(TConflictSide aSide);
	virtual void Update();
	virtual void DrawAtPositionWithValues(TPointIntFloat& aPosition, TIntFloat aWidth, TIntFloat aHeight);
	virtual void Draw();
	void Rotate(TInt aDegree);
	virtual void SetPosition(TPoint& aPoint);
	virtual void SetPosition(TPointIntFloat& aPoint);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CFlagPole(TPoint aLocation, SGameObjectAttributes& aAttributes);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	static void LoadFlagPoleTextures();
	static void UnloadFlagPoleTextures();

	CTextureObject* iFlagPole;
	TInt iAngle;

};

#endif // FLAGPOLE_H
