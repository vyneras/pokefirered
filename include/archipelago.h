#ifndef GUARD_ARCHIPELAGO_H
#define GUARD_ARCHIPELAGO_H

#include "global.h"

#define PLAYER_NAME_BUFFER_SIZE 17 * 250
#define ITEM_NAME_BUFFER_SIZE 36 * 1500
#define NAME_TABLE_BUFFER_SIZE (2 + 2 + 1) * 1500 // 2 bytes for location id, 2 bytes for item name offset, 1 byte for player name id

struct ArchipelagoOptions
{
  /* 0x00 */ bool8 advanceTextWithHoldA;
  /* 0x01 */ u8 receivedItemMessageFilter; // 0 = Show All, 1 = Show Progression Only, 2 = Show None
  /* 0x02 */ bool8 betterShopsEnabled;
  /* 0x03 */ bool8 reusableTms;
  /* 0x04 */ bool8 guaranteedCatch;
  /* 0x05 */ bool8 areTrainersBlind;
  /* 0x06 */ u16 expMultiplierNumerator;
  /* 0x08 */ u16 expMultiplierDenominator;
  /* 0x0A */ bool8 normalizeEncounterRates;

  /* 0x0B */ bool8 openViridianCity;
  /* 0x0C */ u8 route3Requirement; // 0 = Open, 1 = Defeat Brock, 2 = Defeat Any Gym Leader, 3 = Boulder Badge, 4 = Any Badge
  /* 0x0D */ bool8 openCeruleanCity;
  /* 0x0E */ bool8 modifyRoute2;
  /* 0x0F */ bool8 modifyRoute9;
  /* 0x10 */ bool8 blockUndergroundTunnels;
  /* 0x11 */ bool8 route12Boulders;
  /* 0x12 */ bool8 modifyRoute10;
  /* 0x13 */ bool8 modifyRoute12;
  /* 0x14 */ bool8 modifyRoute16;
  /* 0x15 */ bool8 openSilphCo;
  /* 0x16 */ bool8 removeSaffronRockets;
  /* 0x17 */ bool8 modifyRoute23;
  /* 0x18 */ bool8 route23Trees;
  /* 0x19 */ bool8 blockPokemonTower;
  /* 0x1A */ bool8 victoryRoadRocks;
  /* 0x1B */ bool8 earlyFameGossip;
  /* 0x1C */ bool8 blockVermilionSailing;

  /* 0x1D */ bool8 giovanniRequiresGyms;
  /* 0x1E */ u8 giovanniRequiredCount;
  /* 0x1F */ bool8 route22GateRequiresGyms;
  /* 0x10 */ u8 route22GateRequiredCount;
  /* 0x21 */ bool8 route23GuardRequiresGyms;
  /* 0x22 */ u8 route23GuardRequiredCount;
  /* 0x23 */ bool8 eliteFourRequiresGyms;
  /* 0x24 */ u8 eliteFourRequiredCount;
  /* 0x25 */ bool8 eliteFourRematchRequiresGyms;
  /* 0x26 */ u8 eliteFourRematchRequiredCount;
  /* 0x27 */ u8 ceruleanCaveRequirement; // 0 = Vanilla, 1 = Become Champion, 2 = Restore Network Center, 3 = Badges, 4 = Gyms
  /* 0x28 */ u8 ceruleanCaveRequiredCount;

  /* 0x29 */ u32 startingMoney;

  /* 0x2D */ bool8 itemfinderRequired;
  /* 0x2E */ bool8 flashRequired;
  /* 0x2F */ bool8 fameCheckerRequired;

  /* 0x30 */ u8 oaksAideRequiredCounts[5]; // Route 2, Route 10, Route 11, Route 16, Route 15

  /* 0x35 */ bool8 reccuringHiddenItems;
  /* 0x36 */ bool8 isTrainersanity;
  /* 0x37 */ bool8 extraKeyItems;
  /* 0x38 */ bool8 kantoOnly;
  /* 0x39 */ bool8 flyUnlocks;
  /* 0x3A */ bool8 isFamesanity;

  /* 0x3B */ u8 removeBadgeRequirement; // Flash, Cut, Fly, Strength, Surf, Rock Smash, Waterfall
  /* 0x3C */ u8 additionalDarkCaves; // Mt. Moon, Diglett's Cave, Victory Road

  /* 0x3D */ bool8 passesSplit;
  /* 0x3E */ bool8 cardKeysSplit;
  /* 0x3F */ bool8 teasSplit;

  /* 0x40 */ u8 startingLocation;
  /* 0x41 */ u8 freeFlyId;
  /* 0x42 */ u8 townFreeFlyId;
  /* 0x43 */ u16 resortGorgeousMon;
} __attribute__((packed));

struct ArchipelagoReceivedItem
{
    u16 itemId;                  // The id of the item to be received
    u16 itemIndex;               // The index of the item according to the AP server
    bool8 isFilled;              // Whether there is an item in this struct that has not been consumed
    bool8 isProgression;         // Whether a message is progression (for filtering messages)
};

struct ArchipelagoReward
{
    u16 itemId;      // The id of the item to be received
    u16 locationId;  // The flag id that gave this item
};

struct ArchipelagoInfo
{
    u8 auth[16];
};

#define REWARD_QUEUE_SIZE 10
extern struct ArchipelagoReceivedItem gArchipelagoReceivedItem;
extern struct ArchipelagoReward gRewardQueue[REWARD_QUEUE_SIZE];

extern const u16 gArchipelagoStartingItems[];
extern const u16 gArchipelagoStartingItemsCount[];

extern const u8 gArchipelagoPlayerNames[];
extern const u8 gArchipelagoItemNames[];
extern const u8 gArchipelagoNameTable[];

extern const struct ArchipelagoOptions gArchipelagoOptions;
extern const struct ArchipelagoInfo gArchipelagoInfo;
extern bool8 gArchipelagoDeathLinkQueued;

bool8 ArchipelagoSpecial_CanUseHmOutsideBattle(void);
bool8 CanUseHmOutsideBattle(u8 fieldMove);
void SetFlyMapFlag(u8 id);
bool8 IsItemUnique(u16 item);
void GiveStartingItems(void);

#endif // GUARD_ARCHIPELAGO_H
