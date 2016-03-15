/*
 * TIntFloat.cpp
 *
 *  Created on: May 25, 2012
 *      Author: dstoll
 */

#define SHIFT_FACTOR_TIntFloat 16

#include "includes/core/standardLibrary/TIntFloat.h"
#include "includes/core/standardLibrary/TIntFloat64.h"

TIntFloat::TIntFloat()
{
	iIntegerValue = 0;
}

//TIntFloat::TIntFloat(const TInt& aValue)
//{
//	iIntegerValue = aValue << SHIFT_FACTOR_TIntFloat;
//}

TIntFloat::TIntFloat(const TIntFloat& aValue)
{
	iIntegerValue = aValue.iIntegerValue;
}

TIntFloat::TIntFloat(const TIntFloat64& aValue)
{
	iIntegerValue = aValue.GetIntInBaseIntFloat64();
}

//TIntFloat::TIntFloat(const TDouble& aValue)
//{
//	TInt lShiftValue = 1 << SHIFT_FACTOR_TIntFloat;
//	iIntegerValue = (TInt) aValue * lShiftValue;
//}

TIntFloat& TIntFloat::operator=(const TIntFloat& lRightSideValue)
{
	iIntegerValue = lRightSideValue.iIntegerValue;
	return *this;
}

//TIntFloat& TIntFloat::operator=(const TInt& lRightSideValue)
//{
//	iIntegerValue = lRightSideValue << SHIFT_FACTOR_TIntFloat;
//	return *this;
//}

TIntFloat& TIntFloat::operator+=(const TIntFloat& lRightSideValue)
{
	this->iIntegerValue += lRightSideValue.iIntegerValue;
	return *this;
}

TIntFloat& TIntFloat::operator+=(const TInt& lRightSideValue)
{
	this->iIntegerValue += lRightSideValue << SHIFT_FACTOR_TIntFloat;
	return *this;
}

TIntFloat& TIntFloat::operator+=(const TDouble& lRightSideValue)
{
	this->iIntegerValue += lRightSideValue * (1 << SHIFT_FACTOR_TIntFloat);
	return *this;
}

TIntFloat& TIntFloat::operator-=(const TIntFloat& lRightSideValue)
{
	this->iIntegerValue -= lRightSideValue.iIntegerValue;
	return *this;
}

TIntFloat& TIntFloat::operator-=(const TInt& lRightSideValue)
{
	this->iIntegerValue -= lRightSideValue << SHIFT_FACTOR_TIntFloat;
	return *this;
}

TIntFloat& TIntFloat::operator-=(const TDouble& lRightSideValue)
{
	this->iIntegerValue -= lRightSideValue * (1 << SHIFT_FACTOR_TIntFloat);
	return *this;
}

TIntFloat& TIntFloat::operator*=(const TIntFloat& lRightSideValue)
{
	this->iIntegerValue = ((TInt64) this->iIntegerValue * lRightSideValue.iIntegerValue) >> SHIFT_FACTOR_TIntFloat;
	return *this;
}

TIntFloat& TIntFloat::operator*=(const TInt& lRightSideValue)
{
	this->iIntegerValue = this->iIntegerValue * lRightSideValue; //don't need to shiftup since we are already in the right base
	return *this;
}

TIntFloat& TIntFloat::operator*=(const TDouble& lRightSideValue)
{
	this->iIntegerValue = ((TInt64) this->iIntegerValue * Convert(lRightSideValue).iIntegerValue) >> SHIFT_FACTOR_TIntFloat;
	return *this;
}

TIntFloat& TIntFloat::operator/=(const TIntFloat& lRightSideValue)
{
	this->iIntegerValue = ((TInt64) this->iIntegerValue << SHIFT_FACTOR_TIntFloat) / lRightSideValue.iIntegerValue; //shift the upper argument by 16 bit, since dividing by a 16 bit number will shift the number left again
	return *this;
}

TIntFloat& TIntFloat::operator/=(const TInt& lRightSideValue)
{
	this->iIntegerValue = this->iIntegerValue / lRightSideValue; //don't need to shiftup since we are already in the right base
	return *this;
}

TIntFloat& TIntFloat::operator/=(const TDouble& lRightSideValue)
{
	this->iIntegerValue = ((TInt64) this->iIntegerValue << SHIFT_FACTOR_TIntFloat) / Convert(lRightSideValue).iIntegerValue; //shift the upper argument by 16 bit, since dividing by a 16 bit number will shift the number left again
	return *this;
}

TIntFloat TIntFloat::operator+(const TIntFloat& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy += lRightSideValue;
	return lCopy;
}

TIntFloat TIntFloat::operator+(const TInt& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy += lRightSideValue;
	return lCopy;
}

TIntFloat TIntFloat::operator+(const TDouble& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy += lRightSideValue;
	return lCopy;
}

TIntFloat TIntFloat::operator-(const TIntFloat& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy -= lRightSideValue;
	return lCopy;
}

TIntFloat TIntFloat::operator-(const TInt& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy -= lRightSideValue;
	return lCopy;
}

TIntFloat TIntFloat::operator-(const TDouble& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy -= lRightSideValue;
	return lCopy;
}

TIntFloat TIntFloat::operator-() const
{
	TIntFloat lCopy = *this;
	return lCopy *= -1;
}

TIntFloat TIntFloat::operator*(const TIntFloat& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy *= lRightSideValue;
	return lCopy;
}

TIntFloat TIntFloat::operator*(const TInt& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy *= lRightSideValue;
	return lCopy;
}

TIntFloat TIntFloat::operator*(const TDouble& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy *= lRightSideValue;
	return lCopy;
}

TIntFloat TIntFloat::operator/(const TIntFloat& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy /= lRightSideValue;
	return lCopy;
}

TIntFloat TIntFloat::operator/(const TInt& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy /= lRightSideValue;
	return lCopy;
}

TIntFloat TIntFloat::operator/(const TDouble& lRightSideValue) const
{
	TIntFloat lCopy = *this;
	lCopy /= lRightSideValue;
	return lCopy;
}

TBool TIntFloat::operator==(const TIntFloat& lRightSideValue) const
{
	return this->iIntegerValue == lRightSideValue.iIntegerValue;
}

TBool TIntFloat::operator==(const TInt& lRightSideValue) const
{
	return (this->iIntegerValue >> SHIFT_FACTOR_TIntFloat) == lRightSideValue; //shift down to lose precision
}

TBool TIntFloat::operator==(const TDouble& lRightSideValue) const
{
	return (this->iIntegerValue) == TIntFloat::Convert(lRightSideValue).iIntegerValue; //shift down to lose precision
}

TBool TIntFloat::operator!=(const TIntFloat& lRightSideValue) const
{
	return !(*this == lRightSideValue);
}

TBool TIntFloat::operator!=(const TInt& lRightSideValue) const
{
	return !(*this == lRightSideValue);
}

TBool TIntFloat::operator!=(const TDouble& lRightSideValue) const
{
	return (this->iIntegerValue) != TIntFloat::Convert(lRightSideValue).iIntegerValue; //shift down to lose precision
}

TBool TIntFloat::operator>(const TIntFloat& lRightSideValue) const
{
	return (this->iIntegerValue > lRightSideValue.iIntegerValue);
}

TBool TIntFloat::operator>(const TInt& lRightSideValue) const
{
	return (this->iIntegerValue > lRightSideValue << SHIFT_FACTOR_TIntFloat);
}

TBool TIntFloat::operator>(const TDouble& lRightSideValue) const
{
	return (this->iIntegerValue) > TIntFloat::Convert(lRightSideValue).iIntegerValue; //shift down to lose precision
}

TBool TIntFloat::operator>=(const TIntFloat& lRightSideValue) const
{
	return (this->iIntegerValue >= lRightSideValue.iIntegerValue);
}

TBool TIntFloat::operator>=(const TInt& lRightSideValue) const
{
	return (this->iIntegerValue >= lRightSideValue << SHIFT_FACTOR_TIntFloat);
}

TBool TIntFloat::operator>=(const TDouble& lRightSideValue) const
{
	return (this->iIntegerValue) >= TIntFloat::Convert(lRightSideValue).iIntegerValue; //shift down to lose precision
}

TBool TIntFloat::operator<(const TIntFloat& lRightSideValue) const
{
	return (this->iIntegerValue < lRightSideValue.iIntegerValue);
}

TBool TIntFloat::operator<(const TInt& lRightSideValue) const
{
	return (this->iIntegerValue < lRightSideValue << SHIFT_FACTOR_TIntFloat);
}

TBool TIntFloat::operator<(const TDouble& lRightSideValue) const
{
	return (this->iIntegerValue) < TIntFloat::Convert(lRightSideValue).iIntegerValue; //shift down to lose precision
}

TBool TIntFloat::operator<=(const TIntFloat& lRightSideValue) const
{
	return (this->iIntegerValue <= lRightSideValue.iIntegerValue);
}

TBool TIntFloat::operator<=(const TInt& lRightSideValue) const
{
	return (this->iIntegerValue <= lRightSideValue << SHIFT_FACTOR_TIntFloat);
}

TBool TIntFloat::operator<=(const TDouble& lRightSideValue) const
{
	return (this->iIntegerValue) <= TIntFloat::Convert(lRightSideValue).iIntegerValue; //shift down to lose precision
}

void TIntFloat::AssignIntAlreadyInBaseIntFloat(const TInt& aValue)
{
	iIntegerValue = aValue;
}

TIntFloat TIntFloat::Convert(const TInt& aValueInBaseInt)
{
	TIntFloat lValueAsIntFloat;
	lValueAsIntFloat.AssignIntAlreadyInBaseIntFloat(aValueInBaseInt << SHIFT_FACTOR_TIntFloat);
	return lValueAsIntFloat;
}

TIntFloat TIntFloat::Convert(const TDouble& aValueInBaseDouble)
{
	TIntFloat lValueAsIntFloat;
	TInt lShiftValue = 1 << SHIFT_FACTOR_TIntFloat;
	TInt lResult = (TInt) (aValueInBaseDouble * lShiftValue);
	lValueAsIntFloat.AssignIntAlreadyInBaseIntFloat(lResult);
	return lValueAsIntFloat;
}

TDouble TIntFloat::GetAsDouble()
{
	TDouble lReturnValue;
	TInt lShiftValue = 1 << SHIFT_FACTOR_TIntFloat;
	lReturnValue = (TDouble) iIntegerValue / lShiftValue;
	return lReturnValue;
}

TInt TIntFloat::GetIntInBaseIntFloat() const
{
	return iIntegerValue;
}

TInt TIntFloat::GetIntInBaseInt() const
{
	return iIntegerValue >> SHIFT_FACTOR_TIntFloat;
}

