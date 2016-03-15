/*
 * CBombingGuidingSystem.h
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#ifndef CBOMBINGGUIDINGSYSTEM_H_
#define CBOMBINGGUIDINGSYSTEM_H_

#include "includes/core/standardLibrary/StandardLibrary.h"

// INCLUDES
class CAircraft;

class CBombingGuidingSystem
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CBombingGuidingSystem();

	static CBombingGuidingSystem* New(CAircraft* aOwner,TInt aXOffSet,TInt aYOffSet);
	void Draw();
	void AssignAircraft(CAircraft* aOwner);

private:

	CBombingGuidingSystem(CAircraft* aOwner,TInt aXOffSet,TInt aYOffSet);
	void ConstructL();

	CAircraft* iAircraft;
	TInt iXOffset;
	TInt iYOffset;
	};

#endif /* CBOMBINGGUIDINGSYSTEM_H_ */
