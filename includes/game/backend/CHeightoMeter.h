/*
 * CHeightoMeter.h
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#ifndef CHEIGHTOMETER_H_
#define CHEIGHTOMETER_H_

#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/graphic/CGraphicsFont.h"

class CAircraft;

#define HEIGHTOMETER_HEIGHT_STEP_INTERVAL 50
#define HEIGHTOMETER_X_OFFSET 954
#define HEIGHTOMETER_Y_OFFSET 250
#define HEIGHTOMETER_FONT_SIZE 40

// CLASS DECLARATION

/**
 *  CHeightoMeter
 *
 */
class CHeightoMeter
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CHeightoMeter();

	static CHeightoMeter* New(CAircraft* aAircraft);
	void AssignAircraft(CAircraft* aNewAircraft);
	void Draw();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CHeightoMeter(CAircraft* aAircraft);

	CAircraft* iOwnedAircraft;
	CString* iHeightString;
	SFont iFont;

	};


#endif /* CHEIGHTOMETER_H_ */
