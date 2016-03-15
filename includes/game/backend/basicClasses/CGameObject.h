/*
 * CGameObject.h
 *
 *  Created on: Aug 23, 2012
 *      Author: dstoll
 */

#ifndef CGAMEOBJECT_H_
#define CGAMEOBJECT_H_

// INCLUDES
#include "includes/game/backend/gameObjects/CGameObjectDefaultValues.h"
#include "includes/game/backend/basicClasses/CDrawObject.h"
#include "includes/game/backend/CInterval.h"
#include "includes/core/collision/CHitBox.h"
#include "includes/core/sound/CSoundObject.h"

class CAircraft;
class CInterval;

enum TConflictSide
{
	EConflictSideJapanese, EConflictSideAmerican, EConflictSideNeutal
}; //used to avoid friendly fire to some degree

enum TDamageType
{
	EDamageTypeBullet,
	EDamageTypePlaneBullet,
	EDamageTypeFire,
	EDamageTypeExplosion,
	EDamageTypePlaneCrash,
	EDamageTypeTreePlaneCollision,
	EDamageTypeAAGunPlaneCollision,
	EDamageTypePlaneCollision,
	EDamageTypeBuildingPlaneCollision,
	EDamageTypeShipPlaneCollision,
	EDamageTypeTankPlaneCollision,
	EDamageTypeSoldierPlaneCollision,
};

enum TGameObjectIdentifier
{
			EGameObjectIdentifierUndefined = 0,

			/*Projectiles*/
			EGameObjectIdentifier20mmBulletCannon,
			EGameObjectIdentifier13mmBullet,
			EGameObjectIdentifier8mmBullet,
			EGameObjectIdentifierRifleBullet,
			EGameObjectIdentifierExplosive88mmShell,
			EGameObjectIdentifierExplosive75mmShell,
			EGameObjectIdentifier1000kgBomb,
			EGameObjectIdentifierStandardRocket,
			EGameObjectIdentifier500kgBomb,
			EGameObjectIdentifier100kgBomb,
			EGameObjectIdentifierClusterBomb,
			EGameObjectIdentifierPropelledGrenade,
			EGameObjectIdentifierTankShell80mm,
			EGameObjectIdentifierExplosive7InchShell,
			EGameObjectIdentifierExplosive14InchShell,
			EGameObjectIdentifierTorpedo,
			EGameObjectIdentifierUnderWaterTorpedo,

			/*AirPlanes*/
			EGameObjectIdentifierDauntless,
			EGameObjectIdentifierMitchell,
			EGameObjectIdentifierBetty,
			EGameObjectIdentifierVal,

			/*Floor*/
			EGameObjectIdentifierLand,
			EGameObjectIdentifierWater,

			/*Decor*/
			EGameObjectIdentifierSmoke,
			EGameObjectIdentifierCloud,
			EGameObjectIdentifierFlagPole,
			EGameObjectIdentifierForest1,
			EGameObjectIdentifierForest2,
			EGameObjectIdentifierForest3,
			EGameObjectIdentifierForest4,

			/*Destructible Decor*/
			EGameObjectIdentifierTree1,
			EGameObjectIdentifierTree2,
			EGameObjectIdentifierTree3,
			EGameObjectIdentifierBigTree,
			EGameObjectIdentifierSmallTree,

			/*Explosions*/
			EGameObjectIdentifierExplosion14InchShell,
			EGameObjectIdentifierExplosion1000kg,
			EGameObjectIdentifierExplosion500kg,
			EGameObjectIdentifierExplosion1000kgInAir,
			EGameObjectIdentifierExplosion100kgInAir,
			EGameObjectIdentifierExplosion500kgInAir,
			EGameObjectIdentifierExplosion100kg,
			EGameObjectIdentifierExplosion14InchShellInWater,
			EGameObjectIdentifierExplosion1000kgInWater,
			EGameObjectIdentifierExplosion500kgInWater,
			EGameObjectIdentifierExplosion100kgInWater,
			EGameObjectIdentifierWaterSplash,

				/*Buildings*/
			EGameObjectIdentifierJapaneseAAConcrete,
			EGameObjectIdentifierJapaneseAA75mmType88,
			EGameObjectIdentifierJapaneseAA20mmMachineCannon,
			EGameObjectIdentifierAirport,
			EGameObjectIdentifierHangar,
			EGameObjectIdentifierLandingSurface,
			EGameObjectIdentifierHq,
			EGameObjectIdentifierComStation,

			/*Ships*/
			EGameObjectIdentifierUSSVictory,
			EGameObjectIdentifierAmericanLCT,
			EGameObjectIdentifierAmericanCarrierLexington,
			EGameObjectIdentifierAmericanDestroyer,
			EGameObjectIdentifierAmericanBattleshipUSSTennessee,
			EGameObjectIdentifierAmericanTroopTransporter,
			EGameObjectIdentifierJapaneseLCT,
			EGameObjectIdentifierJapaneseCarrierShoho,
			EGameObjectIdentifierJapaneseCarrierShokaku,
			EGameObjectIdentifierJapaneseCarrierTaiho,
			EGameObjectIdentifierJapaneseDestroyer,
			EGameObjectIdentifierJapaneseBattleshipYamato,
			EGameObjectIdentifierJapaneseTroopTransporter,
			EGameObjectIdentifierJapaneseOilTanker,

			/*GrountUnits*/
			EGameObjectIdentifierGroundUnitOverlordAI,
			EGameObjectIdentifierJapaneseRifleman,
			EGameObjectIdentifierJapaneseRocketman,
			EGameObjectIdentifierJapanese97ChiHaTank,
			EGameObjectIdentifierJapaneseAA97ChiHaTank,
			EGameObjectIdentifierAmericanRifleman,
			EGameObjectIdentifierAmericanRocketman,
			EGameObjectIdentifierAmericanShermanTank,
			EGameObjectIdentifierAmericanAAShermanTank,


			EGameObjectIdentifierStopReading //used to find out that we just read in the last gameObject from file for a saveGame
};

typedef TUint TGameObjectType; //can contain more than one Type by | together TGameObjectsType

enum TGameObjectTypes //called X because of name conflinct with typedef
{ //used to group the same objects close to each other during updates and benefit from caching a bit
	EObjectTypesFloor = 1,//any kind of floor objects move on, water or land
	EObjectTypesPlane = 2,// any kind of plane
	EObjectTypesSmallPlane = 4,
	EObjectTypesBigPlane = 8,// to further specify that it is a big plane
	EObjectTypesBuilding = 16,//any kind of building
	EObjectTypesBuildingAA = 32, //an AA Building or Gun
	EObjectTypesGroundUnit = 64,// any kind of ground unit
	EObjectTypesArmouredGroundUnit = 128,// any kind of ground unit that is heavly armoured
	EObjectTypesShip = 256,//any kind of ship
	EObjectTypesDecor = 512,//any kind of item that just stands around not involved in combat
	EObjectTypesDestructibleDecor = 1024,//any kind of item that just stands around but can be destroyed are altered throughout the battle, like trees
	EObjectTypesProjectile = 2048,
	EObjectTypesProjectileExplosive = 4096,
	EObjectTypesLandingSurface = 8192,
	EObjectTypesNoHitBoxNoTexture = 16384, //group for objects that have no hitbox
	EObjectTypesPlaneSpawnLocation = 32768,
	EObjectTypesNotImportant = 65536 //for objects that will never be called or used by other objects
};

//define groups of several similar Object types
#define GAMEOBJECT_TYPE_GROUP_PLANES (EObjectTypesBigPlane | EObjectTypesPlane)
#define GAMEOBJECT_TYPE_GROUP_BUILDINGS (EObjectTypesBuilding)
#define GAMEOBJECT_TYPE_GROUP_TROOPS (EObjectTypesGroundUnit | EObjectTypesArmouredGroundUnit)
#define GAMEOBJECT_TYPE_GROUP_SHIP (EObjectTypesShip)
#define GAMEOBJECT_TYPE_GROUP_PROJECTILE (EObjectTypesProjectile | EObjectTypesProjectileExplosive)
#define GAMEOBJECT_TYPE_GROUP_DECOR (EObjectTypesDecor | EObjectTypesDestructibleDecor)
#define GAMEOBJECT_TYPE_GROUP_OTHERS (EObjectTypesFloor | EObjectTypesLandingSurface | EObjectTypesNoHitBoxNoTexture | EObjectTypesPlaneSpawnLocation | EObjectTypesNotImportant)
#define GAMEOBJECT_TYPE_GROUP_EVERYTHING (GAMEOBJECT_TYPE_GROUP_PLANES | GAMEOBJECT_TYPE_GROUP_BUILDINGS | GAMEOBJECT_TYPE_GROUP_TROOPS | GAMEOBJECT_TYPE_GROUP_SHIP | GAMEOBJECT_TYPE_GROUP_PROJECTILE | GAMEOBJECT_TYPE_GROUP_DECOR | GAMEOBJECT_TYPE_GROUP_OTHERS)

enum TAircraftName
{
	EAircraftNameDauntless,
	EAircraftNameVal
};

enum TAircraftType
{
	EAircraftTypeFighter, EAircraftTypeDiveBomber, EAircraftTypeHeavyBomber
};

struct SDamage //each Damage type has a certain base damage plus a certain armor penetration value that defines how much damage is dealt against a armored target
{
	TInt Damage;
	TInt ArmorPenetration;
};

struct SRandomFiringOffset //used to keep track how much Angle Offset and range offSet a certain shot or bomb has when it is created
{
	TInt AngleOffset;
	TInt RangeOffset;
};

struct SGameObjectAttributes
{
	TInt Health;
	TInt Armor;
	CHitBox* HitBox;
	TBool Reflected;
	TGameObjectType ObjectType;
	TConflictSide ConflictSide;
	TGameObjectIdentifier ObjectIdentifier;
};

class CGameObject : public CDrawObject
{

public:
	virtual ~CGameObject();

	static CGameObject* New(
	/*values neeeded for Inheritance*/TInt aZCor, CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer,
	/*new values*/TPoint aLocation, SGameObjectAttributes& aAttributes);

	virtual void TakeDamage(const SDamage& aDamage, const TDamageType aType); // method call from outside to inflict damage on the object, might have special death animation for specific damage
	virtual void Update(); //runs the Game Object's logic code to update its member data
	virtual TPointIntFloat& GetCurrentPosition();//returns iCoordinates but reset from its internal format
	virtual TPoint GetCurrentPositionNormilized();// shifts the result to the right before returning it
	TBool RecycleObject();
	//both functions just draw the Collision Rectangles,these functions should be used for debug purposes only
	void DrawSimpleHitBoxL();
	virtual void DrawAdvancedHitBoxL();
	//returns if the Object is reflected or not
	const TBool& IsReflected();
	const TBool& IsAlive();
	void AssignConflictSide(TConflictSide aSide);//may be used to changes an object conflict side from its default behaviour
	const TConflictSide& GetConflictSide();
	TGameObjectType GetGameObjectType();
	CHitBox* GetHitBox();
	//returns the DistanceBetween Two GameObjects
	TIntFloat DistanceBetweenGameObjects(CGameObject* aGameObject);
	//this method is called from outside with Objects that it might collide with
	virtual void PossibleCollision(CInterval*& aInterval);
	//lets the object Face Right
	virtual void TurnRight();
	//left the object FaceLeft
	virtual void TurnLeft();
	//sets if The Object should be recycled or not
	virtual void SetRecycleObject(TBool aValue);
	//returns the gameObject Identifier
	TGameObjectIdentifier GetGameObjectIdentifier();
	TInt GetMaxHealth();
	TBool& GetAlreadyAddedToInterval();
	TBool& GetIsMoveableGameObject();
	//returns width of the object
	TIntFloat GetWidth();
	//height of object
	TIntFloat GetHeight();
	//Returns how much health the gameObject has left
	TInt GetHealth();
	// usually called when the object dies, move the object into the background, thus it does not obscure the view of any alive objects
	void GameObjectMoveIntoBackground();
	//this function is used by TLinerOrder to determine if an object is drawn behind or before an object in the Z plane
	//should be in DrawObject class, but due to some dependecies in GameEngine I put it here
	static TInt DetermineObjectDrawOrder(const CGameObject* aTextureObject1, const CGameObject* aTextureObject2);
	//repositions the object to the new location
	virtual void SetPosition(TPoint& aPoint);
	virtual void SetPosition(TPointIntFloat& aPoint);
	//does not reset the lower left coordinate of the texture, those does not reset any rotation that the object had
	virtual void SetPositionWithoutTextureObjectUpdate(TPointIntFloat& aPoint);
	//needed to have a spawingpoint, need to put it here otherwise the pointers will be messed up
	virtual CAircraft* CreateNewAircraft(); //creates a new aircraft and positions it on the deck of the carrier
	virtual void SaveOnDisk(CFileWriteStream &aOutputStream);
	virtual void LoadFromDisk(CFileReadStream &aReadStream);
	//saves the member data of the gameObject
	void GameObjectSaveContentToDisk(CFileWriteStream &aOutputStream);
	void GameObjectLoadContentFromDisk(CFileReadStream &aReadStream);

protected:

	// sets the status of the Object to dead, updates the score, might be overriden by child if specific behaviour is needed
	virtual void Die(TDamageType aType);
	// the general die routine that most gameObjects need to do when they die
	void GameObjectBasicDieRoutine();
	TInt iHealth;//keeps track of how much Health the object has
	TInt iMaxHealth;//keeps track of the max health of an object
	TInt iArmor;
	TPointIntFloat iCoordinates;
	CSoundObject* iSoundObject;
	CHitBox* iHitBox;//the HitBox used for collision detection
	TBool iObjectReflected; // by convention, the object faces left, that is I Corrdinate is the lower LeftCorner
	TBool iRecycleObject;// if true, this object will be removed from the game
	TBool iAlive;//if the Objects is dead or alive
	TBool iIsMoveableObject; //false for gameObjects
	TConflictSide iConflictSide; // what side is the object on,will be set in child constructor
	TGameObjectType iGameObjectType; //will be set in child constructor
	TGameObjectIdentifier iGameObjectIdentifier;

	//should not be loaded or saved. Since it always needs to stay false when the game starts up
	TBool iAlreadyPutIntoInterval;//need this

	CGameObject(TInt aZCor, CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer,//inheritance
			TPoint aLocation, SGameObjectAttributes& aAttributes); //new values
	CGameObject(TInt aZCor, CTextureObject* aTextureObject, CAnimationPlayer* aAnimationPlayer, TPointIntFloat aLocation, SGameObjectAttributes& aAttributes);// aLocation is already adjusted for the internal format

	void ConstructL();
};

#endif /* CGAMEOBJECT_H_ */
