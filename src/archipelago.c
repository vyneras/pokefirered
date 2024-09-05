#include "archipelago.h"
#include "event_data.h"
#include "map_preview_screen.h"
#include "party_menu.h"
#include "util.h"
#include "constants/heal_locations.h"

const struct ArchipelagoOptions gArchipelagoOptions = {
    .advanceTextWithHoldA = FALSE,
    .receivedItemMessageFilter = 0,
    .betterShopsEnabled = FALSE,
    .reusableTms = FALSE,
    .guaranteedCatch = FALSE,

    .areTrainersBlind = TRUE,
    .expMultiplierNumerator = 100,
    .expMultiplierDenominator = 100,

    .openViridianCity = FALSE,
    .route3Requirement = 1,
    .saveBillRequired = TRUE,
    .modifyRoute2 = FALSE,
    .modifyRoute9 = FALSE,
    .blockUndergroundTunnels = FALSE,
    .route12Boulders = FALSE,
    .modifyRoute10 = FALSE,
    .modifyRoute12 = FALSE,
    .modifyRoute16 = FALSE,
    .modifyRoute23 = FALSE,
    .route23Trees = FALSE,
    .blockPokemonTower = FALSE,
    .victoryRoadRocks = FALSE,

    .giovanniRequiresGyms = FALSE,
    .giovanniRequiredCount = 7,
    .route22GateRequiresGyms = FALSE,
    .route22GateRequiredCount = 7,
    .route23GuardRequiresGyms = FALSE,
    .route23GuardRequiredCount = 7,
    .eliteFourRequiresGyms = FALSE,
    .eliteFourRequiredCount = 8,
    .ceruleanCaveRequirement = 0,
    .ceruleanCaveRequiredCount = 8,

    .startingBadges = 0,
    .startingFlyUnlocks = 0,
    .startingMoney = 3000,

    .itemfinderRequired = FALSE,
    .flashRequired = FALSE,
    .fameCheckerRequired = FALSE,

    .oaksAideRequiredCounts = {
        [0] = 10, // Route 2
        [1] = 20, // Route 10
        [2] = 30, // Route 11
        [3] = 40, // Route 16
        [4] = 50  // Route 15
    },

    .reccuringHiddenItems = FALSE,
    .isTrainersanity = FALSE,
    .extraKeyItems = FALSE,
    .kantoOnly = FALSE,
    .flyUnlocks = FALSE,
    .isFamesanity = FALSE,

    .removeBadgeRequirement = 0,
    .additionalDarkCaves = 0,

    .passesSplit = FALSE,
    .cardKeysSplit = FALSE,

    .startingLocation = SPAWN_PALLET_TOWN,
    .freeFlyId = 0,
    .townFreeFlyId = 0,
    .resortGorgeousMon = 25
};

EWRAM_DATA struct ArchipelagoReceivedItem gArchipelagoReceivedItem = {0};
EWRAM_DATA struct ArchipelagoReward gRewardQueue[10] = {0};

const u8 gArchipelagoPlayerNames[PLAYER_NAME_BUFFER_SIZE] = {0};
const u8 gArchipelagoItemNames[ITEM_NAME_BUFFER_SIZE] = {0};
const u8 gArchipelagoNameTable[NAME_TABLE_BUFFER_SIZE] = {0};

const struct ArchipelagoInfo gArchipelagoInfo = {
    .auth = {0},
};

EWRAM_DATA bool8 gArchipelagoDeathLinkQueued = FALSE;

bool8 ArchipelagoSpecial_CheckReusableTms(void)
{
    return gArchipelagoOptions.reusableTms;
}

bool8 CanUseHmOutsideBattle(u8 fieldMove)
{
    if(fieldMove == FIELD_MOVE_FLASH)
    {
      return FlagGet(FLAG_BADGE01_GET) || (gArchipelagoOptions.removeBadgeRequirement & (1 << FIELD_MOVE_FLASH));
    }
    else if(fieldMove == FIELD_MOVE_CUT)
    {
      return FlagGet(FLAG_BADGE02_GET) || (gArchipelagoOptions.removeBadgeRequirement & (1 << FIELD_MOVE_CUT));
    }
    else if(fieldMove == FIELD_MOVE_FLY)
    {
      return FlagGet(FLAG_BADGE03_GET) || (gArchipelagoOptions.removeBadgeRequirement & (1 << FIELD_MOVE_FLY));
    }
    else if(fieldMove == FIELD_MOVE_STRENGTH)
    {
      return FlagGet(FLAG_BADGE04_GET) || (gArchipelagoOptions.removeBadgeRequirement & (1 << FIELD_MOVE_STRENGTH));
    }
    else if(fieldMove == FIELD_MOVE_SURF)
    {
      return FlagGet(FLAG_BADGE05_GET) || (gArchipelagoOptions.removeBadgeRequirement & (1 << FIELD_MOVE_SURF));
    }
    else if(fieldMove == FIELD_MOVE_ROCK_SMASH)
    {
      return FlagGet(FLAG_BADGE06_GET) || (gArchipelagoOptions.removeBadgeRequirement & (1 << FIELD_MOVE_ROCK_SMASH));
    }
    else if(fieldMove == FIELD_MOVE_WATERFALL)
    {
      return FlagGet(FLAG_BADGE07_GET) || (gArchipelagoOptions.removeBadgeRequirement & (1 << FIELD_MOVE_WATERFALL));
    }

    return FALSE;
}

bool8 ArchipelagoSpecial_CanUseHmOutsideBattle(void)
{
    return CanUseHmOutsideBattle(gSpecialVar_0x8003);
}

void SetFlyMapFlag(u8 id)
{
    u32 flag_id = SYS_FLAGS + 0x8F + id;

    if (flag_id < FLAG_WORLD_MAP_PALLET_TOWN || flag_id > FLAG_WORLD_MAP_ROUTE10_POKEMON_CENTER_1F)
        return;

    MapPreview_SetFlag(flag_id);
}
