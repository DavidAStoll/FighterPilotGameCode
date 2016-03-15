/*
 * CShip.h
 *
 *  Created on: Aug 28, 2012
 *      Author: dstoll
 */

#ifndef CSHIP_H_
#define CSHIP_H_

// INCLUDES
#include "includes/game/backend/basicClasses/CMoveableGameObject.h"
// CLASS DECLARATION

/**
 *  CShip
 *
 */
class CShip : public CMoveableGameObject
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CShip();
  void SetTargetPriority(TUint aPriority);
  void MoveToLocation(TIntFloat aXDestination);

protected:

	void SinkingSequence();
	virtual void Die(TDamageType aType); // sets the status of the Object to dead but also starts the dead animation loop
  virtual void Update();
  void ShipBasicUpdate();
  void BasicShipPostConstructCall();
  virtual void UpdateGunPriorities(); //stub
  virtual void SinkingFunctionCallWhenShipMovesDown();
  virtual void SinkingFunctionCallWhenShipRotates();
  virtual void PossibleCollision(CInterval*& aInterval);
  void ShipSaveContentToDisk(CFileWriteStream &aOutputStream);
  void ShipLoadContentFromDisk(CFileReadStream &aReadStream);

	/**
	 * Constructor for performing 1st stage construction
	 */
	CShip(TInt aZValue,TInt aXLocation, TInt aKeelDepth, TIntFloat aShipMaxSpeed, SGameObjectAttributes& aAttributes);

	TInt iSinkingSequenceMoveDownFrame; // to keep track when we let the ship move down a bit to simulate the sinking
	TInt iSinkingSequenceRotateFrame;// to keep track when we need to rotate the ship up a bit
	TIntFloat iXDestination; //location where ship is going to move to
	TIntFloat iShipMaxSpeed;
	TBool iHaveMoveOrder;
	TUint iTargetPriority;

	};

#endif /* CSHIP_H_ */
