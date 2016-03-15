/*
 * CRandomGenerator.h
 *
 *  Created on: Aug 10, 2012
 *      Author: dstoll
 */

#ifndef CRANDOMGENERATOR_H_
#define CRANDOMGENERATOR_H_

#include "includes/core/standardLibrary/StandardLibrary.h"

class CFramesPerSecondCounter;

#define CRANDOM_GENERATOR_NUMBER_OF_RANDOM_NUMBERS 1000

class CRandomGenerator
{
public:

	static CRandomGenerator* New(CFramesPerSecondCounter* aFramesPerSecondCounter);
	virtual ~CRandomGenerator();

	//returns a number that is between the LowerBound(inclusive) and UpperBound(exclusive)
	TInt GetRandomNumberTInt(TInt aLowerBound, TInt aUpperBoud);
	//returns a number that is between the LowerBound(inclusive) and UpperBound(exclusive)
	TIntFloat GetRandomNumberTIntFloat(TInt aLowerBound, TInt aUpperBoud);

protected:

	CRandomGenerator();
	void Construct(CFramesPerSecondCounter* aFramesPerSecondCounter);

  TIntFloat iRandomOffsetValueArray [CRANDOM_GENERATOR_NUMBER_OF_RANDOM_NUMBERS]; //has 200 random numbers stored
  TInt iIndex;
};

#endif /* CRANDOMGENERATOR_H_ */
