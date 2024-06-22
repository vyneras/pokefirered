#include "archipelago.h"
#include "event_data.h"
#include "util.h"

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
    .route3Requirement = 0,
    .saveBillRequired = TRUE,
    .giovanniRequiresGyms = FALSE,
    .giovanniRequiredCount = 7,
    .route22GateRequiresGyms = FALSE,
    .route22GateRequiredCount = 7,
    .route23GuardRequiresGyms = FALSE,
    .route23GuardRequiredCount = 7,
    .eliteFourRequiresGyms = TRUE,
    .eliteFourRequiredCount = 1,
    .ceruleanCaveRequirement = 0,
    .ceruleanCaveRequiredCount = 8,

    .startingBadges = 0,
    .hmTotalBadgeRequirements = 0xFFFFFFFF,
    .hmSpecificBadgeRequirements = {
        [0] = 1 << 0,
        [1] = 1 << 1,
        [2] = 1 << 2,
        [3] = 1 << 3,
        [4] = 1 << 4,
        [5] = 1 << 5,
        [6] = 1 << 6,
        [7] = 1 << 7,
    }, // Field move order. See src/party_menu.c
    .freeFlyLocation = 0,

    .itemfinderRequired = FALSE,

    .oaksAideRequiredCounts = {
        [0] = 10, // Route 2
        [1] = 20, // Route 10
        [2] = 30, // Route 11
        [3] = 40, // Route 16
        [4] = 50 // Route 15
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
    u8 numRequiredBadges = (gArchipelagoOptions.hmTotalBadgeRequirements >> (fieldMove * 4)) & 0xF;
    u8 badgeFilter = 0;

    if (numRequiredBadges <= 8)
    {
        u8 numBadges = 0;
        numBadges += FlagGet(FLAG_BADGE01_GET);
        numBadges += FlagGet(FLAG_BADGE02_GET);
        numBadges += FlagGet(FLAG_BADGE03_GET);
        numBadges += FlagGet(FLAG_BADGE04_GET);
        numBadges += FlagGet(FLAG_BADGE05_GET);
        numBadges += FlagGet(FLAG_BADGE06_GET);
        numBadges += FlagGet(FLAG_BADGE07_GET);
        numBadges += FlagGet(FLAG_BADGE08_GET);

        return numBadges >= numRequiredBadges;
    }

    if (FlagGet(FLAG_BADGE01_GET))
        badgeFilter |= gBitTable[0];
    if (FlagGet(FLAG_BADGE02_GET))
        badgeFilter |= gBitTable[1];
    if (FlagGet(FLAG_BADGE03_GET))
        badgeFilter |= gBitTable[2];
    if (FlagGet(FLAG_BADGE04_GET))
        badgeFilter |= gBitTable[3];
    if (FlagGet(FLAG_BADGE05_GET))
        badgeFilter |= gBitTable[4];
    if (FlagGet(FLAG_BADGE06_GET))
        badgeFilter |= gBitTable[5];
    if (FlagGet(FLAG_BADGE07_GET))
        badgeFilter |= gBitTable[6];
    if (FlagGet(FLAG_BADGE08_GET))
        badgeFilter |= gBitTable[7];

    return (gArchipelagoOptions.hmSpecificBadgeRequirements[fieldMove] & badgeFilter) == gArchipelagoOptions.hmSpecificBadgeRequirements[fieldMove];
}

bool8 ArchipelagoSpecial_CanUseHmOutsideBattle(void)
{
    return CanUseHmOutsideBattle(gSpecialVar_0x8003);
}
