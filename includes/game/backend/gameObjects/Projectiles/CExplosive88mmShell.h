/*
 ============================================================================
 Name		: Explosive88mmShell.h
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CExplosive88mmShell declaration
 ============================================================================
 */

#ifndef EXPLOSIVE88MMSHELL_H
#define EXPLOSIVE88MMSHELL_H


#include "includes/game/backend/basicClasses/CProjectile.h"

// CLASS DECLARATION

/**
 *  CExplosive88mmShell
 * 
 */
class CExplosive88mmShell : public CProjectile
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CExplosive88mmShell();

        /**
         * Two-phased constructor.
         */
        static CExplosive88mmShell* New(TPointIntFloat aLocation,TInt aAngel,TBool aReflected,TInt aRange);
        static void InitilizeExplosive88mmShell();      
        static void RemoveExplosive88mmShell();
        virtual void Destruct(); //called when range is less than 0,might cause an explosion or just remove the object
        
protected:

        /**
         * Constructor for performing 1st stage construction
         */
        CExplosive88mmShell(TPoint aLocation,SGameObjectAttributes& aAttributes, TInt aAngel,TInt aRange);
        CExplosive88mmShell(TPointIntFloat aLocation,SGameObjectAttributes& aAttributes, TInt aAngel,TInt aRange);
        static void LoadExplosive88mmShellTextures();
        static void UnloadExplosive88mmShellTextures();
        void ConstructL();
        

	};

#endif // EXPLOSIVE88MMSHELL_H
