/*
 * CProjectileBomb.h
 *
 *  Created on: Aug 28, 2012
 *      Author: dstoll
 */

#ifndef CPROJECTILEBOMB_H_
#define CPROJECTILEBOMB_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CProjectile.h"

// CLASS DECLARATION

/**
 *  CProjectileBomb
 * Base class for all bombs, since all bombs share the same update function in general, which just updates the speed downward
 */
class CProjectileBomb: public CProjectile
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CProjectileBomb();

	virtual void Update();
	void ProjectileBombObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
	void ProjectileBombObjectLoadContentFromDisk(CFileReadStream &aReadStream);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	TInt iCurrentFrame;
	/**
	 * Constructor for performing 1st stage construction
	 */
	CProjectileBomb(TPointIntFloat aLocation, SGameObjectAttributes& aAttributes, TIntFloat aSpeed, TInt aAngel);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

};

#endif /* CPROJECTILEBOMB_H_ */
