#pragma once

#include <cinttypes>
#include <vector>

#define DECL_STATIC_BLOCK(class_name) static int class_name##__static__
#define STATIC_BLOCK(class_name) int class_name :: class_name##__static__ = []() -> int {
#define END_STATIC_BLOCK return 0; }();

#define NO_COMMANDS 27U
#define TOKEN_OFFSET 1000U
#define INT_OFFSET 1000U


typedef char byte_t;

typedef uint16_t ScriptCode;
typedef int32_t field_value_t;

enum FieldType : uint32_t
{
	Constant = 0,
	User,
	Internal,
	Invalid
};

struct ScriptField
{
	uint32_t type;
	union
	{
		field_value_t value;
		field_value_t index;
	};

	void invalidate();

	bool isInvalid() const;
	bool isConstant() const;
	bool isUser() const;
	bool isInternal() const;

	constexpr static ScriptField invalid() { return { FieldType::Invalid, 3 }; }
};


#define IToken(identifier, code) identifier = (TOKEN_OFFSET + (code))
enum InstructionToken : ScriptCode
{
	IToken(If, 0),
	IToken(Else, 1),
	IToken(Endif, 2),
	IToken(Begin, 3),
	IToken(End, 4),
	IToken(Every, 5),
	IToken(Do, 6),
	IToken(Set, 7),
	IToken(Increment, 8),
	IToken(Decrement, 9),
	IToken(ExpStart, 10),
	IToken(ExpEnd, 11),
	IToken(GreaterThan, 12),
	IToken(LessThan, 13),
	IToken(Equalto, 14),
	IToken(NotEqualTo, 15),
	IToken(GreaterThanEqualTo, 16),
	IToken(LessThanEqualTo, 17),
	IToken(ScriptEnd, 19),
	IToken(And, 20),
	IToken(Or, 21),
	IToken(On, 22),
	IToken(Off, 23),
	IToken(ComputerPlayer, 24),
	IToken(Multiply, 25),
	IToken(Divide, 26)
};
#undef IToken

#define CToken(identifier, code) identifier = (TOKEN_OFFSET + NO_COMMANDS + (code))
enum CommandValueToken : ScriptCode
{
	CToken(CountWild, 31),
	CToken(AttackMarker, 43),
	CToken(AttackBuilding, 44),
	CToken(AttackPerson, 45),
	CToken(AttackNormal, 51),
	CToken(AttackByBoat, 52),
	CToken(AttackByBallon, 53),
	CToken(GuardNormal, 60),
	CToken(GuardWithGhosts, 61),
	CToken(Blue, 91),
	CToken(Red, 92),
	CToken(Yellow, 93),
	CToken(Green, 94)
};

enum CommandToken : ScriptCode
{
	CToken(ConstructBuilding, 1),
	CToken(FetchWood, 2),
	CToken(ShamanGetWilds, 3),
	CToken(HouseAPerson, 4),
	CToken(SendGhosts, 5),
	CToken(BringNewPeopleBack, 6),
	CToken(TrainPeople, 7),
	CToken(PopulateDrumTower, 8),
	CToken(Defend, 9),
	CToken(DefendBase, 10),
	CToken(SpellDefense, 11),
	CToken(Preach, 12),
	CToken(BuildWalls, 13),
	CToken(Sabotage, 14),
	CToken(SpellOffensive, 15),
	CToken(FirewarriorDefend, 16),
	CToken(BuildVehicle, 17),
	CToken(FetchLostPeople, 18),
	CToken(FetchLostVehicle, 19),
	CToken(FetchFarVehicle, 20),
	CToken(AutoAttack, 21),

	CToken(ShamanDefend, 22),
	CToken(FlattenBase, 23),
	CToken(BuildOuterDefences, 24),
	CToken(Spare5, 25),
	CToken(Spare6, 26),
	CToken(Spare7, 27),
	CToken(Spare8, 28),
	CToken(Spare9, 29),
	CToken(Spare10, 30),

	CToken(Attack, 32),
	CToken(AttackBlue, 33),
	CToken(AttackRed, 34),
	CToken(AttackYellow, 35),
	CToken(AttackGreen, 36),
	CToken(SpellAttack, 37),

	CToken(ResetBaseMarker, 38),
	CToken(SetBaseMarker, 39),
	CToken(SetBaseRadius, 40),
	CToken(CountPeopleInMarker, 41),
	CToken(SetDrumTowerPos, 42),

	CToken(ConvertAtMarker, 46),
	CToken(PreachAtMarker, 47),
	CToken(SendGhostPeople, 48),
	CToken(GetSpellsCast, 49),
	CToken(GetNumOneOffSpells, 50),
	CToken(SetAttackVariable, 54),
	CToken(BuildDrumTower, 55),
	CToken(GuardAtMarker, 56),
	CToken(GuardBetweenMarkers, 57),
	CToken(GetHeightAtPos, 58),
	CToken(SendAllPeopleToMarker, 59),
	CToken(ResetConvertMarker, 62),
	CToken(SetConvertMarker, 63),
	CToken(SetMarkerEntry, 64),
	CToken(MarkerEntries, 65),
	CToken(ClearGuardingFrom, 66),
	CToken(SetBuildingDirection, 67),
	CToken(TrainPeopleNow, 68),
	CToken(PrayAtHead, 69),
	CToken(PutPersonInDT, 70),
	CToken(IHaveOneShot, 71),
	CToken(SpellType, 72),
	CToken(BuildingType, 73),
	CToken(BoatPatrol, 74),
	CToken(DefendShamen, 75),
	CToken(SendShamenDefendersHome, 76),
	CToken(BoatType, 77),
	CToken(BallonType, 78),
	CToken(IsBuildingNear, 79),
	CToken(BuildAt, 80),
	CToken(SetSpellEntry, 81),
	CToken(DelayMainDrumTower, 82),
	CToken(BuildMainDrumTower, 83),
	CToken(ZoomTo, 84),
	CToken(DisableUserInputs, 85),
	CToken(EnableUserInputs, 86),
	CToken(OpenDialog, 87),
	CToken(GiveOneShot, 88),
	CToken(ClearStandingPeople, 89),
	CToken(OnlyStandAtMarkers, 90),
	CToken(NavCheck, 95),
	CToken(TargetSWarriors, 96),
	CToken(DontTargetSWarriors, 97),
	CToken(TargetBlueShaman, 98),
	CToken(DontTargetBlueShaman, 99),
	CToken(TargetBlueDrumTowers, 100),
	CToken(DontTargetBlueDrumTowers, 101),
	CToken(HasBlueKilledAGhost, 102),
	CToken(CountGuardFires, 103),
	CToken(GetHeadTriggerCount, 104),
	CToken(MoveShamanToMarker, 105),
	CToken(TrackShamanToAngle, 106),
	CToken(TrackShamanExtraBollocks, 107),
	CToken(IsShamanAvailableForAttack, 108),
	CToken(PartialBuildingCount, 109),
	CToken(SendBluePeopleToMarker, 110),
	CToken(GiveManaToPlayer, 111),
	CToken(IsPlayerInWorldView, 112),
	CToken(SetAutoBuild, 113),
	CToken(DeselectAllBluePeople, 114),
	CToken(FlashButton, 115),
	CToken(TurnPanelOn, 116),
	CToken(GivePlayerSpell, 117),
	CToken(HasPlayerBeenInEncyc, 118),
	CToken(IsBlueShamanSelected, 119),
	CToken(ClearShamanLeftClick, 120),
	CToken(ClearShamanRightClick, 121),
	CToken(IsShamanIconLeftClicked, 122),
	CToken(IsShamanIconRightClicked, 123),
	CToken(TriggerThing, 124),
	CToken(TrackToMarker, 125),
	CToken(CameraRotation, 126),
	CToken(StopCameraRotation, 127),
	CToken(CountBlueShapes, 128),
	CToken(CountBlueInHouses, 129),
	CToken(HasHouseInfoBeenShown, 130),
	CToken(ClearHouseInfoFlag, 131),
	CToken(SetAutoHouse, 132),
	CToken(CountBlueWithBuildCommand, 133),
	CToken(DontHouseSpecialists, 134),
	CToken(TargetPlayerDTAndS, 135),
	CToken(RemovePlayerThing, 136),
	CToken(SetReincarnation, 137),
	CToken(ExtraWoodCollection, 138),
	CToken(SetWoodCollectionRadii, 139),
	CToken(GetNumPeopleConverted, 140),
	CToken(GetNumPeopleBeingPreached, 141),

	CToken(TriggerLevelLost, 142),
	CToken(TriggerLevelWin, 143),

	CToken(RemoveHeadAtPos, 144),
	CToken(SetBucketUsage, 145),
	CToken(SetBucketCountForSpell, 146),
	CToken(CreateMsgNarrative, 147),
	CToken(CreateMsgObjective, 148),
	CToken(CreateMsgInformation, 149),
	CToken(CreateMsgInformationZoom, 150),
	CToken(SetMsgZoom, 151),
	CToken(SetMsgTimeout, 152),
	CToken(SetMsgDeleteOnOk, 153),
	CToken(SetMsgReturnOnOk, 154),
	CToken(SetMsgDeleteOnRmbZoom, 155),
	CToken(SetMsgOpenDlgOnRmbZoom, 156),
	CToken(SetMsgCreateReturnMsgOnRmbZoom, 157),
	CToken(SetMsgOpenDlgOnRmbDelete, 158),
	CToken(SetMsgZoomOnLmbOpenDlg, 159),
	CToken(SetMsgAutoOpenDlg, 160),
	CToken(SetSpecialNoBldgPanel, 161),
	CToken(SetMsgOkSaveExitDlg, 162),
	CToken(FixWildInArea, 163),
	CToken(CheckIfPersonPreachedTo, 164),
	CToken(CountAngels, 165),
	CToken(SetNoBlueReinc, 166),
	CToken(IsShamanInArea, 167),
	CToken(ForceTooltip, 168),
	CToken(SetDefenseRadius, 169),
	CToken(MarvellousHouseDeath, 170),
	CToken(CallToArms, 171),
	CToken(DeleteSmokeStuff, 172),
	CToken(SetTimerGoing, 173),
	CToken(RemoveTimer, 174),
	CToken(HasTimerReachedZero, 175),
	CToken(StartReincNow, 176),
	CToken(TurnPush, 177),
	CToken(FlybyCreateNow, 178),
	CToken(FlybyStart, 179),
	CToken(FlybyStop, 180),
	CToken(FlybyAllowInterrupt, 181),
	CToken(FlybySetEventPos, 182),
	CToken(FlybySetEventAngle, 183),
	CToken(FlybySetEventZoom, 184),
	CToken(FlybySetEventIntPoint, 185),
	CToken(FlybySetEventTooltip, 186),
	CToken(FlybySetEndTarget, 187),
	CToken(FlybySetMessage, 188),
	CToken(KillTeamInArea, 189),
	CToken(ClearAllMsg, 190),
	CToken(SetMsgId, 191),
	CToken(getMsgId, 192),
	CToken(KillAllMsgId, 193),
	CToken(GiveUpAndSulk, 194),
	CToken(AutoMessages, 195),
	CToken(IsPrisionOnLevel, 196)
};
#undef CToken


#define Internal(identifier, code) identifier = (INT_OFFSET + (code))
#define NInternal(identifier, code) identifier = (code)

enum AttributeInternal : ScriptCode
{
	Internal(Expansion, 0),
	Internal(PrefSpyTrains, 1),
	Internal(PrefReligiousTrains, 2),
	Internal(PrefWarriorTrains, 3),
	Internal(PrefFirewarriorTrains, 4),
	Internal(PrefSpyPeople, 5),
	Internal(PrefReligiousPeople, 6),
	Internal(PrefWarriorPeople, 7),
	Internal(PrefFirewariorPeople, 8),
	Internal(MaxBuildingsOnGo, 9),
	Internal(HousePercentage, 10),
	Internal(AwayBrave, 11),
	Internal(AwayWarrior, 12),
	Internal(AwayReligious, 13),
	Internal(DefenseRadIncr, 14),
	Internal(MaxDefensiveActions, 15),
	Internal(AwaySpy, 16),
	Internal(AwayFirewarrior, 17),
	Internal(AttackPercentage, 18),
	Internal(AwayShaman, 19),
	Internal(PeoplePerBoat, 20),
	Internal(PeoplePerBallon, 21),
	Internal(DontUseBoats, 22),
	Internal(MaxSpyAttacks, 23),
	Internal(EnemySpyMaxStand, 24),
	Internal(MaxAttacks, 25),
	Internal(EmptyAtWaypoint, 26),
	Internal(SpyCheckFrequency, 27),
	Internal(RetreatValue, 28),
	Internal(BaseUnderAttackRetreat, 29),
	Internal(RandomBuildSide, 30),
	Internal(UsePreacherForDefense, 31),
	Internal(ShamenBlast, 32),
	Internal(MaxTrainAtOnce, 33),
	Internal(GroupOption, 34),
	Internal(PrefBoatHuts, 35),
	Internal(PrefBallonHuts, 36),
	Internal(PrefBoatDrivers, 37),
	Internal(PrefBallonDrivers, 38),
	Internal(FightStopDistance, 39),
	Internal(SpyDiscoverChance, 40),
	Internal(CountPreachDamage, 41),
	Internal(DontGroupAtDt, 42),
	Internal(SpellDelay, 43),
	Internal(DontDeleteUselessBoatHouse, 44),
	Internal(BoatHouseBroken, 45),
	Internal(DontAutoTrainPreachers, 46),
	Internal(Spare6_, 47)
};


enum ReadOnlyInternal : ScriptCode
{
	NInternal(GameTurn, 0),
	NInternal(MyNumPeople, 1),
	NInternal(BluePeople, 2),
	NInternal(RedPeople, 3),
	NInternal(YellowPeople, 4),
	NInternal(GreenPeople, 5),
	NInternal(MyNumKilledByHuman, 6),
	NInternal(RedRedKilledByHuman, 7),
	NInternal(RedYellowKilledByHuman, 8),
	NInternal(RedGreenKilledByHuman, 9),
	NInternal(RedWildPeople, 10),
	NInternal(BlueMana, 11),
	NInternal(RedMana, 12),
	NInternal(YellowMana, 13),
	NInternal(GreenMana, 14),

	Internal(MyMana, 48),

	Internal(MySpellBurnCost, 49),
	Internal(MySpellBlastCost, 50),
	Internal(MySpellLightning, 51),
	Internal(MySpellWhirlmindCost, 52),
	Internal(MySpellInsectPlagueCost, 53),
	Internal(MySpellInvisibilityCost, 54),
	Internal(MySpellHypnotismCost, 55),
	Internal(MySpellFirestormCost, 56),
	Internal(MySpellGhostArmyCost, 57),
	Internal(MySpellErosionCost, 58),
	Internal(MySpellSwampCost, 59),
	Internal(MySpellLandBridgeCost, 60),
	Internal(MySpellAngelOfDeadCost, 61),
	Internal(MySpellEarthquakeCost, 62),
	Internal(MySpellFlattenCost, 63),
	Internal(MySpellVolcanoCost, 64),
	Internal(MySpellWrathOfGodCost, 65),

	Internal(MyBuildingSmallHut, 66),
	Internal(MyBuildingMediumHut, 67),
	Internal(MyBuildingLargeHut, 68),
	Internal(MyBuildingDrumTower, 69),
	Internal(MyBuildingTemple, 70),
	Internal(MyBuildingSpyTrain, 71),
	Internal(MyBuildingWarriorTrain, 72),
	Internal(MyBuildingFirewarriorTrain, 73),
	Internal(MyBuildingReconversion, 74),
	Internal(MyBuildingWallPiece, 75),
	Internal(MyBuildingGate, 76),
	Internal(MyBuildingCurrOeSlot, 77),
	Internal(MyBuildingBoatHut, 78),
	Internal(MyBuildingBoatHut2, 79),
	Internal(MyBuildingAirshipHut, 80),
	Internal(MyBuildingAirshipHut2, 81),

	Internal(BlueBuildingSmallHut, 82),
	Internal(BlueBuildingMediumHut, 83),
	Internal(BlueBuildingLargeHut, 84),
	Internal(BlueBuildingDrumTower, 85),
	Internal(BlueBuildingTemple, 86),
	Internal(BlueBuildingSpyTrain, 87),
	Internal(BlueBuildingWarriorTrain, 88),
	Internal(BlueBuildingFirewarriorTrain, 89),
	Internal(BlueBuildingReconversion, 90),
	Internal(BlueBuildingWallPiece, 91),
	Internal(BlueBuildingGate, 92),
	Internal(BlueBuildingCurrOeSlot, 93),
	Internal(BlueBuildingBoatHut, 94),
	Internal(BlueBuildingBoatHut2, 95),
	Internal(BlueBuildingAirshipHut, 96),
	Internal(BlueBuildingAirshipHut2, 97),

	Internal(RedBuildingSmallHut, 98),
	Internal(RedBuildingMediumHut, 99),
	Internal(RedBuildingLargeHut, 100),
	Internal(RedBuildingDrumTower, 101),
	Internal(RedBuildingTemple, 102),
	Internal(RedBuildingSpyTrain, 103),
	Internal(RedBuildingWarriorTrain, 104),
	Internal(RedBuildingFirewarriorTrain, 105),
	Internal(RedBuildingReconversion, 106),
	Internal(RedBuildingWallPiece, 107),
	Internal(RedBuildingGate, 108),
	Internal(RedBuildingCurrOeSlot, 109),
	Internal(RedBuildingBoatHut, 110),
	Internal(RedBuildingBoatHut2, 111),
	Internal(RedBuildingAirshipHut, 112),
	Internal(RedBuildingAirshipHut2, 113),

	Internal(YellowBuildingSmallHut, 114),
	Internal(YellowBuildingMediumHut, 115),
	Internal(YellowBuildingLargeHut, 116),
	Internal(YellowBuildingDrumTower, 117),
	Internal(YellowBuildingTemple, 118),
	Internal(YellowBuildingSpyTrain, 119),
	Internal(YellowBuildingWarriorTrain, 120),
	Internal(YellowBuildingFirewarriorTrain, 121),
	Internal(YellowBuildingReconversion, 122),
	Internal(YellowBuildingWallPiece, 123),
	Internal(YellowBuildingGate, 124),
	Internal(YellowBuildingCurrOeSlot, 125),
	Internal(YellowBuildingBoatHut, 126),
	Internal(YellowBuildingBoatHut2, 127),
	Internal(YellowBuildingAirshipHut, 128),
	Internal(YellowBuildingAirshipHut2, 129),

	Internal(GreenBuildingSmallHut, 130),
	Internal(GreenBuildingMediumHut, 131),
	Internal(GreenBuildingLargeHut, 132),
	Internal(GreenBuildingDrumTower, 133),
	Internal(GreenBuildingTemple, 134),
	Internal(GreenBuildingSpyTrain, 135),
	Internal(GreenBuildingWarriorTrain, 136),
	Internal(GreenBuildingFirewarriorTrain, 137),
	Internal(GreenBuildingReconversion, 138),
	Internal(GreenBuildingWallPiece, 139),
	Internal(GreenBuildingGate, 140),
	Internal(GreenBuildingCurrOeSlot, 141),
	Internal(GreenBuildingBoatHut, 142),
	Internal(GreenBuildingBoatHut2, 143),
	Internal(GreenBuildingAirshipHut, 144),
	Internal(GreenBuildingAirshipHut2, 145),

	Internal(MyPersonBrave, 146),
	Internal(MyPersonWarrior, 147),
	Internal(MyPersonReligious, 148),
	Internal(MyPersonSpy, 149),
	Internal(MyPersonFirewarrior, 150),
	Internal(MyPersonShaman, 151),

	Internal(BluePersonBrave, 152),
	Internal(BluePersonWarrior, 153),
	Internal(BluePersonReligious, 154),
	Internal(BluePersonSpy, 155),
	Internal(BluePersonFirewarrior, 156),
	Internal(BluePersonShaman, 157),

	Internal(RedPersonBrave, 158),
	Internal(RedPersonWarrior, 159),
	Internal(RedPersonReligious, 160),
	Internal(RedPersonSpy, 161),
	Internal(RedPersonFirewarrior, 162),
	Internal(RedPersonShaman, 163),

	Internal(YellowPersonBrave, 164),
	Internal(YellowPersonWarrior, 165),
	Internal(YellowPersonReligious, 166),
	Internal(YellowPersonSpy, 167),
	Internal(YellowPersonFirewarrior, 168),
	Internal(YellowPersonShaman, 169),

	Internal(GreenPersonBrave, 170),
	Internal(GreenPersonWarrior, 171),
	Internal(GreenPersonReligious, 172),
	Internal(GreenPersonSpy, 173),
	Internal(GreenPersonFirewarrior, 174),
	Internal(GreenPersonShaman, 175),

	Internal(BlueKilledByMe, 176),
	Internal(RedKilledByMe, 177),
	Internal(YellowKilledByMe, 178),
	Internal(GreenKilledByMe, 179),

	Internal(MyNumKilledByBlue, 180),
	Internal(MyNumKilledByRed, 181),
	Internal(MyNumKilledByYellow, 182),
	Internal(MyNumKilledByGreen, 183),

	Internal(Burn, 184),
	Internal(Blast, 185),
	Internal(LightningBolt, 186),
	Internal(Whirlwind, 187),
	Internal(InsectPlague, 188),
	Internal(Invisibility, 189),
	Internal(Hypnotism, 190),
	Internal(Firestorm, 191),
	Internal(GhostArmy, 192),
	Internal(Erosion, 193),
	Internal(Swamp, 194),
	Internal(LandBridge, 195),
	Internal(AngelOfDead, 196),
	Internal(Earthquake, 197),
	Internal(Flatten, 198),
	Internal(Volcano, 199),
	Internal(WrathOfGod, 200),

	Internal(Brave, 201),
	Internal(Warrior, 202),
	Internal(Religious, 203),
	Internal(Spy, 204),
	Internal(Firewarrior, 205),
	Internal(Shaman, 206),

	Internal(SmallHut, 207),
	Internal(MediumHut, 208),
	Internal(LargeHut, 209),
	Internal(DrumTower, 210),
	Internal(Temple, 211),
	Internal(SpyTrain, 212),
	Internal(WarriorTrain, 213),
	Internal(FirewarriorTrain, 214),
	Internal(Reconversion, 215),
	Internal(WallPiece, 216),
	Internal(Gate, 217),
	Internal(BoatHut, 218),
	Internal(BoatHut2, 219),
	Internal(AirshipHut, 220),
	Internal(AirshipHut2, 221),

	Internal(NoSpecificPerson, 222),
	Internal(NoSpecificBuilding, 223),
	Internal(NoSpecificSpell, 224),

	Internal(TargetShaman, 225),

	Internal(MyVehicleBoat, 226),
	Internal(MyVehicleAirship, 227),

	Internal(BlueVehicleBoat, 228),
	Internal(BlueVehicleAirship, 229),

	Internal(RedVehicleBoat, 230),
	Internal(RedVehicleAirship, 231),

	Internal(YellowVehicleBoat, 232),
	Internal(YellowVehicleAirship, 233),

	Internal(GreenVehicleBoat, 234),
	Internal(GreenVehicleAirship, 235),

	Internal(CpFreeEntries, 236),
	Internal(Random100, 237),

	Internal(NumShamenDefenders, 238),

	Internal(CameraAngle, 239),
	Internal(CameraX, 240),
	Internal(CameraY, 241),

	Internal(MySpellShieldCost, 242),
	Internal(Shield, 243),
	Internal(Convert, 244),
	Internal(Teleport, 245),
	Internal(Bloodlust, 246)
};
#undef Internal
#undef NInternal


template<class _Ty>
void free_ptr_vector(std::vector<_Ty*>& v)
{
	for (_Ty* const p : v)
		if (p)
			delete p;
	v.clear();
}


template<typename _Ty>
void wide_memset(void* const _Dst, const _Ty value, const uintptr_t size)
{
	for (uintptr_t i = 0; i < (size & (~7)); ++i)
		*(reinterpret_cast<_Ty*>(_Dst) + i) = value;
}




