/*
 ============================================================================
 Name		: StatsTracker.cpp
 Author	  : David Stoll
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CStatsTracker implementation
 ============================================================================
 */
#include "includes/game/backend/CStatsTracker.h"
#include "includes/game/backend/gameObjects/GameObjectsIncludes.h"

CStatsTracker::CStatsTracker()
{
	//planes
	iNumberOfDiveBombersKilled = 0;
	iNumberOfDiveBombersLost = 0;
	iNumberOfFightersKilled = 0;
	iNumberOfFightersLost = 0;
	iNumberOfHeavyBombersKilled = 0;
	iNumberOfHeavyBombersLost = 0;

	//ships
	iNumberOfUSSVictoryLost = 0;
	iNumberOfAircraftCarriersKilled = 0;
	iNumberOfAircraftCarriersLost = 0;
	iNumberOfDestroyerKilled = 0;
	iNumberOfDestroyerLost = 0;
	iNumberOfLCTEmptyKilled =0;
	iNumberOfLCTEmptyLost =0;
	iNumberOfLCTLoadedKilled =0;
	iNumberOfLCTLoadedLost =0;
	iNumberOfTroopTransporterKilled = 0;
	iNumberOfTroopTransporterLost = 0;
	iNumberOfOilTankerKilled = 0;
	iNumberOfOilTankerLost = 0;
	iNumberOfBattleshipUSSTennesseeLost = 0;
	iNumberOfBattleshipYamatoKilled = 0;

	//Ground Units
	iNumberOfMediumTanksKilled = 0;
	iNumberOfMediumTanksLost = 0;
	iNumberOfAATanksKilled = 0;
	iNumberOfAATanksLost = 0;
	iNumberOfRiflemanKilled = 0;
	iNumberOfRiflemanLost = 0;
	iNumberOfAntiTankmanKilled = 0;
	iNumberOfAntiTankmanLost = 0;


	//buildings
	iNumberOfAirportsKilled = 0;
	iNumberOfAirportsLost = 0;
	iNumberOfComStationKilled = 0;
	iNumberOfComStationLost = 0;
	iNumberOfHangarKilled = 0;
	iNumberOfHangarLost = 0;
	iNumberOfHqKilled = 0;
	iNumberOfHqLost = 0;
	iNumberOfAA20mmMachineCannonKilled = 0;
	iNumberOfAA20mmMachineCannonLost = 0;
	iNumberOfAA75mmType88Killed = 0;
	iNumberOfAA75mmType88Lost = 0;
	iNumberOfAAConcreteKilled = 0;
	iNumberOfAAConcreteLost = 0;

	//time
	iGameTimeHours = 0;
	iGameTimeMins = 0;
	iGameTimeSecs = 0;
}

CStatsTracker::~CStatsTracker()
{
	if(iBonusObjectives != NULL)
	{
		for(TInt lIndex = 0; lIndex < iBonusObjectives->GetCount(); lIndex++)
		{
			delete iBonusObjectives->Get(lIndex)->NameOfObjective;
		}

		iBonusObjectives->ClearAndDestroy();
		delete iBonusObjectives;
		iBonusObjectives = NULL;
	}
}

CStatsTracker* CStatsTracker::New()
{
	CStatsTracker* self = new CStatsTracker();
	self->ConstructL();
	return self;
}

void CStatsTracker::ConstructL()
{
	iBonusObjectives = CPointerArray<SBonusObjective>::New();
}

void CStatsTracker::UpdateScoreObjectKilled(TGameObjectIdentifier aId, CGameObject* aDestroyedGameObject)
{
	switch(aId)
	{
		//----------------   PLANES   --------------
		case EGameObjectIdentifierBetty:
		{
			iNumberOfHeavyBombersKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(BETTY_KILL_BASE_SCORE);
			break;
		}
		case EGameObjectIdentifierVal:
		{
			iNumberOfDiveBombersKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(VAL_KILL_BASE_SCORE);
			break;
		}

		//--------------    SHIPS   --------------------------
		case EGameObjectIdentifierJapaneseLCT:
		{
			CJapaneseLCT* lJapaneseLCT = static_cast<CJapaneseLCT*> (aDestroyedGameObject);
			if(lJapaneseLCT->GetIsLoaded())
			{
				iNumberOfLCTLoadedKilled++;
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(JAPANESE_LCT_KILL_BASE_SCORE_LOADED);
			}else
			{
				iNumberOfLCTEmptyKilled++;
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(JAPANESE_LCT_KILL_BASE_SCORE_EMPTY);
			}
			break;
		}
		case EGameObjectIdentifierJapaneseDestroyer:
		{
			iNumberOfDestroyerKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(DESTROYER_KILL_BASE_SCORE);
			break;
		}
		case EGameObjectIdentifierJapaneseBattleshipYamato:
		{
			iNumberOfBattleshipYamatoKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(JAPANESE_BATTLESHIP_YAMATO_KILL_BASE_SCORE);
			break;
		}
		case EGameObjectIdentifierJapaneseCarrierTaiho:
		case EGameObjectIdentifierJapaneseCarrierShokaku:
		case EGameObjectIdentifierJapaneseCarrierShoho:
		{
			iNumberOfAircraftCarriersKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(CARRIER_KILL_BASE_SCORE);
			break;
		}
		case EGameObjectIdentifierJapaneseTroopTransporter:
		{
			iNumberOfTroopTransporterKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(TROOP_TRANSPORTER_KILL_BASE_SCORE);
			break;
		}
		case EGameObjectIdentifierJapaneseOilTanker:
		{
			iNumberOfOilTankerKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(OIL_TANKER_KILL_BASE_SCORE);
			break;
		}

			//--------------    BUILDINGS   --------------------------
		case EGameObjectIdentifierAirport:
		{
			iNumberOfAirportsKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(AIRPORT_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierComStation:
		{
			iNumberOfComStationKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(COMSTATION_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierHangar:
		{
			iNumberOfHangarKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(HANGAR_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierHq:
		{
			iNumberOfHqKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(HQ_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierJapaneseAA20mmMachineCannon:
		{
			iNumberOfAA20mmMachineCannonKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(JAPANESE_AA_20MM_MACHINE_CANNON_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierJapaneseAA75mmType88:
		{
			iNumberOfAA75mmType88Killed++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(JAPANESE_AA_75MM_TYPE88_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierJapaneseAAConcrete:
		{
			iNumberOfAAConcreteKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(JAPANESE_AA_CONCRETE_KILL_BASE_SCORE);
		}
			break;

			//--------------    GROUND UNITS   --------------------------
		case EGameObjectIdentifierJapanese97ChiHaTank:
		{
			iNumberOfMediumTanksKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(JAPANESE_97_CHI_HA_TANK_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierJapaneseAA97ChiHaTank:
		{
			iNumberOfAATanksKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(JAPANESE_AA_97_CHI_HA_TANK_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierJapaneseRifleman:
		{
			iNumberOfRiflemanKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(JAPANESE_RIFLEMAN_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierJapaneseRocketman:
		{
			iNumberOfAntiTankmanKilled++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(JAPANESE_ROCKETMAN_KILL_BASE_SCORE);
		}
			break;

		default:
			//not handled
			break;
	}
}

void CStatsTracker::UpdateScoreObjectLost(TGameObjectIdentifier aId, CGameObject* aDestroyedGameObject)
{
	switch(aId)
	{
		//----------------   PLANES   --------------
		case EGameObjectIdentifierMitchell:
		{
			iNumberOfHeavyBombersLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-MITCHELL_KILL_BASE_SCORE);
			break;
		}
		case EGameObjectIdentifierDauntless:
		{
			iNumberOfDiveBombersLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-DAUNTLESS_KILL_BASE_SCORE);
			break;
		}

			//--------------    SHIPS   --------------------------
		case EGameObjectIdentifierUSSVictory:
		{
			iNumberOfUSSVictoryLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-USS_VICTORY_KILL_BASE_SCORE);
			break;
		}
		case EGameObjectIdentifierAmericanLCT:
		{
			CAmericanLCT* lAmericanLCT = static_cast<CAmericanLCT*> (aDestroyedGameObject);
			if(lAmericanLCT->GetIsLoaded())
			{
				iNumberOfLCTLoadedLost++;
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-AMERICAN_LCT_KILL_BASE_SCORE_LOADED);
			}else
			{
				iNumberOfLCTEmptyLost++;
				CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-AMERICAN_LCT_KILL_BASE_SCORE_EMPTY);
			}
			break;
		}
		case EGameObjectIdentifierAmericanDestroyer:
		{
			iNumberOfDestroyerLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-DESTROYER_KILL_BASE_SCORE);
			break;
		}
		case EGameObjectIdentifierAmericanBattleshipUSSTennessee:
		{
			iNumberOfBattleshipUSSTennesseeLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-AMERICAN_BATTLESHIP_USS_TENNESSEE_KILL_BASE_SCORE);
			break;
		}
		case EGameObjectIdentifierAmericanCarrierLexington:
		{
			iNumberOfAircraftCarriersLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-CARRIER_KILL_BASE_SCORE);
			break;
		}
		case EGameObjectIdentifierAmericanTroopTransporter:
		{
			iNumberOfTroopTransporterLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-TROOP_TRANSPORTER_KILL_BASE_SCORE);
			break;
		}

			//--------------    BUILDINGS   --------------------------
		case EGameObjectIdentifierAirport:
		{
			iNumberOfAirportsLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-AIRPORT_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierComStation:
		{
			iNumberOfComStationLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-COMSTATION_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierHangar:
		{
			iNumberOfHangarLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-HANGAR_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierHq:
		{
			iNumberOfHqLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-HQ_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierJapaneseAA20mmMachineCannon:
		{
			iNumberOfAA20mmMachineCannonLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-JAPANESE_AA_20MM_MACHINE_CANNON_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierJapaneseAA75mmType88:
		{
			iNumberOfAA75mmType88Lost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-JAPANESE_AA_75MM_TYPE88_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierJapaneseAAConcrete:
		{
			iNumberOfAAConcreteLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-JAPANESE_AA_CONCRETE_KILL_BASE_SCORE);
		}
			break;

			//--------------    GROUND UNITS   --------------------------
		case EGameObjectIdentifierAmericanShermanTank:
		{
			iNumberOfMediumTanksLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-AMERICAN_SHERMAN_TANK_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierAmericanAAShermanTank:
		{
			iNumberOfAATanksLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-AMERICAN_AA_SHERMAN_TANK_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierAmericanRifleman:
		{
			iNumberOfRiflemanLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-AMERICAN_RIFLEMAN_KILL_BASE_SCORE);
		}
			break;
		case EGameObjectIdentifierAmericanRocketman:
		{
			iNumberOfAntiTankmanLost++;
			CFighterPilotThePacificWar::FighterGame->iGameData->GetMap()->GetPlayer()->ChangePoints(-AMERICAN_ROCKETMAN_KILL_BASE_SCORE);
		}
			break;

		default:
			//not handled
			break;
	}
}

void CStatsTracker::SaveOnDisk(CFileWriteStream &aOutputStream)
{
	//airplanes
	aOutputStream.WriteInt32(iNumberOfDiveBombersKilled);
	aOutputStream.WriteInt32(iNumberOfDiveBombersLost);
	aOutputStream.WriteInt32(iNumberOfFightersKilled);
	aOutputStream.WriteInt32(iNumberOfFightersLost);
	aOutputStream.WriteInt32(iNumberOfHeavyBombersKilled);
	aOutputStream.WriteInt32(iNumberOfHeavyBombersLost);

	//ships
	aOutputStream.WriteInt32(iNumberOfUSSVictoryLost);
	aOutputStream.WriteInt32(iNumberOfAircraftCarriersKilled);
	aOutputStream.WriteInt32(iNumberOfAircraftCarriersLost);
	aOutputStream.WriteInt32(iNumberOfDestroyerKilled);
	aOutputStream.WriteInt32(iNumberOfDestroyerLost);
	aOutputStream.WriteInt32(iNumberOfBattleshipUSSTennesseeLost);
	aOutputStream.WriteInt32(iNumberOfBattleshipYamatoKilled);
	aOutputStream.WriteInt32(iNumberOfLCTEmptyKilled);
	aOutputStream.WriteInt32(iNumberOfLCTEmptyLost);
	aOutputStream.WriteInt32(iNumberOfLCTLoadedKilled);
	aOutputStream.WriteInt32(iNumberOfLCTLoadedLost);
	aOutputStream.WriteInt32(iNumberOfTroopTransporterKilled);
	aOutputStream.WriteInt32(iNumberOfTroopTransporterLost);
	aOutputStream.WriteInt32(iNumberOfOilTankerKilled);
	aOutputStream.WriteInt32(iNumberOfOilTankerLost);

	//buildings
	aOutputStream.WriteInt32(iNumberOfAirportsKilled);
	aOutputStream.WriteInt32(iNumberOfAirportsLost);
	aOutputStream.WriteInt32(iNumberOfComStationKilled);
	aOutputStream.WriteInt32(iNumberOfComStationLost);
	aOutputStream.WriteInt32(iNumberOfHangarKilled);
	aOutputStream.WriteInt32(iNumberOfHangarLost);
	aOutputStream.WriteInt32(iNumberOfHqKilled);
	aOutputStream.WriteInt32(iNumberOfHqLost);
	aOutputStream.WriteInt32(iNumberOfAA20mmMachineCannonKilled);
	aOutputStream.WriteInt32(iNumberOfAA20mmMachineCannonLost);
	aOutputStream.WriteInt32(iNumberOfAA75mmType88Killed);
	aOutputStream.WriteInt32(iNumberOfAA75mmType88Lost);
	aOutputStream.WriteInt32(iNumberOfAAConcreteKilled);
	aOutputStream.WriteInt32(iNumberOfAAConcreteLost);

	//Ground Units
	aOutputStream.WriteInt32(iNumberOfMediumTanksKilled);
	aOutputStream.WriteInt32(iNumberOfMediumTanksLost);
	aOutputStream.WriteInt32(iNumberOfAATanksKilled);
	aOutputStream.WriteInt32(iNumberOfAATanksLost);
	aOutputStream.WriteInt32(iNumberOfRiflemanKilled);
	aOutputStream.WriteInt32(iNumberOfRiflemanLost);
	aOutputStream.WriteInt32(iNumberOfAntiTankmanKilled);
	aOutputStream.WriteInt32(iNumberOfAntiTankmanLost);

	for(TInt lCurrentIndex = 0; lCurrentIndex < iBonusObjectives->GetCount(); lCurrentIndex++)
	{
		aOutputStream.WriteInt32(1);
		aOutputStream.WriteInt32(iBonusObjectives->Get(lCurrentIndex)->ExtraScore);
		aOutputStream.WriteString(iBonusObjectives->Get(lCurrentIndex)->NameOfObjective->GetData());
	}
	aOutputStream.WriteInt32(0);//signals that we are done
}

void CStatsTracker::LoadFromDisk(CFileReadStream &aReadStream)
{
	for(TInt lIndex = 0; lIndex < iBonusObjectives->GetCount(); lIndex++)
	{
		delete iBonusObjectives->Get(lIndex)->NameOfObjective;
	}
	iBonusObjectives->ClearAndDestroy();

	//airplanes
	iNumberOfDiveBombersKilled = aReadStream.ReadInt32();
	iNumberOfDiveBombersLost = aReadStream.ReadInt32();
	iNumberOfFightersKilled = aReadStream.ReadInt32();
	iNumberOfFightersLost = aReadStream.ReadInt32();
	iNumberOfHeavyBombersKilled = aReadStream.ReadInt32();
	iNumberOfHeavyBombersLost = aReadStream.ReadInt32();

	//ships
	iNumberOfUSSVictoryLost = aReadStream.ReadInt32();
	iNumberOfAircraftCarriersKilled = aReadStream.ReadInt32();
	iNumberOfAircraftCarriersLost = aReadStream.ReadInt32();
	iNumberOfDestroyerKilled = aReadStream.ReadInt32();
	iNumberOfDestroyerLost = aReadStream.ReadInt32();
	iNumberOfBattleshipUSSTennesseeLost = aReadStream.ReadInt32();
	iNumberOfBattleshipYamatoKilled = aReadStream.ReadInt32();
	iNumberOfLCTEmptyKilled = aReadStream.ReadInt32();
	iNumberOfLCTEmptyLost = aReadStream.ReadInt32();
	iNumberOfLCTLoadedKilled = aReadStream.ReadInt32();
	iNumberOfLCTLoadedLost = aReadStream.ReadInt32();
	iNumberOfTroopTransporterKilled = aReadStream.ReadInt32();
	iNumberOfTroopTransporterLost = aReadStream.ReadInt32();
	iNumberOfOilTankerKilled = aReadStream.ReadInt32();
	iNumberOfOilTankerLost = aReadStream.ReadInt32();

	//buildings
	iNumberOfAirportsKilled = aReadStream.ReadInt32();
	iNumberOfAirportsLost = aReadStream.ReadInt32();
	iNumberOfComStationKilled = aReadStream.ReadInt32();
	iNumberOfComStationLost = aReadStream.ReadInt32();
	iNumberOfHangarKilled = aReadStream.ReadInt32();
	iNumberOfHangarLost = aReadStream.ReadInt32();
	iNumberOfHqKilled = aReadStream.ReadInt32();
	iNumberOfHqLost = aReadStream.ReadInt32();
	iNumberOfAA20mmMachineCannonKilled = aReadStream.ReadInt32();
	iNumberOfAA20mmMachineCannonLost = aReadStream.ReadInt32();
	iNumberOfAA75mmType88Killed = aReadStream.ReadInt32();
	iNumberOfAA75mmType88Lost = aReadStream.ReadInt32();
	iNumberOfAAConcreteKilled = aReadStream.ReadInt32();
	iNumberOfAAConcreteLost = aReadStream.ReadInt32();

	//Ground Units
	iNumberOfMediumTanksKilled = aReadStream.ReadInt32();
	iNumberOfMediumTanksLost = aReadStream.ReadInt32();
	iNumberOfAATanksKilled = aReadStream.ReadInt32();
	iNumberOfAATanksLost = aReadStream.ReadInt32();
	iNumberOfRiflemanKilled = aReadStream.ReadInt32();
	iNumberOfRiflemanLost = aReadStream.ReadInt32();
	iNumberOfAntiTankmanKilled = aReadStream.ReadInt32();
	iNumberOfAntiTankmanLost = aReadStream.ReadInt32();

	//start reading in the extra events
	TInt32 lHasAnotherObject = aReadStream.ReadInt32();
	while(lHasAnotherObject == 1)
	{
		SBonusObjective lTempObjective;
		lTempObjective.ExtraScore = aReadStream.ReadInt32();
		lTempObjective.NameOfObjective = new CString();
		aReadStream.ReadString(lTempObjective.NameOfObjective);

		iBonusObjectives->Append(new SBonusObjective(lTempObjective));
		lHasAnotherObject = aReadStream.ReadInt32(); // check the next object
	}
}
