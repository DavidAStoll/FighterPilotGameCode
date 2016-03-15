/*
 ============================================================================
 Name		: JapaneseAAConcrete.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CJapaneseAAConcrete declaration
 ============================================================================
 */

#ifndef JAPANESEAACONCRETE_H
#define JAPANESEAACONCRETE_H

// INCLUDES
#include "includes/game/backend/basicClasses/CAAArterially.h"


class CGunAA88mm;

// CLASS DECLARATION

/**
 *  CJapaneseAAConcrete
 * 
 */
class CJapaneseAAConcrete : public CAAArterially
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CJapaneseAAConcrete();

	/**
	 * Two-phased constructor.
	 */
	static CJapaneseAAConcrete* New(TPoint aLocation);
	static CJapaneseAAConcrete* New(TPoint aLocation, TConflictSide aSide);
	static CJapaneseAAConcrete* New(TInt aXLocation);
	static CJapaneseAAConcrete* New(TInt aXLocation, TConflictSide aSide);
	
	static void InitilizeJapaneseAAConcrete();      
	static void RemoveJapaneseAAConcrete();
	
	virtual void Update();
	virtual void Draw();
	virtual void Shoot(TInt aRange);
	virtual void DrawAtPositionWithValues(TPointIntFloat& aPosition,TIntFloat lWidth,TIntFloat lHeight);
	virtual void PossibleCollision(CInterval*& aInterval);
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CJapaneseAAConcrete(TPoint aLocation,SGameObjectAttributes& aAttributes);
	//should be called if canon has been rotated sinc this method will reposition the canon back to its fix point
	virtual void RepositionGun();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
	
	CTextureObject* iConcreteBase;
	CGunAA88mm* iGunAA88mm;
	
	static void LoadJapaneseAAConcreteTextures();
	static void UnloadJapaneseAAConcreteTextures();
	
	void ConstructL();
	};

#endif // JAPANESEAACONCRETE_H
