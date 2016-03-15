/*
 ============================================================================
 Name		: ClusterBomb.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CClusterBomb declaration
 ============================================================================
 */

#ifndef CLUSTERBOMB_H
#define CLUSTERBOMB_H



#include "includes/game/backend/basicClasses/CProjectileBomb.h"

/**
 *  CClusterBomb
 * 
 */
class CClusterBomb : public CProjectileBomb
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CClusterBomb();

	/**
	 * Two-phased constructor.
	 */
	static CClusterBomb* New(TPointIntFloat aLocation,TIntFloat aSpeed, TInt aAngle,TInt aTimerFrames,TBool aReflected);

	
        static void InitilizeClusterBomb();      
        static void RemoveClusterBomb();
        virtual void PossibleCollision(CInterval*& aInterval);
        virtual void Update();
        virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
        virtual void LoadFromDisk(CFileReadStream &aReadStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CClusterBomb(TPointIntFloat aLocation,SGameObjectAttributes& aAttributes,TIntFloat aSpeed,TInt aAngle,TInt aTimerFrames);

        static void LoadClusterBombTextures();
        static void UnloadClusterBombTextures();
        static void CreateClusterBombCollisionPointsL();
        static void RemoveClusterBombCollisionPoints();
                    
        TInt iCurrentFrameCounterUntilExplosion;
        TInt iFramesUntilExplodes;

        /**
         * EPOC default constructor for performing 2nd stage construction
         */
        void ConstructL();
        
        static SBoundingCollisionPoint* CollisionPoints;

	};

#endif // CLUSTERBOMB_H
