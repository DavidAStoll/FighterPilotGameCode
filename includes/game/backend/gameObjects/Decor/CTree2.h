/*
 ============================================================================
 Name		: Tree2.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CTree2 declaration
 ============================================================================
 */

#ifndef TREE2_H
#define TREE2_H


// CLASS DECLARATION
#include "includes/game/backend/basicClasses/CGameObject.h"


enum TTree2Type //used to specify what kind of Palm tree you want to have 
{
  ETree2BowedTree,ETree2BowedTreeReflected, ETree2BowedTreeSmaller,ETree2BowedTreeSmallerReflected,
  ETree2MiddleTree,ETree2MiddleTreeReflected,ETree2MiddleTreeSmaller,ETree2MiddleTreeSmallerReflected
};

/**
 *  CTree2
 * 
 */
class CTree2 : public CGameObject
        {
public:
        // Constructors and destructor

				/**
				 * Destructor.
				 */
				~CTree2();

				/**
				 * Two-phased constructor.
				 */
				static CTree2* New(TPoint aLocation,TTree2Type aType);
				static CTree2* New(TInt aXLocation,TTree2Type aType);

				virtual void PossibleCollision(CInterval*& aInterval);
				static void InitilizeTree2();
				static void RemoveTree2();
				virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
				virtual void LoadFromDisk(CFileReadStream &aReadStream);

protected:

        /**
         * Constructor for performing 1st stage construction
         */
        CTree2(TPoint aLocation,SGameObjectAttributes& aAttributes);

        virtual void Die(TDamageType aType); // does nothing since land can't die

        /**
         * EPOC default constructor for performing 2nd stage construction
         */
        void ConstructL(TTree2Type aType);
        
        static void LoadTree2Textures();
        static void UnloadTree2Textures();

        TTree2Type iTreeType;

        };

#endif // TREE2_H
