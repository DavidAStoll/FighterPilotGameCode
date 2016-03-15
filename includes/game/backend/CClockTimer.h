/*
 * CClockTimer.h
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#ifndef CCLOCKTIMER_H_
#define CCLOCKTIMER_H_

// INCLUDES
#include "includes/core/standardLibrary/StandardLibrary.h"
#include "includes/core/graphic/CGraphicsFont.h"

// CLASS DECLARATION

/**
 *  CClockTimer
 *
 */
class CClockTimer
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CClockTimer();
	static CClockTimer* New(TInt aMinutes, TInt aSeconds,TPoint aPosition,TColour aDrawColor);

	void Draw();
	void DecreaseTimerBySecond();
	void DecreaseTimerByMinute();
	void SetNewTime(TInt aMinutes, TInt aSeconds);
	void SaveOnDisk(CFileWriteStream &aOutputStream);
	void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CClockTimer(TInt aMinutes, TInt aSeconds,TPoint aPosition,TColour aDrawColor);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	TInt iCurrentSeconds;
	TInt iCurrentMinutes;
	TPoint iPosition;
	TColour iDrawColor;
	SFont iFont;

	};


#endif /* CCLOCKTIMER_H_ */
