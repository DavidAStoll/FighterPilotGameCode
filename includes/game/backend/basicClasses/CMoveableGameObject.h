/*
 * CMoveableGameObject.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dstoll
 */

#ifndef CMOVEABLEGAMEOBJECT_H_
#define CMOVEABLEGAMEOBJECT_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CGameObject.h"

class CMoveableGameObject : public CGameObject
{
// Constructors and destructor
public:
        /**
         * Destructor.
         */
        virtual ~CMoveableGameObject();
        virtual void Move();
        void SetPixelsPerMoveX(TIntFloat aValue);
        void SetPixelsPerMoveY(TIntFloat aValue);
        void SetSpeed(TIntFloat aValue);
        virtual TIntFloat* GetPixelsPerMoveX();
        virtual TIntFloat* GetPixelsPerMoveY();
        virtual void TurnLeft();
        virtual void TurnRight();
        void MoveableGameObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
        void MoveableGameObjectLoadContentFromDisk(CFileReadStream &aReadStream);
        void SetIsLeavingMap(TBool aValue);
        TInt& GetFramesToLive();
        TBool GetIsLeavingMap();
        TBool GetIsMoving();
        TBool& GetIsSlowlyRecycledRef();
        const TInt& GetAngle();

protected:

      	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
      	void MoveableObjectBasicDieRoutine();
        //must be set by child class
        TIntFloat iPixelsPerMoveX;// tells us how many pixels the object will move, might be negitive if we move left
        TIntFloat iPixelsPerMoveY;
        TInt iFramesToLive;
        TInt iAngle;
        TIntFloat iSpeed;

        //values set by constructer
        TBool iIsLeavingMap;
        TBool iIsSlowlyRecycled;
        TInt iCurrentFrame; // keeps track of what frame we are currently looking at
        /**
         * Constructor for performing 1st stage construction
         */
        CMoveableGameObject(
/*values neeeded for DrawObject*/       TInt aZCor,CTextureObject* aTextureObject,CAnimationPlayer* aAnimationPlayer,
/*values needed for GameObject*/        TPoint aLocation,SGameObjectAttributes& aAttributes,
/*new values*/                          TInt aFramesPerMove);
        //to avoid loss of precision for the location
        CMoveableGameObject(
/*values neeeded for DrawObject*/       TInt aZCor,CTextureObject* aTextureObject,CAnimationPlayer* aAnimationPlayer,
/*values needed for GameObject*/        TPointIntFloat aLocation,SGameObjectAttributes& aAttributes,
/*new values*/                          TInt aFramesPerMove);

};

#endif /* CMOVEABLEGAMEOBJECT_H_ */
