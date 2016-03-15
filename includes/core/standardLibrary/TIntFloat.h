/*
 * TIntFloat.h
 *
 *  Created on: May 24, 2012
 *      Author: dstoll
 */

#ifndef TINTFLOAT_H_
#define TINTFLOAT_H_

#include "includes/core/standardLibrary/BasicTypes.h"

class TIntFloat64;

class TIntFloat
{
public:

	TIntFloat();
	//TIntFloat(const TInt& aValue);
	TIntFloat(const TIntFloat& aValue);
	TIntFloat(const TIntFloat64& aValue);
	//TIntFloat(const TDouble& aValue);
	TIntFloat& operator=(const TIntFloat& lRightSideValue);
	//TIntFloat& operator=(const TInt& lRightSideValue);
	TIntFloat& operator+=(const TIntFloat& lRightSideValue);
	TIntFloat& operator+=(const TInt& lRightSideValue);
	TIntFloat& operator+=(const TDouble& lRightSideValue);
	TIntFloat& operator-=(const TIntFloat& lRightSideValue);
	TIntFloat& operator-=(const TInt& lRightSideValue);
	TIntFloat& operator-=(const TDouble& lRightSideValue);
	TIntFloat& operator*=(const TIntFloat& lRightSideValue);
	TIntFloat& operator*=(const TInt& lRightSideValue);
	TIntFloat& operator*=(const TDouble& lRightSideValue);
	TIntFloat& operator/=(const TIntFloat& lRightSideValue);
	TIntFloat& operator/=(const TInt& lRightSideValue);
	TIntFloat& operator/=(const TDouble& lRightSideValue);
	TIntFloat operator+(const TIntFloat& lRightSideValue) const;
	TIntFloat operator+(const TInt& lRightSideValue) const;
	TIntFloat operator+(const TDouble& lRightSideValue) const;
	TIntFloat operator-(const TIntFloat& lRightSideValue) const;
	TIntFloat operator-(const TInt& lRightSideValue) const;
	TIntFloat operator-(const TDouble& lRightSideValue) const;
	TIntFloat operator-() const;
	TIntFloat operator*(const TIntFloat& lRightSideValue) const;
	TIntFloat operator*(const TInt& lRightSideValue) const;
	TIntFloat operator*(const TDouble& lRightSideValue) const;
	TIntFloat operator/(const TIntFloat& lRightSideValue) const;
	TIntFloat operator/(const TInt& lRightSideValue) const;
	TIntFloat operator/(const TDouble& lRightSideValue) const;
	TBool operator==(const TIntFloat& lRightSideValue) const;
	TBool operator==(const TInt& lRightSideValue) const;
	TBool operator==(const TDouble& lRightSideValue) const;
	TBool operator!=(const TIntFloat& lRightSideValue) const;
	TBool operator!=(const TInt& lRightSideValue) const;
	TBool operator!=(const TDouble& lRightSideValue) const;
	TBool operator>(const TIntFloat& lRightSideValue) const;
	TBool operator>(const TInt& lRightSideValue) const;
	TBool operator>(const TDouble& lRightSideValue) const;
	TBool operator>=(const TIntFloat& lRightSideValue) const;
	TBool operator>=(const TInt& lRightSideValue) const;
	TBool operator>=(const TDouble& lRightSideValue) const;
	TBool operator<(const TIntFloat& lRightSideValue) const;
	TBool operator<(const TInt& lRightSideValue) const;
	TBool operator<(const TDouble& lRightSideValue) const;
	TBool operator<=(const TIntFloat& lRightSideValue) const;
	TBool operator<=(const TInt& lRightSideValue) const;
	TBool operator<=(const TDouble& lRightSideValue) const;
	//this Int will no be shifted into base and will replace the IntegerValue directly
	void AssignIntAlreadyInBaseIntFloat(const TInt& aValue);
	//make use of this special function to convert TInt to TIntFloat to avoid typos
	static TIntFloat Convert(const TInt& aValueInBaseInt);
	//make use of this special function to convert TDouble to TIntFloat to avoid typos
	static TIntFloat Convert(const TDouble& aValueInBaseDouble);
	//returns a double that should have the same precison as the IntFloat, obviously is not scaled anymore
	TDouble GetAsDouble();
	//returns the integer value shifted into the IntFloat Base
	TInt GetIntInBaseIntFloat() const;
	//returns the integers value in Base Int, just a normal int without the Float component
	TInt GetIntInBaseInt() const;


private:
	TInt iIntegerValue;
};

#endif /* TINTFLOAT_H_ */
