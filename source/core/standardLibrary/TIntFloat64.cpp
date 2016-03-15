/*
 * TIntFloat64.cpp
 *
 *  Created on: May 25, 2012
 *      Author: dstoll
 */

#include "includes/core/standardLibrary/TIntFloat64.h"
#include "includes/core/standardLibrary/TIntFloat.h"

//keep this internal to avoid errors
#define SHIFT_FACTOR_TIntFloat64 16

TIntFloat64::TIntFloat64()
{
	iIntegerValue = 0;
}

//TIntFloat64::TIntFloat64(const TInt64& aValue)
//{
//	iIntegerValue = aValue;
//}

TIntFloat64::TIntFloat64(const TIntFloat& aValue)
{
	iIntegerValue = (TInt64) aValue.GetIntInBaseIntFloat();
}

TIntFloat64::TIntFloat64(const TIntFloat64& aValue)
{
	iIntegerValue = aValue.iIntegerValue;
}

 TIntFloat64& TIntFloat64::operator=(const TIntFloat64& lRightSideValue)
{
	iIntegerValue = lRightSideValue.iIntegerValue;
	return *this;
}

 TIntFloat64& TIntFloat64::operator=(const TIntFloat& lRightSideValue)
{
	iIntegerValue = lRightSideValue.GetIntInBaseIntFloat();
	return *this;
}

// TIntFloat64& TIntFloat64::operator=(const TInt& lRightSideValue)
//{
//	iIntegerValue = lRightSideValue << SHIFT_FACTOR_TIntFloat64;
//	return *this;
//}

 TIntFloat64& TIntFloat64::operator+=(const TIntFloat64& lRightSideValue)
{
	this->iIntegerValue += lRightSideValue.iIntegerValue;
	return *this;
}

 TIntFloat64& TIntFloat64::operator+=(const TIntFloat& lRightSideValue)
{
	this->iIntegerValue += lRightSideValue.GetIntInBaseIntFloat();
	return *this;
}

 TIntFloat64& TIntFloat64::operator+=(const TInt& lRightSideValue)
{
	this->iIntegerValue += lRightSideValue << SHIFT_FACTOR_TIntFloat64;
	return *this;
}

 TIntFloat64& TIntFloat64::operator+=(const TDouble& lRightSideValue)
 {
 	this->iIntegerValue += lRightSideValue * (1 << SHIFT_FACTOR_TIntFloat64);
 	return *this;
 }

 TIntFloat64& TIntFloat64::operator-=(const TIntFloat64& lRightSideValue)
{
	this->iIntegerValue -= lRightSideValue.iIntegerValue;
	return *this;
}

 TIntFloat64& TIntFloat64::operator-=(const TIntFloat& lRightSideValue)
{
	this->iIntegerValue -= lRightSideValue.GetIntInBaseIntFloat();
	return *this;
}

 TIntFloat64& TIntFloat64::operator-=(const TInt& lRightSideValue)
{
	this->iIntegerValue -= lRightSideValue << SHIFT_FACTOR_TIntFloat64;
	return *this;
}


 TIntFloat64& TIntFloat64::operator-=(const TDouble& lRightSideValue)
 {
 	this->iIntegerValue -= lRightSideValue * (1 << SHIFT_FACTOR_TIntFloat64);
 	return *this;
 }

 TIntFloat64& TIntFloat64::operator*=(const TIntFloat64& lRightSideValue)
{
	this->iIntegerValue = (this->iIntegerValue * lRightSideValue.iIntegerValue) >> SHIFT_FACTOR_TIntFloat64;
	return *this;
}

 TIntFloat64& TIntFloat64::operator*=(const TIntFloat& lRightSideValue)
{
	this->iIntegerValue = ((TInt64) this->iIntegerValue * lRightSideValue.GetIntInBaseIntFloat()) >> SHIFT_FACTOR_TIntFloat64;
	return *this;
}

 TIntFloat64& TIntFloat64::operator*=(const TInt& lRightSideValue)
{
	this->iIntegerValue = this->iIntegerValue * lRightSideValue; //don't need to shiftup since we are already in the right base
	return *this;
}

 TIntFloat64& TIntFloat64::operator*=(const TDouble& lRightSideValue)
 {
 	this->iIntegerValue = ((TInt64) this->iIntegerValue * ConvertFromDouble(lRightSideValue).iIntegerValue) >> SHIFT_FACTOR_TIntFloat64;
 	return *this;
 }

 TIntFloat64& TIntFloat64::operator/=(const TIntFloat64& lRightSideValue)
{
	this->iIntegerValue = (this->iIntegerValue << SHIFT_FACTOR_TIntFloat64) / lRightSideValue.iIntegerValue; //shift the upper argument by 16 bit, since dividing by a 16 bit number will shift the number left again
	return *this;
}

 TIntFloat64& TIntFloat64::operator/=(const TIntFloat& lRightSideValue)
{
	this->iIntegerValue = (TInt64) (this->iIntegerValue << SHIFT_FACTOR_TIntFloat64) / lRightSideValue.GetIntInBaseIntFloat(); //shift the upper argument by 16 bit, since dividing by a 16 bit number will shift the number left again
	return *this;
}

 TIntFloat64& TIntFloat64::operator/=(const TInt& lRightSideValue)
{
	this->iIntegerValue = this->iIntegerValue / lRightSideValue; //don't need to shiftup since we are already in the right base
	return *this;
}

 TIntFloat64& TIntFloat64::operator/=(const TDouble& lRightSideValue)
 {
 	this->iIntegerValue = ((TInt64) this->iIntegerValue << SHIFT_FACTOR_TIntFloat64) / ConvertFromDouble(lRightSideValue).iIntegerValue; //shift the upper argument by 16 bit, since dividing by a 16 bit number will shift the number left again
 	return *this;
 }

 TIntFloat64 TIntFloat64::operator+(const TIntFloat64& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy += lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator+(const TIntFloat& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy += lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator+(const TInt& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy += lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator+(const TDouble& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy += lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator-(const TIntFloat64& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy -= lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator-(const TIntFloat& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy -= lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator-(const TInt& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy -= lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator-(const TDouble& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy -= lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator-() const
{
	TIntFloat64 lCopy = *this;
	return lCopy *= -1;
}

 TIntFloat64 TIntFloat64::operator*(const TIntFloat64& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy *= lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator*(const TIntFloat& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy *= lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator*(const TInt& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy *= lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator*(const TDouble& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy *= lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator/(const TIntFloat64& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy /= lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator/(const TIntFloat& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy /= lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator/(const TInt& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy /= lRightSideValue;
	return lCopy;
}

 TIntFloat64 TIntFloat64::operator/(const TDouble& lRightSideValue) const
{
	TIntFloat64 lCopy = *this;
	lCopy /= lRightSideValue;
	return lCopy;
}

 TBool TIntFloat64::operator==(const TIntFloat64& lRightSideValue) const
{
	return this->iIntegerValue == lRightSideValue.iIntegerValue;
}

 TBool TIntFloat64::operator==(const TIntFloat& lRightSideValue) const
{
	return this->iIntegerValue == lRightSideValue.GetIntInBaseIntFloat();
}

 TBool TIntFloat64::operator==(const TInt& lRightSideValue) const
{
	return (this->iIntegerValue >> SHIFT_FACTOR_TIntFloat64) == lRightSideValue; //shift down to lose precision
}

 TBool TIntFloat64::operator==(const TDouble& lRightSideValue) const
 {
 	return (this->iIntegerValue) == ConvertFromDouble(lRightSideValue).iIntegerValue; //shift down to lose precision
 }

 TBool TIntFloat64::operator!=(const TIntFloat64& lRightSideValue) const
{
	return !(*this == lRightSideValue);
}

 TBool TIntFloat64::operator!=(const TIntFloat& lRightSideValue) const
{
	return !(*this == lRightSideValue);
}

 TBool TIntFloat64::operator!=(const TInt& lRightSideValue) const
{
	return !(*this == lRightSideValue);
}

 TBool TIntFloat64::operator!=(const TDouble& lRightSideValue) const
 {
 	return (this->iIntegerValue) != ConvertFromDouble(lRightSideValue).iIntegerValue; //shift down to lose precision
 }

 TBool TIntFloat64::operator>(const TIntFloat64& lRightSideValue) const
{
	return iIntegerValue > lRightSideValue.iIntegerValue;
}

 TBool TIntFloat64::operator>(const TIntFloat& lRightSideValue) const
{
	return iIntegerValue > lRightSideValue.GetIntInBaseIntFloat();
}

 TBool TIntFloat64::operator>(const TInt& lRightSideValue) const
{
	return iIntegerValue > lRightSideValue << SHIFT_FACTOR_TIntFloat64;
}

 TBool TIntFloat64::operator>(const TDouble& lRightSideValue) const
 {
 	return (this->iIntegerValue) > ConvertFromDouble(lRightSideValue).iIntegerValue; //shift down to lose precision
 }

 TBool TIntFloat64::operator>=(const TIntFloat64& lRightSideValue) const
{
	return iIntegerValue >= lRightSideValue.iIntegerValue;
}
 TBool TIntFloat64::operator>=(const TIntFloat& lRightSideValue) const
{
	return iIntegerValue >= lRightSideValue.GetIntInBaseIntFloat();
}

 TBool TIntFloat64::operator>=(const TInt& lRightSideValue) const
{
	return iIntegerValue >= lRightSideValue << SHIFT_FACTOR_TIntFloat64;
}

 TBool TIntFloat64::operator>=(const TDouble& lRightSideValue) const
 {
 	return (this->iIntegerValue) >= ConvertFromDouble(lRightSideValue).iIntegerValue; //shift down to lose precision
 }

 TBool TIntFloat64::operator<(const TIntFloat64& lRightSideValue) const
{
	return iIntegerValue < lRightSideValue.iIntegerValue;
}

 TBool TIntFloat64::operator<(const TIntFloat& lRightSideValue) const
{
	return iIntegerValue < lRightSideValue.GetIntInBaseIntFloat();
}

 TBool TIntFloat64::operator<(const TInt& lRightSideValue) const
{
	return iIntegerValue < lRightSideValue << SHIFT_FACTOR_TIntFloat64;
}

 TBool TIntFloat64::operator<(const TDouble& lRightSideValue) const
 {
 	return (this->iIntegerValue) < ConvertFromDouble(lRightSideValue).iIntegerValue; //shift down to lose precision
 }

 TBool TIntFloat64::operator<=(const TIntFloat64& lRightSideValue) const
{
	return iIntegerValue <= lRightSideValue.iIntegerValue;
}

 TBool TIntFloat64::operator<=(const TIntFloat& lRightSideValue) const
{
	return iIntegerValue <= lRightSideValue.GetIntInBaseIntFloat();
}

 TBool TIntFloat64::operator<=(const TInt& lRightSideValue) const
{
	return iIntegerValue <= lRightSideValue << SHIFT_FACTOR_TIntFloat64;
}

 TBool TIntFloat64::operator<=(const TDouble& lRightSideValue) const
 {
 	return (this->iIntegerValue) <= ConvertFromDouble(lRightSideValue).iIntegerValue; //shift down to lose precision
 }

 void TIntFloat64::AssignIntAlreadyInBaseIntFloat64(const TInt64& aValue)
 {
 	iIntegerValue = aValue;
 }

 TIntFloat64 TIntFloat64::Convert(const TInt64& aValueInBaseInt)
 {
	 TIntFloat64 lValueAsIntFloat;
 	lValueAsIntFloat.AssignIntAlreadyInBaseIntFloat64(aValueInBaseInt << SHIFT_FACTOR_TIntFloat64);
 	return lValueAsIntFloat;
 }

 TIntFloat64 TIntFloat64::ConvertFromDouble(const TDouble& aValueInBaseDouble)
 {
		TIntFloat64 lValueAsIntFloat64;
		TInt lShiftValue = 1 << SHIFT_FACTOR_TIntFloat64;
		TInt lResult = (TInt) aValueInBaseDouble * lShiftValue;
		lValueAsIntFloat64.AssignIntAlreadyInBaseIntFloat64(lResult);
		return lValueAsIntFloat64;
 }

 TInt64 TIntFloat64::GetIntInBaseIntFloat64() const
{
	return iIntegerValue;
}

 TInt64 TIntFloat64::GetIntInBaseInt() const
{
	return iIntegerValue >> SHIFT_FACTOR_TIntFloat64;
}
