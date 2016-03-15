/*
 ============================================================================
 Name		: StandardRocket.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CStandardRocket declaration
 ============================================================================
 */

#ifndef STANDARDROCKET_H
#define STANDARDROCKET_H


#include "includes/game/backend/basicClasses/CProjectile.h"

class CGeneralSmokeCreator;



// CLASS DECLARATION

/**
 *  CStandardRocket
 * 
 */
class CStandardRocket : public CProjectile
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CStandardRocket();

	/**
	 * Two-phased constructor.
	 */
	static CStandardRocket* New(TPointIntFloat aLocation,TIntFloat aSpeed,TInt aAngel,TBool aReflected,CGameObject* aCreator);

        static void InitilizeStandardRocket();      
        static void RemoveStandardRocket();
        virtual void Destruct(); //called when range is less than 0,might cause an explosion or just remove the object
        void Destruct(TPointIntFloat* lCollisionPoint); //called when range is less than 0,might cause an explosion or just remove the object
        virtual void DestructWithoutExplosion();
        virtual void Update();
        virtual void PossibleCollision(CInterval*& aInterval);
        virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
        virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CStandardRocket(TPoint aLocation,SGameObjectAttributes& aAttributes,TIntFloat aSpeed, TInt aAngel,CGameObject* aCreator);
	CStandardRocket(TPointIntFloat aLocation,SGameObjectAttributes& aAttributes,TIntFloat aSpeed, TInt aAngel,CGameObject* aCreator);

        static void LoadStandardRocketTextures();
        static void UnloadStandardRocketTextures();
        static void CreateStandardRocketCollisionPointsL();
        static void RemoveStandardRocketCollisionPoints();
        
        CGameObject* iCreatorObject;
        CGeneralSmokeCreator* iSmokeCreator;

        /**
         * EPOC default constructor for performing 2nd stage construction
         */
        void ConstructL();
        static SBoundingCollisionPoint* CollisionPoints;
	};

#endif // STANDARDROCKET_H
