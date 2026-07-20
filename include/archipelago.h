#ifndef GUARD_ARCHIPELAGO_H
#define GUARD_ARCHIPELAGO_H

#include "global.h"

#define PLAYER_NAME_BUFFER_SIZE 17 * 1000
#define ITEM_NAME_BUFFER_SIZE 36 * 2000
#define NAME_TABLE_BUFFER_SIZE (2 + 2 + 2) * 2000 // 2 bytes for location id, 2 bytes for item name offset, 2 bytes for player name id

struct ArchipelagoOptions
{
    /* 0x00 */ u8 windowFrameType;
    /* 0x01 */ u16 expMultiplier;
    /* 0x03 */ u16 textSpeedOption:2; // 0 = Slow, 1 = Mid, 2 = Fast, 3 = Instant
               u16 turboA:2; // 0 = Off, 1 = A, 2 = B, 3 = A/B
               u16 autoRun:1;
               u16 buttonMode:2; // 0 = Help, 1 = L/R, 2 = L=A
               u16 battleScene:1;
               u16 battleStyle:1; // 0 = Shift, 1 = Set
               u16 showEffectiveness:1;
               u16 expDistribution:2; // 0 = Gen III, 1 = Gen VI, 2 = Gen VIII
               u16 sound:1; // 0 = Mono, 1 = Stereo
               u16 lowHPBeep:1;
               u16 skipFanfares:1;
               u16 bikeMusic:1;
    /* 0x05 */ u16 surfMusic:1;
               u16 guaranteedCatch:1;
               u16 guaranteedRun:1;
               u16 encounterRates:1;
               u16 encounterMode:2; // 0 = Random, 1 = Boost, 2 = Rotate
               u16 blindTrainers:1;
               u16 skipNicknames:1;
               u16 itemMessages:2; // 0 = Show All, 1 = Show Progression Only, 2 = Show None
    /* 0x07 */ bool8 betterShops;
    /* 0x08 */ bool8 cheaperCoins;
    /* 0x09 */ bool8 reusableTms;
    /* 0x0A */ bool8 unlockSeenDexInfo;
    /* 0x0B */ bool8 physicalSpecialSplit;

    /* 0x0C */ bool8 openViridianCity;
    /* 0x0D */ u8 route3Requirement; // 0 = Open, 1 = Defeat Brock, 2 = Defeat Any Gym Leader, 3 = Boulder Badge, 4 = Any Badge
    /* 0x0E */ bool8 openCeruleanCity;
    /* 0x0F */ bool8 modifyRoute2;
    /* 0x10 */ bool8 modifyRoute9;
    /* 0x11 */ bool8 blockTunnels;
    /* 0x12 */ bool8 route12Boulders;
    /* 0x13 */ bool8 modifyRoute10;
    /* 0x14 */ bool8 modifyRoute12;
    /* 0x15 */ bool8 modifyRoute16;
    /* 0x16 */ bool8 openSilphCo;
    /* 0x17 */ bool8 removeSaffronRockets;
    /* 0x18 */ bool8 modifyRoute23;
    /* 0x19 */ bool8 route23Trees;
    /* 0x1A */ bool8 blockPokemonTower;
    /* 0x1B */ bool8 victoryRoadRocks;
    /* 0x1C */ bool8 earlyFameGossip;
    /* 0x1D */ bool8 blockSailing;
    /* 0x1E */ u8 elevatorsState; // 0 = Open, 1 = Locked, 2 = Disabled

    /* 0x1F */ bool8 giovanniRequiresGyms;
    /* 0x20 */ u8 giovanniRequiredCount;
    /* 0x21 */ bool8 route22GateRequiresGyms;
    /* 0x22 */ u8 route22GateRequiredCount;
    /* 0x23 */ bool8 route23GuardRequiresGyms;
    /* 0x24 */ u8 route23GuardRequiredCount;
    /* 0x25 */ bool8 eliteFourRequiresGyms;
    /* 0x26 */ u8 eliteFourRequiredCount;
    /* 0x27 */ bool8 eliteFourRematchRequiresGyms;
    /* 0x28 */ u8 eliteFourRematchRequiredCount;
    /* 0x29 */ u8 ceruleanCaveRequirement; // 0 = Vanilla, 1 = Become Champion, 2 = Restore Network Center, 3 = Badges, 4 = Gyms
    /* 0x2A */ u8 ceruleanCaveRequiredCount;
    /* 0x2B */ u8 cinnabarFossilCount;
    /* 0x2C */ u8 rematchRequiresGyms;

    /* 0x2D */ u32 startingMoney;

    /* 0x31 */ bool8 itemfinderRequired;
    /* 0x32 */ bool8 flashRequired;
    /* 0x33 */ bool8 fameCheckerRequired;
    /* 0x34 */ bool8 bikeRequiresJumpingShoes;
    /* 0x35 */ bool8 acrobaticBike;

    /* 0x36 */ u8 oaksAideRequiredCounts[5]; // Route 2, Route 10, Route 11, Route 16, Route 15

    /* 0x3B */ bool8 reccuringHiddenItems;
    /* 0x3C */ bool8 isTrainersanity;
    /* 0x3D */ bool8 isDexsanity;
    /* 0x3E */ bool8 extraKeyItems;
    /* 0x3F */ bool8 kantoOnly;
    /* 0x40 */ bool8 flyUnlocks;
    /* 0x41 */ bool8 isFamesanity;
    /* 0x42 */ bool8 gymKeys;
    /* 0x43 */ bool8 isShopsanity;

    /* 0x44 */ u8 removeBadgeRequirement; // Flash, Cut, Fly, Strength, Surf, Rock Smash, Waterfall
    /* 0x45 */ u8 additionalDarkCaves; // Mt. Moon, Diglett's Cave, Victory Road

    /* 0x46 */ bool8 passesSplit;
    /* 0x47 */ bool8 cardKeysSplit;
    /* 0x48 */ bool8 teasSplit;

    /* 0x49 */ u8 startingLocation;
    /* 0x4A */ u8 startingRespawn;
    /* 0x4B */ u8 freeFlyId;
    /* 0x4C */ u8 townFreeFlyId;
    /* 0x4D */ u16 resortGorgeousMon;
    /* 0x4F */ u16 introSpecies;
    /* 0x51 */ u16 pcItemId;
    /* 0x53 */ bool8 remoteItems;
    /* 0x54 */ bool8 internalEntrancesRandomized;
    /* 0x55 */ bool8 pokemonCenterEntrancesRandomized;
    /* 0x56 */ bool8 skipIntro;
    /* 0x57 */ bool8 randomized;
    /* 0x58 */ u8 version[16];
} __attribute__((packed));

struct ArchipelagoReceivedItem
{
    u16 itemId;                  // The id of the item to be received
    u16 itemIndex;               // The index of the item according to the AP server
    bool8 isFilled;              // Whether there is an item in this struct that has not been consumed
    bool8 isProgression;         // Whether a message is progression (for filtering messages)
};

struct ArchipelagoInfo
{
    u8 auth[16];
};

extern struct ArchipelagoReceivedItem gArchipelagoReceivedItem;

extern const u16 gArchipelagoStartingItems[];
extern const u16 gArchipelagoStartingItemsCount[];

extern const u8 gArchipelagoPlayerNames[];
extern const u8 gArchipelagoItemNames[];
extern const u8 gArchipelagoNameTable[];

extern const struct ArchipelagoOptions gArchipelagoOptions;
extern const struct ArchipelagoInfo gArchipelagoInfo;
extern bool8 gArchipelagoDeathLinkReceived;
extern bool8 gArchipelagoDeathLinkSent;

bool8 CanUseHmOutsideBattle(u8 fieldMove);
void SetFlyMapFlag(u8 id);
bool8 IsItemUnique(u16 item);
void GiveStartingItems(void);
void UnlockAllSeenDexInfo(void);
bool8 CanLeavePewterCity(void);
bool8 CanLeaveCeruleanCity(void);
bool8 CanEnterSilphCo(void);
bool8 HasRequiredBadgesOrGyms(bool8 requiresGyms, u8 requiredCount);
bool8 CanEnterCeruleanCave(void);

#endif // GUARD_ARCHIPELAGO_H
