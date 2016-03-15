/*
 ============================================================================
 Name		: Ship.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Copright © 2011 HQ Interactive Inc.
 Description : CShip implementation
 ============================================================================
 */

#include "includes/game/backend/basicClasses/CShip.h"

CShip::CShip(TInt aZValue,TInt aXLocation, TInt aKeelDepth, TIntFloat aShipMaxSpeed, SGameObjectAttributes& aAttributes):
CMoveableGameObject(aZValue, NULL, NULL, TPoint(aXLocation,-aKeelDepth), aAttributes, DEFAULT_FRAMES_PER_MOVE)
	{
	iSinkingSequenceMoveDownFrame = 0;
	iSinkingSequenceRotateFrame = 0;
	iShipMaxSpeed = aShipMaxSpeed;
	iHaveMoveOrder = false;
	iTargetPriority =  GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_TROOPS;
	}

CShip::~CShip()
	{
	}

//------------------- functions -------------------//

void CShip::ShipBasicUpdate()
{
	if(!iAlive)
		SinkingSequence();
	else
	{
		//check if we are moving anywhere
		if(iHaveMoveOrder)
		{
			if(iObjectReflected)
			{
				//move to the right
				if(iCoordinates.iX > iXDestination) //reached destination
				{
					iHaveMoveOrder = false;
					iSpeed = TIntFloat::Convert(0);
					iPixelsPerMoveX = (iSpeed * CMath::GraphicsCosTable(iAngle));
				}
			}
			//move to the left
			else if(iCoordinates.iX < iXDestination)
			{
				iHaveMoveOrder = false;
				iSpeed = TIntFloat::Convert(0);
				iPixelsPerMoveX = (iSpeed * CMath::GraphicsCosTable(iAngle));
			}
		}
	}
}

void CShip::BasicShipPostConstructCall()
{
	if(iObjectReflected)
		TurnRight();
	else
		TurnLeft();
}

void CShip::Update()
{
	ShipBasicUpdate();
}

void CShip::SetTargetPriority(TUint aPriority)
{
	iTargetPriority = aPriority;
	UpdateGunPriorities();
}

void CShip::UpdateGunPriorities()
{
	//stub - should contain the code, that let Guns attack targets what ever the current priority is
}

void CShip::MoveToLocation(TIntFloat aXDestination)
{
	iHaveMoveOrder = true;
	iSpeed = iShipMaxSpeed;
	iXDestination = aXDestination;
	//check what way we have to turn the ship
	if(iCoordinates.iX > iXDestination) // ship is to the right of the destination. Need to turn to the left
		TurnLeft();
	else
		TurnRight();
}

void CShip::SinkingFunctionCallWhenShipMovesDown()
{
}

void CShip::SinkingFunctionCallWhenShipRotates()
{
}

void CShip::PossibleCollision(CInterval*& aInterval)
{

	CPointerArray<CGameObject>* lGameObjects = aInterval->GetGameObjectsByType(EObjectTypesPlane);

	for(TInt lCurrentObjectIndex = 0; lCurrentObjectIndex < lGameObjects->GetCount(); lCurrentObjectIndex++)
	{
		CGameObject* lCurrentGameObject = lGameObjects->Get(lCurrentObjectIndex);

		//only collide with objects of type plane that are alive
		if(lCurrentGameObject->IsAlive() && iHitBox->IntersectionL(lCurrentGameObject->GetHitBox()))
		{
			SDamage lDamage;
			lDamage.Damage = KILL_ANY_AIRPLANE_DAMAGE;
			lDamage.ArmorPenetration = KILL_ANY_AIRPLANE_ARMOR_PENETRATION;//should kill any object
			lCurrentGameObject->TakeDamage(lDamage, EDamageTypeShipPlaneCollision);
		}
	}

	//clean up
	delete lGameObjects;
}

void CShip::SinkingSequence()
{
	iSinkingSequenceMoveDownFrame++;
	iSinkingSequenceRotateFrame++;
	if(iSinkingSequenceMoveDownFrame == 3)
	{
		iSinkingSequenceMoveDownFrame = 0;
		iCoordinates.iY += -TIntFloat::Convert(SHIP_BASIC_SINKING_SEQUENCE_INCREASE_Y);
		iTextureObject->ChangeYCoordinate(-TIntFloat::Convert(SHIP_BASIC_SINKING_SEQUENCE_INCREASE_Y));

		//might need to do some post setup
		SinkingFunctionCallWhenShipMovesDown();
	}
	if(iSinkingSequenceRotateFrame == 230 && !(iAngle == 90 || iAngle == 270))
	{
		iSinkingSequenceRotateFrame = 0;

		//rotate by 1 degrees
		iAngle += -1;
		iTextureObject->RotateVertexes(-1);
		iCoordinates = iTextureObject->ReturnCurrentFixPoint(); // new location of ship

		//might need to dop some post setup
		SinkingFunctionCallWhenShipRotates();
	}
}

void CShip::Die(TDamageType aType)
{
	if(!iAlive)
		return; //can only die once

	MoveableObjectBasicDieRoutine();
}


void CShip::ShipSaveContentToDisk(CFileWriteStream &aOutputStream)
{
	MoveableGameObjectSaveContentToDisk(aOutputStream);
	aOutputStream.WriteInt32(iSinkingSequenceMoveDownFrame);
	aOutputStream.WriteInt32(iSinkingSequenceRotateFrame);
	aOutputStream.WriteIntFloat(iXDestination);
	aOutputStream.WriteIntFloat(iShipMaxSpeed);
	aOutputStream.WriteInt32(iHaveMoveOrder);
	aOutputStream.WriteInt32(iTargetPriority);
}
void CShip::ShipLoadContentFromDisk(CFileReadStream &aReadStream)
{
	MoveableGameObjectLoadContentFromDisk(aReadStream);
	iSinkingSequenceMoveDownFrame = aReadStream.ReadInt32();
	iSinkingSequenceRotateFrame = aReadStream.ReadInt32();
	iXDestination = aReadStream.ReadIntFloat();
	iShipMaxSpeed = aReadStream.ReadIntFloat();
	iHaveMoveOrder = aReadStream.ReadInt32();
	iTargetPriority = aReadStream.ReadInt32();
}
