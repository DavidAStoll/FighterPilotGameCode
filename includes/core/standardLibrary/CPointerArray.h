/*
 * CPointerArray.h
 *
 *  Created on: Jun 10, 2012
 *      Author: dstoll
 */

#ifndef CPOINTERARRAY_H_
#define CPOINTERARRAY_H_

#include <string.h>
#include <malloc.h>
#include "includes/core/standardLibrary/BasicTypes.h"


#define CPOINTER_ARRAY_INITIAL_SIZE 32
#define CPOINTER_ARRAY_INCREASE_MUTILPLIER 2 //always double the size of what it was previously

template <class GenericClass> class CPointerArray
{
public:

	//basic constructors
	static CPointerArray* New();
	static CPointerArray* New(const CPointerArray& aCopy);
	static CPointerArray* New(TInt aSizeOfArray);
	~CPointerArray(); //users should call the function provided instead before calling the delete operator

	//operators
	CPointerArray* operator= (const CPointerArray& aCopy);
	GenericClass* operator[] (const TUint& aIndex);

	//Adds an Pointer to an Element to the end of the array
	void Append(GenericClass* aElementPointer);
	//Adds all the data from a CPointerArray of the same type
	void AppendArray(const CPointerArray& aCopy);
	//returns the element stored at the given index
	GenericClass* Get(const TUint& aIndex) const;
	//returns a pointer to the underlying data array
	GenericClass** GetArrayData() const;
	//return how many elements are stored in the array
	TUint GetCount() const;
	//keeps the memory allocated to the array, but removes all references to elements
	void Clear();
	//keeps the memory allocated to the array, but removes all references to elements and calls their destructor
	void ClearAndDestroy();
	//looks through the array to find the location of the object, if it is not found it returns -1
	TInt Find(GenericClass* aObject);
	//Sorts the array using merge sort, creates one more temp array during sorting, in ascending order
	//Needs a pointer to a method that compares two class objects, this method should return negative if first object is less than object 2, 0 if equal, Positive if greater
	void SortMerge(TInt (*aSortMethod)(const GenericClass*, const GenericClass*));
	//removes the reference to an Element at the particular position
	void Remove(TUint aPosition);
	//removes the reference to an Element at the particular position and calls its destructor
	void RemoveAndDestroy(TUint aPosition);


private:

	GenericClass** iPointerArray;
	TUint iSizeOfArray;
	TUint iCount; //keeps track how many objects are stored in the array

	CPointerArray();
	CPointerArray(TInt aSizeOfArray);

	void Construct();
	void Construct(const CPointerArray& aCopy);
};

//------------------------------------- IMPLEMENTATION ----------------------------------------//

template <class GenericClass>
CPointerArray<GenericClass>::CPointerArray()
{
	iSizeOfArray = CPOINTER_ARRAY_INITIAL_SIZE;
	iCount = 0;
	iPointerArray = NULL;
}

template <class GenericClass>
CPointerArray<GenericClass>::CPointerArray(TInt aSizeOfArray)
{
	iSizeOfArray = aSizeOfArray;
	iCount = 0;
	iPointerArray = NULL;
}

template <class GenericClass>
CPointerArray<GenericClass>::~CPointerArray()
{
	if(iPointerArray != NULL)
	{
		delete[] iPointerArray;
		iPointerArray = NULL;
	}
}

template <class GenericClass>
CPointerArray<GenericClass>* CPointerArray<GenericClass>::New()
{
	CPointerArray* lSelf = new CPointerArray();
	lSelf->Construct();
	return lSelf;
}

template <class GenericClass>
CPointerArray<GenericClass>* CPointerArray<GenericClass>::New(TInt aSizeOfArray)
{
	CPointerArray* lSelf = new CPointerArray(aSizeOfArray);
	lSelf->Construct();
	return lSelf;
}

template <class GenericClass>
CPointerArray<GenericClass>* CPointerArray<GenericClass>::New(const CPointerArray& aCopy)
{
	CPointerArray* lSelf = new CPointerArray();
	lSelf->Construct(aCopy);
	return lSelf;
}

template <class GenericClass>
void CPointerArray<GenericClass>::Construct()
{
	iPointerArray = new GenericClass* [iSizeOfArray];
}

template <class GenericClass>
void CPointerArray<GenericClass>::Construct(const CPointerArray& aCopy)
{
	//allocate enough space
	iPointerArray = new GenericClass* [aCopy.iSizeOfArray];

	//copy over data
	memcpy(iPointerArray, aCopy.iPointerArray, sizeof(GenericClass*) * aCopy.iSizeOfArray);

	//adjust members
	iCount = aCopy.iCount;
	iSizeOfArray = aCopy.iSizeOfArray;
}

template <class GenericClass>
CPointerArray<GenericClass>* CPointerArray<GenericClass>::operator =(const CPointerArray& aCopy)
{
	//if the the array is assigned to itself
	if(this == &aCopy)
		return this;

	if(iPointerArray != NULL)
	{
		delete iPointerArray;
		iPointerArray = NULL;
	}

	//allocate enough space
	iPointerArray = new GenericClass* [aCopy.iSizeOfArray];

	//copy over data
	memcpy(iPointerArray, aCopy.iPointerArray, sizeof(GenericClass*) * aCopy.iSizeOfArray);

	//adjust members
	iCount = aCopy.iCount;
	iSizeOfArray = aCopy.iSizeOfArray;

	return this;
}

template <class GenericClass>
GenericClass* CPointerArray<GenericClass>::operator [](const TUint& aIndex)
{
	return iPointerArray[aIndex];
}

template <class GenericClass>
void CPointerArray<GenericClass>::Append(GenericClass* aElementPointer)
{
	iCount++;
	if(iCount > iSizeOfArray)
	{
		//need a bigger array
		iSizeOfArray *= CPOINTER_ARRAY_INCREASE_MUTILPLIER;
		iPointerArray = (GenericClass**) realloc(iPointerArray, sizeof(GenericClass*) * iSizeOfArray);
	}

	iPointerArray[iCount - 1] = aElementPointer;
}

template <class GenericClass>
void CPointerArray<GenericClass>::AppendArray(const CPointerArray& aCopy)
{
	TInt lOldCount = iCount;
	iCount += aCopy.iCount; //update count

	//check if we have need more space
	if(iCount > iSizeOfArray)
	{
		//need a bigger array
		iSizeOfArray = iCount * CPOINTER_ARRAY_INCREASE_MUTILPLIER;
		iPointerArray = (GenericClass**) realloc(iPointerArray, sizeof(GenericClass*) * iSizeOfArray);
	}

	//copy over data from second array but only the part that contains useful data, append the data
	memcpy(iPointerArray + lOldCount, aCopy.iPointerArray, sizeof(GenericClass*) * aCopy.iCount);
}

template <class GenericClass>
GenericClass* CPointerArray<GenericClass>::Get(const TUint& aIndex) const
{
	return iPointerArray[aIndex];
}

template <class GenericClass>
GenericClass**  CPointerArray<GenericClass>::GetArrayData() const
{
	return iPointerArray;
}

template <class GenericClass>
TUint CPointerArray<GenericClass>::GetCount() const
{
	return iCount;
}

template <class GenericClass>
void CPointerArray<GenericClass>::Clear()
{
	//we just need to reset the count, since we don't really need to reset the pointers
	iCount = 0;
}

template <class GenericClass>
void CPointerArray<GenericClass>::ClearAndDestroy()
{
	//delete all objects pointed to
	for(TInt lIndex = 0; lIndex < iCount; lIndex++)
	{
		delete iPointerArray[lIndex];
	}

	iCount = 0;
}

template <class GenericClass>
TInt CPointerArray<GenericClass>::Find(GenericClass* aObject)
{
	for(TInt lIndex = 0; lIndex < iCount; lIndex++)
	{
		 if(iPointerArray[lIndex] == aObject)
		 {
			 return lIndex;
		 }
	}

	return -1;
}

template <class GenericClass>
void CPointerArray<GenericClass>::SortMerge(TInt (*aSortMethod)(const GenericClass*, const GenericClass*))
{
	if(iCount > 1)
	{
		GenericClass** lStorageArray =  new GenericClass* [iSizeOfArray];

		for(TInt lContainerSize = 1; lContainerSize < iCount; lContainerSize = lContainerSize * 2) //size of each Container
		{
			for(TInt lContainerIndex = 0; lContainerIndex < iCount; lContainerIndex = lContainerIndex + (2 * lContainerSize)) //starting index of Each Container
			{
				TInt lLeftStartIndex = lContainerIndex;
				TInt lRightStartIndex = lContainerIndex + lContainerSize;
				TInt lMaxIndex = lContainerIndex + lContainerSize * 2;

				//need to do checks to make sure that we are not out of bounds
				if(lRightStartIndex > iCount)
					lRightStartIndex = iCount;
				if(lMaxIndex > iCount)
					lMaxIndex = iCount;

				//keep track MoveableIndex pointers
				TInt lLeftIndex = lLeftStartIndex;
				TInt lRightIndex = lRightStartIndex;

				for(TInt lRunIndex = lLeftStartIndex; lRunIndex < lMaxIndex; lRunIndex++)
				{
					if(lLeftIndex < lRightStartIndex //check if entire Left Container has been added, otherwise add entire Right container
						&& (lRightIndex >= lMaxIndex //means that the entire Right Container has been, added thus, just add the left container
						|| aSortMethod(iPointerArray[lLeftIndex], iPointerArray[lRightIndex]) < 0) //since smaller returns negative
						)
					{
						lStorageArray[lRunIndex] = iPointerArray[lLeftIndex];
						lLeftIndex++;
					}
					else
					{
						lStorageArray[lRunIndex] = iPointerArray[lRightIndex];
						lRightIndex++;
					}
				}
			}

			//sort has been done, need to switch array order around
			GenericClass** lTempArray = lStorageArray; //need to keep track of Pointer
			lStorageArray = iPointerArray; //StoreArray uses the old, unsorted version of the array to store next results
			iPointerArray = lTempArray; //points to the new sorted version
		}

		//all sorted, delete allocated memory
		if(lStorageArray)
		{
			delete[] lStorageArray; //always points to the old version, thus can be removed
			lStorageArray = NULL;
		}
	}
}

template <class GenericClass>
void CPointerArray<GenericClass>::Remove(TUint aPosition)
{
	TInt lNumberCellsRightOfPosition = (iCount - 1)  - aPosition;

	if(lNumberCellsRightOfPosition > 0) //in case last IndexPosition
	{
		//just shift(copy) all bytes to the right of the remove location by 1 cell left
		memcpy(iPointerArray + aPosition, iPointerArray + aPosition + 1, sizeof(GenericClass*) * lNumberCellsRightOfPosition);
	}

	//decrease by 1
	iCount--;
}

template <class GenericClass>
void CPointerArray<GenericClass>::RemoveAndDestroy(TUint aPosition)
{
	//delete the object
	delete iPointerArray[aPosition];

	//reset the array remove the gap in the array
	Remove(aPosition);
}

#endif /* CPOINTERARRAY_H_ */
