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
#include "berry_powder.h"
#include "pokemon_jump.h"
#include "event_scripts.h"

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
    gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_MID;
    gSaveBlock2Ptr->optionsWindowFrameType = 0;
    gSaveBlock2Ptr->optionsSound = OPTIONS_SOUND_MONO;
    gSaveBlock2Ptr->optionsBattleStyle = OPTIONS_BATTLE_STYLE_SHIFT;
    gSaveBlock2Ptr->optionsBattleSceneOff = FALSE;
    gSaveBlock2Ptr->regionMapZoom = FALSE;
    gSaveBlock2Ptr->optionsButtonMode = OPTIONS_BUTTON_MODE_HELP;
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
    SetWarpDestination(MAP_GROUP(PALLET_TOWN_PLAYERS_HOUSE_2F), MAP_NUM(PALLET_TOWN_PLAYERS_HOUSE_2F), -1, 6, 6);
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
    u8 rivalName[PLAYER_NAME_LENGTH + 1];

    StringCopy(rivalName, gSaveBlock1Ptr->rivalName);
    gDifferentSaveFile = TRUE;
    gSaveBlock2Ptr->encryptionKey = 0;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ClearBattleTower();
    ClearSav1();
    ClearMailData();
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
    SetFlyMapFlag(gArchipelagoOptions.freeFlyId);
    VarSet(VAR_RESORT_GORGEOUS_REQUESTED_MON, gArchipelagoOptions.resortGorgeousMon);

    if (!gArchipelagoOptions.reccuringHiddenItems) SetAllRenewableItemFlags();
    if (gArchipelagoOptions.betterShopsEnabled) FlagSet(FLAG_BETTER_SHOPS_ENABLED);
    if (gArchipelagoOptions.openViridianCity) VarSet(VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 1);
    if (!gArchipelagoOptions.route12Boulders) FlagSet(FLAG_HIDE_ROUTE_12_BOULDERS);
    if (!gArchipelagoOptions.blockPokemonTower) VarSet(VAR_MAP_SCENE_POKEMON_TOWER_1F, 1);

    if (!(gArchipelagoOptions.additionalDarkCaves & (1 << 0))) VarSet(VAR_MT_MOON_DARKNESS, 1);
    if (!(gArchipelagoOptions.additionalDarkCaves & (1 << 1))) VarSet(VAR_DIGLETTS_CAVE_DARKNESS, 1);
    if (!(gArchipelagoOptions.additionalDarkCaves & (1 << 2))) VarSet(VAR_VICTORY_ROAD_DARKNESS, 1);

    if (gArchipelagoOptions.startingBadges & (1 << 0)) FlagSet(FLAG_BADGE01_GET);
    if (gArchipelagoOptions.startingBadges & (1 << 1)) FlagSet(FLAG_BADGE02_GET);
    if (gArchipelagoOptions.startingBadges & (1 << 2)) FlagSet(FLAG_BADGE03_GET);
    if (gArchipelagoOptions.startingBadges & (1 << 3)) FlagSet(FLAG_BADGE04_GET);
    if (gArchipelagoOptions.startingBadges & (1 << 4)) FlagSet(FLAG_BADGE05_GET);
    if (gArchipelagoOptions.startingBadges & (1 << 5)) FlagSet(FLAG_BADGE06_GET);
    if (gArchipelagoOptions.startingBadges & (1 << 6)) FlagSet(FLAG_BADGE07_GET);
    if (gArchipelagoOptions.startingBadges & (1 << 7)) FlagSet(FLAG_BADGE08_GET);

    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 0)) MapPreview_SetFlag(FLAG_WORLD_MAP_PALLET_TOWN);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 1)) MapPreview_SetFlag(FLAG_WORLD_MAP_VIRIDIAN_CITY);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 2)) MapPreview_SetFlag(FLAG_WORLD_MAP_PEWTER_CITY);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 3)) MapPreview_SetFlag(FLAG_WORLD_MAP_CERULEAN_CITY);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 4)) MapPreview_SetFlag(FLAG_WORLD_MAP_LAVENDER_TOWN);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 5)) MapPreview_SetFlag(FLAG_WORLD_MAP_VERMILION_CITY);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 6)) MapPreview_SetFlag(FLAG_WORLD_MAP_CELADON_CITY);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 7)) MapPreview_SetFlag(FLAG_WORLD_MAP_FUCHSIA_CITY);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 8)) MapPreview_SetFlag(FLAG_WORLD_MAP_CINNABAR_ISLAND);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 9)) MapPreview_SetFlag(FLAG_WORLD_MAP_INDIGO_PLATEAU_EXTERIOR);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 10)) MapPreview_SetFlag(FLAG_WORLD_MAP_SAFFRON_CITY);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 11)) MapPreview_SetFlag(FLAG_WORLD_MAP_ONE_ISLAND);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 12)) MapPreview_SetFlag(FLAG_WORLD_MAP_TWO_ISLAND);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 13)) MapPreview_SetFlag(FLAG_WORLD_MAP_THREE_ISLAND);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 14)) MapPreview_SetFlag(FLAG_WORLD_MAP_FOUR_ISLAND);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 15)) MapPreview_SetFlag(FLAG_WORLD_MAP_FIVE_ISLAND);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 16)) MapPreview_SetFlag(FLAG_WORLD_MAP_SEVEN_ISLAND);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 17)) MapPreview_SetFlag(FLAG_WORLD_MAP_SIX_ISLAND);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 18)) MapPreview_SetFlag(FLAG_WORLD_MAP_ROUTE4_POKEMON_CENTER_1F);
    if (gArchipelagoOptions.startingFlyUnlocks & (1 << 19)) MapPreview_SetFlag(FLAG_WORLD_MAP_ROUTE10_POKEMON_CENTER_1F);

    if (!gArchipelagoOptions.extraKeyItems) RunScriptImmediately(EventScript_SetExtraKeyItemFlags);
}

static void ResetMiniGamesResults(void)
{
    CpuFill16(0, &gSaveBlock2Ptr->berryCrush, sizeof(struct BerryCrush));
    SetBerryPowder(&gSaveBlock2Ptr->berryCrush.berryPowderAmount, 0);
    ResetPokemonJumpRecords();
    CpuFill16(0, &gSaveBlock2Ptr->berryPick, sizeof(struct BerryPickingResults));
}
