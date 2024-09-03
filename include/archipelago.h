#ifndef GUARD_ARCHIPELAGO_H
#define GUARD_ARCHIPELAGO_H

#include "global.h"

#define PLAYER_NAME_BUFFER_SIZE 17 * 250
#define ITEM_NAME_BUFFER_SIZE 36 * 500
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

  /* 0x0A */ bool8 openViridianCity;
  /* 0x0B */ u8 route3Requirement; // 0 = Open, 1 = Defeat Brock, 2 = Defeat Any Gym Leader, 3 = Boulder Badge, 4 = Any Badge
  /* 0x0C */ bool8 saveBillRequired;
  /* 0x0D */ bool8 modifyRoute9; // Replace tree with rock
  /* 0x0E */ bool8 blockUndergroundTunnels; // Add rocks around Undeground Tunnel Entrances

  /* 0x0F */ bool8 giovanniRequiresGyms;
  /* 0x10 */ u8 giovanniRequiredCount;
  /* 0x11 */ bool8 route22GateRequiresGyms;
  /* 0x12 */ u8 route22GateRequiredCount;
  /* 0x13 */ bool8 route23GuardRequiresGyms;
  /* 0x14 */ u8 route23GuardRequiredCount;
  /* 0x15 */ bool8 eliteFourRequiresGyms;
  /* 0x16 */ u8 eliteFourRequiredCount;
  /* 0x17 */ u8 ceruleanCaveRequirement; // 0 = Vanilla, 1 = Become Champion, 2 = Restore Network Center, 3 = Badges, 4 = Gyms
  /* 0x18 */ u8 ceruleanCaveRequiredCount;

  /* 0x19 */ u8 startingBadges;
  /* 0x1A */ u32 startingFlyUnlocks;
  /* 0x1E */ u32 startingMoney;

  /* 0x22 */ bool8 itemfinderRequired;
  /* 0x23 */ bool8 fameCheckerRequired;

  /* 0x24 */ u8 oaksAideRequiredCounts[5]; // Route 2, Route 10, Route 11, Route 16, Route 15

  /* 0x29 */ bool8 reccuringHiddenItems;
  /* 0x2A */ bool8 isTrainersanity;
  /* 0x2B */ bool8 extraKeyItems;
  /* 0x2C */ bool8 kantoOnly;
  /* 0x2D */ bool8 flyUnlocks;
  /* 0x2E */ bool8 isFamesanity;

  /* 0x2F */ u8 removeBadgeRequirement; // Flash, Cut, Fly, Strength, Surf, Rock Smash, Waterfall

  /* 0x30 */ bool8 passesSplit;

  /* 0x31 */ u8 free_fly_id;
  /* 0x32 */ u8 town_free_fly_id;
  /* 0x33 */ u16 resort_gorgeous_mon;
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

extern const u8 gArchipelagoPlayerNames[];
extern const u8 gArchipelagoItemNames[];
extern const u8 gArchipelagoNameTable[];

extern const struct ArchipelagoOptions gArchipelagoOptions;
extern const struct ArchipelagoInfo gArchipelagoInfo;
extern bool8 gArchipelagoDeathLinkQueued;

bool8 ArchipelagoSpecial_CanUseHmOutsideBattle(void);
bool8 CanUseHmOutsideBattle(u8 fieldMove);
void SetFlyMapFlag(u8 id);

#endif // GUARD_ARCHIPELAGO_H
