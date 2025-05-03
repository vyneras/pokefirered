#ifndef GUARD_ARCHIPELAGO_H
#define GUARD_ARCHIPELAGO_H

#include "global.h"

#define PLAYER_NAME_BUFFER_SIZE 17 * 250
#define ITEM_NAME_BUFFER_SIZE 36 * 1600
#define NAME_TABLE_BUFFER_SIZE (2 + 2 + 1) * 1600 // 2 bytes for location id, 2 bytes for item name offset, 1 byte for player name id

struct ArchipelagoOptions
{
    /* 0x00 */ u8 windowFrameType;
    /* 0x01 */ u16 textSpeedOption:3; // 0 = Slow, 1 = Mid, 2 = Fast, 3 = Instant
               u16 turboA:1;
               u16 autoRun:1;
               u16 buttonMode:2; // 0 = Help, 1 = LR, 2 = L=A
               u16 battleScene:1;
               u16 battleStyle:1; // 0 = Shift, 1 = Set
               u16 showEffectiveness:1;
               u16 expMultiplier:3; // 0 = None, 1 = Half, 2 = Normal, 3 = Double, 4 = Triple, 5 = Quadruple, 6 = Custom
               u16 sound:1; // 0 = Mono, 1 = Stereo
               u16 lowHPBeep:1;
               u16 skipFanfares:1;
    /* 0x03 */ u16 bikeMusic:1;
               u16 surfMusic:1;
               u16 guaranteedCatch:1;
               u16 normalizeEncounterRates:1;
               u16 blindTrainers:1;
               u16 itemMessages:2; // 0 = Show All, 1 = Show Progression Only, 2 = Show None
    /* 0x04 */ bool8 betterShopsEnabled;
    /* 0x05 */ bool8 reusableTms;
    /* 0x06 */ u16 expMultiplierNumerator;
    /* 0x08 */ u16 expMultiplierDenominator;
    /* 0x0A */ bool8 unlockSeenDexInfo;
    /* 0x0B */ bool8 physicalSpecialSplit;

    /* 0x0C */ bool8 openViridianCity;
    /* 0x0D */ u8 route3Requirement; // 0 = Open, 1 = Defeat Brock, 2 = Defeat Any Gym Leader, 3 = Boulder Badge, 4 = Any Badge
    /* 0x0E */ bool8 openCeruleanCity;
    /* 0x0F */ bool8 modifyRoute2;
    /* 0x10 */ bool8 modifyRoute9;
    /* 0x11 */ bool8 blockUndergroundTunnels;
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
    /* 0x1D */ bool8 blockVermilionSailing;

    /* 0x1E */ bool8 giovanniRequiresGyms;
    /* 0x1F */ u8 giovanniRequiredCount;
    /* 0x20 */ bool8 route22GateRequiresGyms;
    /* 0x21 */ u8 route22GateRequiredCount;
    /* 0x22 */ bool8 route23GuardRequiresGyms;
    /* 0x23 */ u8 route23GuardRequiredCount;
    /* 0x24 */ bool8 eliteFourRequiresGyms;
    /* 0x25 */ u8 eliteFourRequiredCount;
    /* 0x26 */ bool8 eliteFourRematchRequiresGyms;
    /* 0x27 */ u8 eliteFourRematchRequiredCount;
    /* 0x28 */ u8 ceruleanCaveRequirement; // 0 = Vanilla, 1 = Become Champion, 2 = Restore Network Center, 3 = Badges, 4 = Gyms
    /* 0x29 */ u8 ceruleanCaveRequiredCount;

    /* 0x2A */ u32 startingMoney;

    /* 0x2E */ bool8 itemfinderRequired;
    /* 0x2F */ bool8 flashRequired;
    /* 0x30 */ bool8 fameCheckerRequired;

    /* 0x31 */ u8 oaksAideRequiredCounts[5]; // Route 2, Route 10, Route 11, Route 16, Route 15

    /* 0x36 */ bool8 reccuringHiddenItems;
    /* 0x37 */ bool8 isTrainersanity;
    /* 0x38 */ bool8 isDexsanity;
    /* 0x39 */ bool8 extraKeyItems;
    /* 0x3A */ bool8 kantoOnly;
    /* 0x3B */ bool8 flyUnlocks;
    /* 0x3C */ bool8 isFamesanity;
    /* 0x3D */ bool8 gymKeys;
    /* 0x3E */ bool8 isShopsanity;

    /* 0x3F */ u8 removeBadgeRequirement; // Flash, Cut, Fly, Strength, Surf, Rock Smash, Waterfall
    /* 0x40 */ u8 additionalDarkCaves; // Mt. Moon, Diglett's Cave, Victory Road

    /* 0x41 */ bool8 passesSplit;
    /* 0x42 */ bool8 cardKeysSplit;
    /* 0x43 */ bool8 teasSplit;

    /* 0x44 */ u8 startingLocation;
    /* 0x45 */ u8 freeFlyId;
    /* 0x46 */ u8 townFreeFlyId;
    /* 0x47 */ u16 resortGorgeousMon;
    /* 0x49 */ u16 introSpecies;
    /* 0x4B */ u16 pcItemId;
    /* 0x4D */ bool8 remoteItems;
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
extern bool8 gArchipelagoDeathLinkQueued;

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
