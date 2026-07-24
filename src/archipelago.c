#include "archipelago.h"
#include "event_data.h"
#include "item.h"
#include "map_preview_screen.h"
#include "party_menu.h"
#include "pokedex.h"
#include "util.h"
#include "constants/heal_locations.h"
#include "constants/items.h"
#include "constants/species.h"

#define NUM_BADGES_GYMS 8

const u16 gBadgeFlags[NUM_BADGES_GYMS] = {
    FLAG_BADGE01_GET,
    FLAG_BADGE02_GET,
    FLAG_BADGE03_GET,
    FLAG_BADGE04_GET,
    FLAG_BADGE05_GET,
    FLAG_BADGE06_GET,
    FLAG_BADGE07_GET,
    FLAG_BADGE08_GET
};

const u16 gGymFlags[NUM_BADGES_GYMS] = {
    FLAG_DEFEATED_BROCK,
    FLAG_DEFEATED_MISTY,
    FLAG_DEFEATED_LT_SURGE,
    FLAG_DEFEATED_ERIKA,
    FLAG_DEFEATED_KOGA,
    FLAG_DEFEATED_SABRINA,
    FLAG_DEFEATED_BLAINE,
    FLAG_DEFEATED_LEADER_GIOVANNI
};

const struct ArchipelagoOptions gArchipelagoOptions = {
    .windowFrameType = 0,
    .expMultiplier = 100,
    .textSpeedOption = 3,
    .turboA = 0,
    .autoRun = 0,
    .buttonMode = 0,
    .battleScene = 1,
    .battleStyle = 0,
    .showEffectiveness = 1,
    .expDistribution = 0,
    .sound = 0,
    .lowHPBeep = 1,
    .skipFanfares = 0,
    .bikeMusic = 1,
    .surfMusic = 1,
    .guaranteedCatch = 0,
    .guaranteedRun = 0,
    .encounterRates = 0,
    .encounterMode = 0,
    .blindTrainers = 0,
    .skipNicknames = 0,
    .itemMessages = 1,
    .betterShops = FALSE,
    .cheaperCoins = FALSE,
    .reusableTms = FALSE,
    .unlockSeenDexInfo = FALSE,
    .physicalSpecialSplit = FALSE,

    .openViridianCity = FALSE,
    .route3Requirement = 1,
    .openCeruleanCity = FALSE,
    .diglettsCaveRoadblock = 0,
    .route9Roadblock = 0,
    .blockUndergroundPaths = FALSE,
    .route12Boulders = FALSE,
    .route10Waterfall = FALSE,
    .route12Rocks = FALSE,
    .route16Rock = FALSE,
    .openSilphCo = FALSE,
    .removeSaffronRockets = FALSE,
    .route23Waterfall = FALSE,
    .route23Trees = FALSE,
    .blockPokemonTower = FALSE,
    .victoryRoadRocks = FALSE,
    .earlyFameGossip = FALSE,
    .blockSailing = FALSE,
    .elevatorsState = 0,

    .giovanniRequiresGyms = FALSE,
    .giovanniRequiredCount = 7,
    .route22GateRequiresGyms = FALSE,
    .route22GateRequiredCount = 7,
    .route23GuardRequiresGyms = FALSE,
    .route23GuardRequiredCount = 7,
    .eliteFourRequiresGyms = FALSE,
    .eliteFourRequiredCount = 8,
    .eliteFourRematchRequiresGyms = FALSE,
    .eliteFourRematchRequiredCount = 8,
    .ceruleanCaveRequirement = 0,
    .ceruleanCaveRequiredCount = 8,
    .cinnabarFossilCount = 2,
    .rematchRequiresGyms = TRUE,

    .startingMoney = 3000,

    .itemfinderRequired = FALSE,
    .flashRequired = FALSE,
    .fameCheckerRequired = FALSE,
    .bikeRequiresJumpingShoes = TRUE,
    .acrobaticBike = FALSE,

    .oaksAideRequiredCounts = {
        [0] = 10, // Route 2
        [1] = 20, // Route 10
        [2] = 30, // Route 11
        [3] = 40, // Route 16
        [4] = 50  // Route 15
    },

    .reccuringHiddenItems = FALSE,
    .isTrainersanity = FALSE,
    .isDexsanity = FALSE,
    .extraKeyItems = FALSE,
    .kantoOnly = FALSE,
    .flyUnlocks = FALSE,
    .isFamesanity = FALSE,
    .gymKeys = FALSE,
    .isShopsanity = FALSE,

    .removeBadgeRequirement = 0,
    .additionalDarkCaves = 0,

    .passesSplit = FALSE,
    .cardKeysSplit = FALSE,
    .teasSplit = FALSE,

    .startingLocation = SPAWN_PALLET_TOWN,
    .startingRespawn = SPAWN_PALLET_TOWN,
    .freeFlyId = 0,
    .townFreeFlyId = 0,
    .resortGorgeousMon = 25,
    .introSpecies = SPECIES_NIDORAN_F,
    .pcItemId = ITEM_POTION,
    .remoteItems = FALSE,
    .internalEntrancesRandomized = FALSE,
    .pokemonCenterEntrancesRandomized = FALSE,
    .skipIntro = FALSE,
    .randomized = FALSE,
    .version = _("AP DEV")
};

EWRAM_DATA struct ArchipelagoReceivedItem gArchipelagoReceivedItem = {0};

const u16 gArchipelagoStartingItems[ITEMS_COUNT] = {0};
const u16 gArchipelagoStartingItemsCount[ITEMS_COUNT] = {0};

const u8 gArchipelagoPlayerNames[PLAYER_NAME_BUFFER_SIZE] = {0};
const u8 gArchipelagoItemNames[ITEM_NAME_BUFFER_SIZE] = {0};
const u8 gArchipelagoNameTable[NAME_TABLE_BUFFER_SIZE] = {0};

const struct ArchipelagoInfo gArchipelagoInfo = {
    .auth = {0},
};

EWRAM_DATA u8 gArchipelagoDeathLinkReceived = FALSE;
EWRAM_DATA u8 gArchipelagoDeathLinkSent = FALSE;

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

void SetFlyMapFlag(u8 id)
{
    u32 flag_id = SYS_FLAGS + 0x8F + id;

    if (flag_id < FLAG_WORLD_MAP_PALLET_TOWN || flag_id > FLAG_WORLD_MAP_ROUTE10_POKEMON_CENTER_1F)
        return;

    MapPreview_SetFlag(flag_id);
}

bool8 IsItemUnique(u16 item)
{
    return (item >= ITEM_BADGE_1 && item <= ITEM_BADGE_8) ||
           (item >= ITEM_FLY_PALLET && item <= ITEM_FLY_SIX_ISLAND) ||
           (item >= ITEM_COINS_10 && item <= ITEM_COINS_100) ||
           (item >= ITEM_PROG_PASS && item <= ITEM_PROG_ROD) ||
           item == ITEM_RUNNING_SHOES ||
           item == ITEM_JUMPING_SHOES ||
           item == ITEM_POKEDEX;
}

void GiveStartingItems(void)
{
    u16 i;

    for (i = 0; i < ITEMS_COUNT; i++)
    {
        u16 item = gArchipelagoStartingItems[i];
        u16 count = gArchipelagoStartingItemsCount[i];
        if (item != ITEM_NONE && count > 0)
        {
            AddBagItem(item, count);
        }
    }
}

void UnlockAllSeenDexInfo(void)
{
    u16 i;

    for (i = 1; i <= NATIONAL_DEX_COUNT; i++)
    {
        GetSetPokedexFlag(i, FLAG_SET_SEEN);
    }
}

bool8 CanLeavePewterCity(void)
{
    u8 i;

    if (gArchipelagoOptions.route3Requirement == 1)
    {
        return FlagGet(FLAG_DEFEATED_BROCK);
    }
    else if (gArchipelagoOptions.route3Requirement == 2)
    {
        for (i = 0; i < NUM_BADGES_GYMS; i++)
        {
            if (FlagGet(gGymFlags[i]))
            {
                return TRUE;
            }
        }

        return FALSE;
    }
    else if (gArchipelagoOptions.route3Requirement == 3)
    {
        return FlagGet(FLAG_BADGE01_GET);
    }
    else if (gArchipelagoOptions.route3Requirement == 4)
    {
        for (i = 0; i < NUM_BADGES_GYMS; i++)
        {
            if (FlagGet(gBadgeFlags[i]))
            {
                return TRUE;
            }
        }

        return FALSE;
    }

    return TRUE;
}

bool8 CanLeaveCeruleanCity(void)
{
    return FlagGet(FLAG_GOT_SS_TICKET) || gArchipelagoOptions.openCeruleanCity;
}

bool8 CanEnterSilphCo(void)
{
    return FlagGet(FLAG_RESCUED_MR_FUJI) || FlagGet(FLAG_HIDE_SAFFRON_ROCKETS) || gArchipelagoOptions.openSilphCo;
}

bool8 HasRequiredBadgesOrGyms(bool8 requiresGyms, u8 requiredCount)
{
    u8 i;
    u8 count = 0;

    if (requiresGyms)
    {
        for (i = 0; i < NUM_BADGES_GYMS; i++)
        {
            if (FlagGet(gGymFlags[i]))
            {
                count++;
            }
        }
    }
    else
    {
        for (i = 0; i < NUM_BADGES_GYMS; i++)
        {
            if (FlagGet(gBadgeFlags[i]))
            {
                count++;
            }
        }
    }

    return count >= requiredCount;
}

bool8 CanEnterCeruleanCave(void)
{
    u8 i;
    u8 count = 0;

    if(gArchipelagoOptions.ceruleanCaveRequirement == 0)
    {
        return FlagGet(FLAG_SYS_GAME_CLEAR) && FlagGet(FLAG_SYS_CAN_LINK_WITH_RS);
    }
    else if (gArchipelagoOptions.ceruleanCaveRequirement == 1)
    {
        return FlagGet(FLAG_SYS_GAME_CLEAR);
    }
    else if (gArchipelagoOptions.ceruleanCaveRequirement == 2)
    {
        return FlagGet(FLAG_SYS_CAN_LINK_WITH_RS);
    }
    else if (gArchipelagoOptions.ceruleanCaveRequirement == 3)
    {
        for (i = 0; i < NUM_BADGES_GYMS; i++)
        {
            if (FlagGet(gBadgeFlags[i]))
            {
                count++;
            }
        }
    }
    else
    {
        for (i = 0; i < NUM_BADGES_GYMS; i++)
        {
            if (FlagGet(gGymFlags[i]))
            {
                count++;
            }
        }
    }

    return count >= gArchipelagoOptions.ceruleanCaveRequiredCount;
}
