#include "global.h"
#include "gflib.h"
#include "archipelago.h"
#include "random.h"
#include "overworld.h"
#include "constants/maps.h"
#include "load_save.h"
#include "item_menu.h"
#include "tm_case.h"
#include "berry_pouch.h"
#include "quest_log.h"
#include "wild_encounter.h"
#include "event_data.h"
#include "mail_data.h"
#include "map_preview_screen.h"
#include "play_time.h"
#include "money.h"
#include "battle_records.h"
#include "pokemon_size_record.h"
#include "pokemon_storage_system.h"
#include "roamer.h"
#include "item.h"
#include "player_pc.h"
#include "berry.h"
#include "easy_chat.h"
#include "union_room_chat.h"
#include "mystery_gift.h"
#include "renewable_hidden_items.h"
#include "trainer_tower.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "berry_powder.h"
#include "pokemon_jump.h"
#include "event_scripts.h"
#include "field_specials.h"
#include "constants/items.h"

// this file's functions
static void ResetMiniGamesResults(void);

// EWRAM vars
EWRAM_DATA bool8 gDifferentSaveFile = FALSE;

void SetTrainerId(u32 trainerId, u8 *dst)
{
    dst[0] = trainerId;
    dst[1] = trainerId >> 8;
    dst[2] = trainerId >> 16;
    dst[3] = trainerId >> 24;
}

void CopyTrainerId(u8 *dst, u8 *src)
{
    s32 i;
    for (i = 0; i < 4; i++)
        dst[i] = src[i];
}

static void InitPlayerTrainerId(void)
{
    u32 trainerId = (Random() << 0x10) | GetGeneratedTrainerIdLower();
    SetTrainerId(trainerId, gSaveBlock2Ptr->playerTrainerId);
}

static void SetDefaultOptions(void)
{
    gSaveBlock2Ptr->optionsWindowFrameType = gArchipelagoOptions.windowFrameType;
    gSaveBlock2Ptr->optionsTextSpeed = gArchipelagoOptions.textSpeedOption;
    gSaveBlock2Ptr->optionsTurboA = gArchipelagoOptions.turboA;
    gSaveBlock2Ptr->optionsAutoRun = gArchipelagoOptions.autoRun;
    gSaveBlock2Ptr->optionsButtonMode = gArchipelagoOptions.buttonMode;
    gSaveBlock2Ptr->optionsBattleScene = gArchipelagoOptions.battleScene;
    gSaveBlock2Ptr->optionsBattleStyle = gArchipelagoOptions.battleStyle;
    gSaveBlock2Ptr->optionsShowEffectiveness = gArchipelagoOptions.showEffectiveness;
    gSaveBlock2Ptr->optionsExpMultiplier = gArchipelagoOptions.expMultiplier;
    gSaveBlock2Ptr->optionsExpDistribution = gArchipelagoOptions.expDistribution;
    gSaveBlock2Ptr->optionsSound = gArchipelagoOptions.sound;
    gSaveBlock2Ptr->optionsLowHPBeep = gArchipelagoOptions.lowHPBeep;
    gSaveBlock2Ptr->optionsSkipFanfares = gArchipelagoOptions.skipFanfares;
    gSaveBlock2Ptr->optionsBikeMusic = gArchipelagoOptions.bikeMusic;
    gSaveBlock2Ptr->optionsSurfMusic = gArchipelagoOptions.surfMusic;
    gSaveBlock2Ptr->optionsGuaranteedCatch = gArchipelagoOptions.guaranteedCatch;
    gSaveBlock2Ptr->optionsGuaranteedRun = gArchipelagoOptions.guaranteedRun;
    gSaveBlock2Ptr->optionsEncounterRates = gArchipelagoOptions.encounterRates;
    gSaveBlock2Ptr->optionsBlindTrainers = gArchipelagoOptions.blindTrainers;
    gSaveBlock2Ptr->optionsSkipNicknames = gArchipelagoOptions.skipNicknames;
    gSaveBlock2Ptr->optionsItemMessages = gArchipelagoOptions.itemMessages;
    gSaveBlock2Ptr->regionMapZoom = FALSE;
}

static void ClearPokedexFlags(void)
{
    memset(&gSaveBlock2Ptr->pokedex.owned, 0, sizeof(gSaveBlock2Ptr->pokedex.owned));
    memset(&gSaveBlock2Ptr->pokedex.seen, 0, sizeof(gSaveBlock2Ptr->pokedex.seen));
}

static void ClearBattleTower(void)
{
    CpuFill32(0, &gSaveBlock2Ptr->battleTower, sizeof(gSaveBlock2Ptr->battleTower));
}

static void WarpToPlayersRoom(void)
{
    SetLastHealLocationWarp(gArchipelagoOptions.startingRespawn);
    SetTeleportLocationWarp(gArchipelagoOptions.startingLocation);
    SetWarpDestinationToHealLocation(gArchipelagoOptions.startingLocation);
    WarpIntoMap();
}

void Sav2_ClearSetDefault(void)
{
    ClearSav2();
    SetDefaultOptions();
}

void ResetMenuAndMonGlobals(void)
{
    gDifferentSaveFile = FALSE;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetBagCursorPositions();
    ResetTMCaseCursorPos();
    BerryPouch_CursorResetToTop();
    ResetQuestLog();
    SeedWildEncounterRng(Random());
    ResetSpecialVars();
}

void NewGameInitData(void)
{
    u8 i;
    u8 rivalName[PLAYER_NAME_LENGTH + 1];

    StringCopy(rivalName, gSaveBlock1Ptr->rivalName);
    gDifferentSaveFile = TRUE;
    gSaveBlock2Ptr->encryptionKey = 0;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ClearBattleTower();
    ClearSav1();
    ClearMailData();
    gSaveBlock2Ptr->progressivePassesCount = 0;
    gSaveBlock2Ptr->progressiveCardKeyCount = 0;
    gSaveBlock2Ptr->progressiveRod = 0;
    gSaveBlock2Ptr->specialSaveWarpFlags = 0;
    gSaveBlock2Ptr->gcnLinkFlags = 0;
    gSaveBlock2Ptr->unkFlag1 = TRUE;
    gSaveBlock2Ptr->unkFlag2 = FALSE;
    InitPlayerTrainerId();
    PlayTimeCounter_Reset();
    ClearPokedexFlags();
    InitEventData();
    ResetFameChecker();
    SetMoney(&gSaveBlock1Ptr->money, gArchipelagoOptions.startingMoney);
    ResetGameStats();
    ClearPlayerLinkBattleRecords();
    InitHeracrossSizeRecord();
    InitMagikarpSizeRecord();
    EnableNationalPokedex();
    EnableNationalPokedex_RSE();
    gPlayerPartyCount = 0;
    ZeroPlayerPartyMons();
    ResetPokemonStorageSystem();
    ClearRoamerData();
    gSaveBlock1Ptr->registeredItem = 0;
    ClearBag();
    NewGameInitPCItems();
    ClearEnigmaBerries();
    InitEasyChatPhrases();
    ResetTrainerFanClub();
    UnionRoomChat_InitializeRegisteredTexts();
    ResetMiniGamesResults();
    ClearMysteryGift();
    WarpToPlayersRoom();
    RunScriptImmediately(EventScript_ResetAllMapFlags);
    StringCopy(gSaveBlock1Ptr->rivalName, rivalName);
    ResetTrainerTowerResults();

    FlagSet(FLAG_SYS_SEVII_MAP_123);
    FlagSet(FLAG_SYS_SEVII_MAP_4567);
    FlagSet(FLAG_SYS_POKEMON_GET);
    GiveStartingItems();
    VarSet(VAR_STARTER_MON, gSaveBlock2Ptr->starterIndex);
    ScriptGiveMon(GetStarterSpecies(), 5, 0, 0, 0, 0);

    switch (VarGet(VAR_STARTER_MON))
    {
    case 0:
        FlagSet(FLAG_HIDE_BULBASAUR_BALL);
        FlagSet(FLAG_HIDE_CHARMANDER_BALL);
        break;
    case 1:
        FlagSet(FLAG_HIDE_SQUIRTLE_BALL);
        FlagSet(FLAG_HIDE_BULBASAUR_BALL);
        break;
    case 2:
        FlagSet(FLAG_HIDE_CHARMANDER_BALL);
        FlagSet(FLAG_HIDE_SQUIRTLE_BALL);
        break;
    default:
        break;
    }

    SetFlyMapFlag(gArchipelagoOptions.freeFlyId);
    VarSet(VAR_RESORT_GORGEOUS_REQUESTED_MON, gArchipelagoOptions.resortGorgeousMon);

    if (gArchipelagoOptions.unlockSeenDexInfo) UnlockAllSeenDexInfo();
    if (!gArchipelagoOptions.reccuringHiddenItems) SetAllRenewableItemFlags();
    if (gArchipelagoOptions.openViridianCity) VarSet(VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 1);
    if (!gArchipelagoOptions.route12Boulders) FlagSet(FLAG_HIDE_ROUTE_12_BOULDERS);
    if (!gArchipelagoOptions.blockPokemonTower) VarSet(VAR_MAP_SCENE_POKEMON_TOWER_1F, 1);
    if (gArchipelagoOptions.blockPokemonTower) VarSet(VAR_MAP_SCENE_POKEMON_TOWER_6F, 1);
    if (gArchipelagoOptions.removeSaffronRockets)
    {
        FlagSet(FLAG_HIDE_SAFFRON_ROCKETS);
        FlagClear(FLAG_HIDE_SAFFRON_CIVILIANS);
    }
    if (gArchipelagoOptions.earlyFameGossip) FlagClear(FLAG_HIDE_POSTGAME_GOSSIPERS);
    if (!gArchipelagoOptions.kantoOnly) FlagSet(FLAG_HIDE_MOVE_MANIAC);

    if (!(gArchipelagoOptions.additionalDarkCaves & (1 << 0))) VarSet(VAR_MT_MOON_DARKNESS, 1);
    if (!(gArchipelagoOptions.additionalDarkCaves & (1 << 1))) VarSet(VAR_DIGLETTS_CAVE_DARKNESS, 1);
    if (!(gArchipelagoOptions.additionalDarkCaves & (1 << 2))) VarSet(VAR_VICTORY_ROAD_DARKNESS, 1);

    if (!gArchipelagoOptions.extraKeyItems) RunScriptImmediately(EventScript_SetExtraKeyItemFlags);
    if (!gArchipelagoOptions.cardKeysSplit) RunScriptImmediately(EventScript_SetSplitCardKeyItemFlags);
    if (!gArchipelagoOptions.gymKeys) RunScriptImmediately(EventScript_SetGymKeysFlags);

    if (!gArchipelagoOptions.randomized)
    {
        AddBagItem(ITEM_BERRY_POUCH, 1);
        AddBagItem(ITEM_TM_CASE, 1);
        AddBagItem(ITEM_JUMPING_SHOES, 1);
    }

    SetPokemonCenterShopStartingFlags();

    if (FlagGet(FLAG_SYS_POKEDEX_GET))
    	gSaveBlock1Ptr->dexsanityItemsGiven = TRUE;
}

static void ResetMiniGamesResults(void)
{
    CpuFill16(0, &gSaveBlock2Ptr->berryCrush, sizeof(struct BerryCrush));
    SetBerryPowder(&gSaveBlock2Ptr->berryCrush.berryPowderAmount, 0);
    ResetPokemonJumpRecords();
    CpuFill16(0, &gSaveBlock2Ptr->berryPick, sizeof(struct BerryPickingResults));
}
