#include "archipelago.h"
#include "event_data.h"
#include "party_menu.h"
#include "util.h"

const struct ArchipelagoOptions gArchipelagoOptions = {
    .advanceTextWithHoldA = FALSE,
    .receivedItemMessageFilter = 0,
    .betterShopsEnabled = FALSE,
    .reusableTms = FALSE,
    .guaranteedCatch = FALSE,

    .areTrainersBlind = FALSE,
    .expMultiplierNumerator = 100,
    .expMultiplierDenominator = 100,

    .openViridianCity = FALSE,
    .route3Requirement = 0,
    .saveBillRequired = TRUE,
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
    .freeFlyLocation = 0,

    .itemfinderRequired = FALSE,
    .reccuringHiddenItems = FALSE,

    .oaksAideRequiredCounts = {
        [0] = 10, // Route 2
        [1] = 20, // Route 10
        [2] = 30, // Route 11
        [3] = 40, // Route 16
        [4] = 50  // Route 15
    },

    .isTrainersanity = FALSE,

    .removeBadgeRequirement = {
      [0] = FALSE, // Flash
      [1] = FALSE, // Cut
      [2] = FALSE, // Fly
      [3] = FALSE, // Strength
      [4] = FALSE, // Surf
      [5] = FALSE, // Rock Smash
      [6] = FALSE  // Waterfall
    }
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
      return FlagGet(FLAG_BADGE01_GET) || gArchipelagoOptions.removeBadgeRequirement[FIELD_MOVE_FLASH];
    }
    else if(fieldMove == FIELD_MOVE_CUT)
    {
      return FlagGet(FLAG_BADGE02_GET) || gArchipelagoOptions.removeBadgeRequirement[FIELD_MOVE_CUT];
    }
    else if(fieldMove == FIELD_MOVE_FLY)
    {
      return FlagGet(FLAG_BADGE03_GET) || gArchipelagoOptions.removeBadgeRequirement[FIELD_MOVE_FLY];
    }
    else if(fieldMove == FIELD_MOVE_STRENGTH)
    {
      return FlagGet(FLAG_BADGE04_GET) || gArchipelagoOptions.removeBadgeRequirement[FIELD_MOVE_STRENGTH];
    }
    else if(fieldMove == FIELD_MOVE_SURF)
    {
      return FlagGet(FLAG_BADGE05_GET) || gArchipelagoOptions.removeBadgeRequirement[FIELD_MOVE_SURF];
    }
    else if(fieldMove == FIELD_MOVE_ROCK_SMASH)
    {
      return FlagGet(FLAG_BADGE06_GET) || gArchipelagoOptions.removeBadgeRequirement[FIELD_MOVE_ROCK_SMASH];
    }
    else if(fieldMove == FIELD_MOVE_WATERFALL)
    {
      return FlagGet(FLAG_BADGE07_GET) || gArchipelagoOptions.removeBadgeRequirement[FIELD_MOVE_WATERFALL];
    }

    return FALSE;
}

bool8 ArchipelagoSpecial_CanUseHmOutsideBattle(void)
{
    return CanUseHmOutsideBattle(gSpecialVar_0x8003);
}
