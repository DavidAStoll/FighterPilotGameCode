/*
 * CCryptographyEngima.h
 *
 *  Created on: Nov 6, 2012
 *      Author: dstoll
 */

#ifndef CCryptographyEngima_H_
#define CCryptographyEngima_H_

#include "includes/core/standardLibrary/StandardLibrary.h"

class CCryptographyEngima
{
public:
	static CCryptographyEngima* New(TUint aNumberOfWheels);
	virtual ~CCryptographyEngima();

	//takes an Array of Ints and encodes them using the engima algorithm
	CPointerArray<TInt>* EncodeData(CPointerArray<TInt>& aDataToEndcode);
	//takes an Array of Ints that have previously be encoded and decodes it
	CPointerArray<TInt>* DecodeData(CPointerArray<TInt>& aDataToDecode);

protected:
	CCryptographyEngima(TUint aNumberOfWheels);
	void Construct();

	//Encodes a Single Byte using the wheels, wheels Offset will be changed after calling this function
	TUint8 EncodeByte(TUint8 aValue);
	//Decodes a SingleByte using the wheels, Offset will change after this call
	TUint8 DecodeByte(TUint8 aValue);
	//returns OffsetValue
	TUint8 GetOffsetValueForCodeValue(TUint8 aCodeValue);

	TInt iInitialOffset;
	TUint iNumberOfWheels; //how many code wheels do we want to use during encoding
	TInt* iCodeWheelOffsets;
	TUint8 iCodeValues[256]; //contains the code values for encoding data
};

#endif /* CCryptographyEngima_H_ */
