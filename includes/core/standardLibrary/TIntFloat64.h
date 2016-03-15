/*
 * TIntFloat64.h
 *
 *  Created on: May 25, 2012
 *      Author: dstoll
 */

//Extactly the same as an TIntfloat except that it can holder bigger numbers since it uses a 64 bit TInt
#ifndef TINTFLOAT64_H_
#define TINTFLOAT64_H_

#include "includes/core/standardLibrary/BasicTypes.h"

class TIntFloat;

class TIntFloat64
{
public:

	TIntFloat64();
	//TIntFloat64(const TInt64& aValue);
	TIntFloat64(const TIntFloat& aValue);
	TIntFloat64(const TIntFloat64& aValue);
	TIntFloat64& operator=(const TIntFloat64& lRightSideValue);
	TIntFloat64& operator=(const TIntFloat& lRightSideValue);
	//TIntFloat64& operator=(const TInt& lRightSideValue);
	TIntFloat64& operator+=(const TIntFloat64& lRightSideValue);
	TIntFloat64& operator+=(const TIntFloat& lRightSideValue);
	TIntFloat64& operator+=(const TInt& lRightSideValue);
	TIntFloat64& operator+=(const TDouble& lRightSideValue);
	TIntFloat64& operator-=(const TIntFloat64& lRightSideValue);
	TIntFloat64& operator-=(const TIntFloat& lRightSideValue);
	TIntFloat64& operator-=(const TInt& lRightSideValue);
	TIntFloat64& operator-=(const TDouble& lRightSideValue);
	TIntFloat64& operator*=(const TIntFloat64& lRightSideValue);
	TIntFloat64& operator*=(const TIntFloat& lRightSideValue);
	TIntFloat64& operator*=(const TInt& lRightSideValue);
	TIntFloat64& operator*=(const TDouble& lRightSideValue);
	TIntFloat64& operator/=(const TIntFloat64& lRightSideValue);
	TIntFloat64& operator/=(const TIntFloat& lRightSideValue);
	TIntFloat64& operator/=(const TInt& lRightSideValue);
	TIntFloat64& operator/=(const TDouble& lRightSideValue);
	TIntFloat64 operator+(const TIntFloat64& lRightSideValue) const;
	TIntFloat64 operator+(const TIntFloat& lRightSideValue) const;
	TIntFloat64 operator+(const TInt& lRightSideValue) const;
	TIntFloat64 operator+(const TDouble& lRightSideValue) const;
	TIntFloat64 operator-(const TIntFloat64& lRightSideValue) const;
	TIntFloat64 operator-(const TIntFloat& lRightSideValue) const;
	TIntFloat64 operator-(const TInt& lRightSideValue) const;
	TIntFloat64 operator-(const TDouble& lRightSideValue) const;
	TIntFloat64 operator-() const;
	TIntFloat64 operator*(const TIntFloat64& lRightSideValue) const;
	TIntFloat64 operator*(const TIntFloat& lRightSideValue) const;
	TIntFloat64 operator*(const TInt& lRightSideValue) const;
	TIntFloat64 operator*(const TDouble& lRightSideValue) const;
	TIntFloat64 operator/(const TIntFloat64& lRightSideValue) const;
	TIntFloat64 operator/(const TIntFloat& lRightSideValue) const;
	TIntFloat64 operator/(const TInt& lRightSideValue) const;
	TIntFloat64 operator/(const TDouble& lRightSideValue) const;
	TBool operator==(const TIntFloat64& lRightSideValue) const;
	TBool operator==(const TIntFloat& lRightSideValue) const ;
	TBool operator==(const TInt& lRightSideValue) const;
	TBool operator==(const TDouble& lRightSideValue) const;
	TBool operator!=(const TIntFloat64& lRightSideValue) const;
	TBool operator!=(const TIntFloat& lRightSideValue) const;
	TBool operator!=(const TInt& lRightSideValue) const;
	TBool operator!=(const TDouble& lRightSideValue) const;
	TBool operator>(const TIntFloat64& lRightSideValue) const;
	TBool operator>(const TIntFloat& lRightSideValue) const;
	TBool operator>(const TInt& lRightSideValue) const;
	TBool operator>(const TDouble& lRightSideValue) const;
	TBool operator>=(const TIntFloat64& lRightSideValue) const;
	TBool operator>=(const TIntFloat& lRightSideValue) const;
	TBool operator>=(const TInt& lRightSideValue) const;
	TBool operator>=(const TDouble& lRightSideValue) const;
	TBool operator<(const TIntFloat64& lRightSideValue) const;
	TBool operator<(const TIntFloat& lRightSideValue) const;
	TBool operator<(const TInt& lRightSideValue) const;
	TBool operator<(const TDouble& lRightSideValue) const;
	TBool operator<=(const TIntFloat64& lRightSideValue) const;
	TBool operator<=(const TIntFloat& lRightSideValue) const;
	TBool operator<=(const TInt& lRightSideValue) const;
	TBool operator<=(const TDouble& lRightSideValue) const;
	//this Int will no be shifted into base and will replace the IntegerValue directly
	void AssignIntAlreadyInBaseIntFloat64(const TInt64& aValue);
	//make use of this special function to convert TInt to TIntFloat64 to avoid typos
	static TIntFloat64 Convert(const TInt64& aValueInBaseInt);
	//make use of this special function to convert TDouble to TIntFloat
	static TIntFloat64 ConvertFromDouble(const TDouble& aValueInBaseDouble);
	//returns the integer value shifted into the IntFloat Base
	TInt64 GetIntInBaseIntFloat64() const;
	//returns the integers value in Base Int, just a normal int without the Float component
	TInt64 GetIntInBaseInt() const;

private:
	TInt64 iIntegerValue;
};

#endif /* TINTFLOAT64_H_ */
