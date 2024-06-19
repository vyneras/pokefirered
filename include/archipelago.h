#ifndef GUARD_ARCHIPELAGO_H
#define GUARD_ARCHIPELAGO_H

#include "global.h"

#define PLAYER_NAME_BUFFER_SIZE 17 * 250
#define ITEM_NAME_BUFFER_SIZE 36 * 500
#define NAME_TABLE_BUFFER_SIZE (2 + 2 + 1) * 1500 // 2 bytes for location id, 2 bytes for item name offset, 1 byte for player name id

struct ArchipelagoOptions
{
  /* 0x00 */ bool8 advanceTextWithHoldA;
  /* 0x01 */ u8 receivedItemMessageFilter; // 0 = Show All; 1 = Show Progression Only; 2 = Show None
  /* 0x02 */ bool8 betterShopsEnabled;
  /* 0x03 */ bool8 reusableTms;
  /* 0x04 */ bool8 guaranteedCatch;

  /* 0x05 */ bool8 areTrainersBlind;
  /* 0x06 */ u16 expMultiplierNumerator;
  /* 0x08 */ u16 expMultiplierDenominator;

  /* 0x0A */ bool8 openViridianCity;
  /* 0x0B */ u8 route3Requirement; // 0 = Open, 1 = Defeat Brock, 2 = Defeat Any Gym Leader, 3 = Boulder Badge, 4 = Any Badge
  /* 0x0C */ bool8 giovanniRequiresGyms;
  /* 0xD */ u8 giovanniRequiredCount;
  /* 0x0E */ bool8 route22GateRequiresGyms;
  /* 0x0F */ u8 route22GateRequiredCount;
  /* 0x10 */ bool8 route23GuardRequiresGyms;
  /* 0x11 */ u8 route23GuardRequiredCount;
  /* 0x12 */ bool8 eliteFourRequiresGyms;
  /* 0x13 */ u8 eliteFourRequiredCount;
  /* 0x14 */ u8 ceruleanCaveRequirement; // 0 = Vanilla, 1 = Become Champion, 2 = Restore Network Center, 3 = Gyms, 4 = Badges
  /* 0x15 */ u8 ceruleanCaveRequiredCount;

  /* 0x16 */ u8 startingBadges;
  /* 0x17 */ u32 hmTotalBadgeRequirements;
  /* 0x1B */ u8 hmSpecificBadgeRequirements[8];
  /* 0x23 */ u8 freeFlyLocation;

  /* 0x24 */ bool8 itemfinderRequired;
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

#define REWARD_QUEUE_SIZE 10
extern struct ArchipelagoReceivedItem gArchipelagoReceivedItem;
extern struct ArchipelagoReward gRewardQueue[REWARD_QUEUE_SIZE];

extern const u8 gArchipelagoPlayerNames[];
extern const u8 gArchipelagoItemNames[];
extern const u8 gArchipelagoNameTable[];

struct ArchipelagoInfo
{
    u8 auth[16];
};

extern const struct ArchipelagoOptions gArchipelagoOptions;
extern const struct ArchipelagoInfo gArchipelagoInfo;
extern bool8 gArchipelagoDeathLinkQueued;

bool8 ArchipelagoSpecial_CanUseHmOutsideBattle(void);
bool8 CanUseHmOutsideBattle(u8 fieldMove);

#endif // GUARD_ARCHIPELAGO_H
