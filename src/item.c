#include "global.h"
#include "gflib.h"
#include "archipelago.h"
#include "berry.h"
#include "coins.h"
#include "event_data.h"
#include "item.h"
#include "item_use.h"
#include "load_save.h"
#include "map_preview_screen.h"
#include "quest_log.h"
#include "strings.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/shops.h"

EWRAM_DATA struct BagPocket gBagPockets[NUM_BAG_POCKETS] = {};

void SortAndCompactBagPocket(struct BagPocket * pocket);

const u8 gFlyUnlockNames[NUM_FLY_UNLOCKS][FLY_UNLOCK_NAME_LENGTH + 1] = {
    [ITEM_FLY_PALLET - ITEM_FLY_PALLET]       = _("PALLET TOWN"),
    [ITEM_FLY_VIRIDIAN - ITEM_FLY_PALLET]     = _("VIRIDIAN CITY"),
    [ITEM_FLY_PEWTER - ITEM_FLY_PALLET]       = _("PEWTER CITY"),
    [ITEM_FLY_CERULEAN - ITEM_FLY_PALLET]     = _("CERULEAN CITY"),
    [ITEM_FLY_LAVENDER - ITEM_FLY_PALLET]     = _("LAVENDER TOWN"),
    [ITEM_FLY_VERMILION - ITEM_FLY_PALLET]    = _("VERMILION CITY"),
    [ITEM_FLY_CELADON - ITEM_FLY_PALLET]      = _("CELADON CITY"),
    [ITEM_FLY_FUCHSIA - ITEM_FLY_PALLET]      = _("FUCHSIA CITY"),
    [ITEM_FLY_CINNABAR - ITEM_FLY_PALLET]     = _("CINNABAR ISLAND"),
    [ITEM_FLY_INDIGO - ITEM_FLY_PALLET]       = _("INDIGO PLATEAU"),
    [ITEM_FLY_SAFFRON - ITEM_FLY_PALLET]      = _("SAFFRON CITY"),
    [ITEM_FLY_ROUTE4 - ITEM_FLY_PALLET]       = _("ROUTE 4"),
    [ITEM_FLY_ROUTE10 - ITEM_FLY_PALLET]      = _("ROUTE 10"),
    [ITEM_FLY_ONE_ISLAND - ITEM_FLY_PALLET]   = _("ONE ISLAND"),
    [ITEM_FLY_TWO_ISLAND - ITEM_FLY_PALLET]   = _("TWO ISLAND"),
    [ITEM_FLY_THREE_ISLAND - ITEM_FLY_PALLET] = _("THREE ISLAND"),
    [ITEM_FLY_FOUR_ISLAND - ITEM_FLY_PALLET]  = _("FOUR ISLAND"),
    [ITEM_FLY_FIVE_ISLAND - ITEM_FLY_PALLET]  = _("FIVE ISLAND"),
    [ITEM_FLY_SEVEN_ISLAND - ITEM_FLY_PALLET] = _("SEVEN ISLAND"),
    [ITEM_FLY_SIX_ISLAND - ITEM_FLY_PALLET]   = _("SIX ISLAND")
};

const u16 gPokemonCenterShopItems[] = {
    ITEM_POKE_BALL,
    ITEM_GREAT_BALL,
    ITEM_ULTRA_BALL,
    ITEM_POTION,
    ITEM_SUPER_POTION,
    ITEM_HYPER_POTION,
    ITEM_MAX_POTION,
    ITEM_FULL_RESTORE,
    ITEM_REVIVE,
    ITEM_ANTIDOTE,
    ITEM_PARALYZE_HEAL,
    ITEM_AWAKENING,
    ITEM_BURN_HEAL,
    ITEM_ICE_HEAL,
    ITEM_FULL_HEAL,
    ITEM_ESCAPE_ROPE,
    ITEM_REPEL,
    ITEM_SUPER_REPEL,
    ITEM_MAX_REPEL,
    ITEM_X_ATTACK,
    ITEM_X_DEFEND,
    ITEM_X_SPEED,
    ITEM_X_SPECIAL,
    ITEM_X_ACCURACY,
    ITEM_GUARD_SPEC,
    ITEM_DIRE_HIT,
    ITEM_NONE
};

// Item descriptions and data
#include "data/items.h"

u16 GetBagItemQuantity(u16 * ptr)
{
    return gSaveBlock2Ptr->encryptionKey ^ *ptr;
}

void SetBagItemQuantity(u16 * ptr, u16 value)
{
    *ptr = value ^ gSaveBlock2Ptr->encryptionKey;
}

u16 GetPcItemQuantity(u16 * ptr)
{
    return 0 ^ *ptr;
}

void SetPcItemQuantity(u16 * ptr, u16 value)
{
    *ptr = value ^ 0;
}

void ApplyNewEncryptionKeyToBagItems(u32 key)
{
    u32 i, j;

    for (i = 0; i < NUM_BAG_POCKETS; i++)
    {
        for (j = 0; j < gBagPockets[i].capacity; j++)
        {
            ApplyNewEncryptionKeyToHword(&gBagPockets[i].itemSlots[j].quantity, key);
        }
    }
}

void ApplyNewEncryptionKeyToBagItems_(u32 key)
{
    ApplyNewEncryptionKeyToBagItems(key);
}

void SetBagPocketsPointers(void)
{
    gBagPockets[POCKET_ITEMS - 1].itemSlots = gSaveBlock1Ptr->bagPocket_Items;
    gBagPockets[POCKET_ITEMS - 1].capacity = BAG_ITEMS_COUNT;
    gBagPockets[POCKET_KEY_ITEMS - 1].itemSlots = gSaveBlock1Ptr->bagPocket_KeyItems;
    gBagPockets[POCKET_KEY_ITEMS - 1].capacity = BAG_KEYITEMS_COUNT;
    gBagPockets[POCKET_POKE_BALLS - 1].itemSlots = gSaveBlock1Ptr->bagPocket_PokeBalls;
    gBagPockets[POCKET_POKE_BALLS - 1].capacity = BAG_POKEBALLS_COUNT;
    gBagPockets[POCKET_TM_CASE - 1].itemSlots = gSaveBlock1Ptr->bagPocket_TMHM;
    gBagPockets[POCKET_TM_CASE - 1].capacity = BAG_TMHM_COUNT;
    gBagPockets[POCKET_BERRY_POUCH - 1].itemSlots = gSaveBlock1Ptr->bagPocket_Berries;
    gBagPockets[POCKET_BERRY_POUCH - 1].capacity = BAG_BERRIES_COUNT;
}

void CopyItemName(u16 itemId, u8 * dest)
{
    if (itemId == ITEM_ENIGMA_BERRY)
    {
        StringCopy(dest, GetBerryInfo(ITEM_TO_BERRY(ITEM_ENIGMA_BERRY))->name);
        StringAppend(dest, gText_Berry);
    }
    else
    {
        StringCopy(dest, ItemId_GetName(itemId));
    }
}

void CopyUniqueItemName(u16 itemId, u8 * dest)
{
    StringCopy(dest, ItemId_GetUniqueName(itemId));
}

s8 BagPocketGetFirstEmptySlot(u8 pocketId)
{
    u16 i;

    for (i = 0; i < gBagPockets[pocketId].capacity; i++)
    {
        if (gBagPockets[pocketId].itemSlots[i].itemId == ITEM_NONE)
            return i;
    }

    return -1;
}

bool8 IsPocketNotEmpty(u8 pocketId)
{
    u8 i;

    for (i = 0; i < gBagPockets[pocketId - 1].capacity; i++)
    {
        if (gBagPockets[pocketId - 1].itemSlots[i].itemId != ITEM_NONE)
            return TRUE;
    }

    return FALSE;
}

bool8 CheckBagHasItem(u16 itemId, u16 count)
{
    u8 i;
    u8 pocket;

    if (ItemId_GetPocket(itemId) == 0)
        return FALSE;

    pocket = ItemId_GetPocket(itemId) - 1;
    // Check for item slots that contain the item
    for (i = 0; i < gBagPockets[pocket].capacity; i++)
    {
        if (gBagPockets[pocket].itemSlots[i].itemId == itemId)
        {
            u16 quantity;
            // Does this item slot contain enough of the item?
            quantity = GetBagItemQuantity(&gBagPockets[pocket].itemSlots[i].quantity);
            if (quantity >= count)
                return TRUE;
                // RS and Emerald check whether there is enough of the
                // item across all stacks.
                // For whatever reason, FR/LG assume there's only one
                // stack of the item.
            else
                return FALSE;
        }
    }
    return FALSE;
}

bool8 HasAtLeastOneBerry(void)
{
    u8 itemId;
    bool8 exists;

    exists = CheckBagHasItem(ITEM_BERRY_POUCH, 1);
    if (!exists)
    {
        gSpecialVar_Result = FALSE;
        return FALSE;
    }
    for (itemId = FIRST_BERRY_INDEX; itemId <= LAST_BERRY_INDEX; itemId++)
    {
        exists = CheckBagHasItem(itemId, 1);
        if (exists)
        {
            gSpecialVar_Result = TRUE;
            return TRUE;
        }
    }

    gSpecialVar_Result = FALSE;
    return FALSE;
}

bool8 CheckBagHasSpace(u16 itemId, u16 count)
{
    u8 i;
    u8 pocket;

    if (ItemId_GetPocket(itemId) == 0)
        return FALSE;

    pocket = ItemId_GetPocket(itemId) - 1;
    // Check for item slots that contain the item
    for (i = 0; i < gBagPockets[pocket].capacity; i++)
    {
        if (gBagPockets[pocket].itemSlots[i].itemId == itemId)
        {
            u16 quantity;
            // Does this stack have room for more??
            quantity = GetBagItemQuantity(&gBagPockets[pocket].itemSlots[i].quantity);
            if (quantity + count <= 999)
                return TRUE;
            // RS and Emerald check whether there is enough of the
            // item across all stacks.
            // For whatever reason, FR/LG assume there's only one
            // stack of the item.
            else
                return FALSE;
        }
    }

    if (BagPocketGetFirstEmptySlot(pocket) != -1)
        return TRUE;

    return FALSE;
}

bool8 AddBagItem(u16 itemId, u16 count)
{
    u8 i;
    u8 pocket;
    s8 idx;

    if (itemId == ITEM_ARCHIPELAGO_PROGRESSION)
        return TRUE;

    if (IsItemUnique(itemId))
        return AddUniqueBagItem(itemId, count);

    if (ItemId_GetPocket(itemId) == 0)
        return FALSE;

    pocket = ItemId_GetPocket(itemId) - 1;
    for (i = 0; i < gBagPockets[pocket].capacity; i++)
    {
        if (gBagPockets[pocket].itemSlots[i].itemId == itemId)
        {
            u16 quantity;
            // Does this stack have room for more??
            quantity = GetBagItemQuantity(&gBagPockets[pocket].itemSlots[i].quantity);
            if (quantity + count <= 999)
            {
                quantity += count;
                SetBagItemQuantity(&gBagPockets[pocket].itemSlots[i].quantity, quantity);
                return TRUE;
            }
            // RS and Emerald check whether there is enough of the
            // item across all stacks.
            // For whatever reason, FR/LG assume there's only one
            // stack of the item.
            else
                return FALSE;
        }
    }

    if (itemId == ITEM_BERRY_POUCH)
        FlagSet(FLAG_SYS_GOT_BERRY_POUCH);

    if (!gArchipelagoOptions.isShopsanity)
    {
        if (itemId == ITEM_FIRE_STONE)
            gSaveBlock2Ptr->shopItemFlags[SHOP_CELADON_CITY_DEPT_EVO - SHOPSANITY_FIRST_INDEX] |= (1 << 2);
        else if (itemId == ITEM_THUNDER_STONE)
            gSaveBlock2Ptr->shopItemFlags[SHOP_CELADON_CITY_DEPT_EVO - SHOPSANITY_FIRST_INDEX] |= (1 << 3);
        else if (itemId == ITEM_WATER_STONE)
            gSaveBlock2Ptr->shopItemFlags[SHOP_CELADON_CITY_DEPT_EVO - SHOPSANITY_FIRST_INDEX] |= (1 << 4);
        else if (itemId == ITEM_LEAF_STONE)
            gSaveBlock2Ptr->shopItemFlags[SHOP_CELADON_CITY_DEPT_EVO - SHOPSANITY_FIRST_INDEX] |= (1 << 5);
    }

    if (itemId == ITEM_TOWN_MAP)
        SetFlyMapFlag(gArchipelagoOptions.townFreeFlyId);

    idx = BagPocketGetFirstEmptySlot(pocket);
    if (idx == -1)
        return FALSE;

    gBagPockets[pocket].itemSlots[idx].itemId = itemId;
    SetBagItemQuantity(&gBagPockets[pocket].itemSlots[idx].quantity, count);
    SetPokemonCenterShopFlag(itemId);
    return TRUE;
}

bool8 AddUniqueBagItem(u16 itemId, u16 count)
{
    switch (itemId)
    {
    case ITEM_BADGE_1:
        FlagSet(FLAG_BADGE01_GET);
        break;
    case ITEM_BADGE_2:
        FlagSet(FLAG_BADGE02_GET);
        break;
    case ITEM_BADGE_3:
        FlagSet(FLAG_BADGE03_GET);
        break;
    case ITEM_BADGE_4:
        FlagSet(FLAG_BADGE04_GET);
        break;
    case ITEM_BADGE_5:
        FlagSet(FLAG_BADGE05_GET);
        break;
    case ITEM_BADGE_6:
        FlagSet(FLAG_BADGE06_GET);
        break;
    case ITEM_BADGE_7:
        FlagSet(FLAG_BADGE07_GET);
        break;
    case ITEM_BADGE_8:
        FlagSet(FLAG_BADGE08_GET);
        break;
    case ITEM_FLY_PALLET:
        MapPreview_SetFlag(FLAG_WORLD_MAP_PALLET_TOWN);
        break;
    case ITEM_FLY_VIRIDIAN:
        MapPreview_SetFlag(FLAG_WORLD_MAP_VIRIDIAN_CITY);
        break;
    case ITEM_FLY_PEWTER:
        MapPreview_SetFlag(FLAG_WORLD_MAP_PEWTER_CITY);
        break;
    case ITEM_FLY_CERULEAN:
        MapPreview_SetFlag(FLAG_WORLD_MAP_CERULEAN_CITY);
        break;
    case ITEM_FLY_LAVENDER:
        MapPreview_SetFlag(FLAG_WORLD_MAP_LAVENDER_TOWN);
        break;
    case ITEM_FLY_VERMILION:
        MapPreview_SetFlag(FLAG_WORLD_MAP_VERMILION_CITY);
        break;
    case ITEM_FLY_CELADON:
        MapPreview_SetFlag(FLAG_WORLD_MAP_CELADON_CITY);
        break;
    case ITEM_FLY_FUCHSIA:
        MapPreview_SetFlag(FLAG_WORLD_MAP_FUCHSIA_CITY);
        break;
    case ITEM_FLY_CINNABAR:
        MapPreview_SetFlag(FLAG_WORLD_MAP_CINNABAR_ISLAND);
        break;
    case ITEM_FLY_INDIGO:
        MapPreview_SetFlag(FLAG_WORLD_MAP_INDIGO_PLATEAU_EXTERIOR);
        break;
    case ITEM_FLY_SAFFRON:
        MapPreview_SetFlag(FLAG_WORLD_MAP_SAFFRON_CITY);
        break;
    case ITEM_FLY_ONE_ISLAND:
        MapPreview_SetFlag(FLAG_WORLD_MAP_ONE_ISLAND);
        break;
    case ITEM_FLY_TWO_ISLAND:
        MapPreview_SetFlag(FLAG_WORLD_MAP_TWO_ISLAND);
        break;
    case ITEM_FLY_THREE_ISLAND:
        MapPreview_SetFlag(FLAG_WORLD_MAP_THREE_ISLAND);
        break;
    case ITEM_FLY_FOUR_ISLAND:
        MapPreview_SetFlag(FLAG_WORLD_MAP_FOUR_ISLAND);
        break;
    case ITEM_FLY_FIVE_ISLAND:
        MapPreview_SetFlag(FLAG_WORLD_MAP_FIVE_ISLAND);
        break;
    case ITEM_FLY_SIX_ISLAND:
        MapPreview_SetFlag(FLAG_WORLD_MAP_SIX_ISLAND);
        break;
    case ITEM_FLY_SEVEN_ISLAND:
        MapPreview_SetFlag(FLAG_WORLD_MAP_SEVEN_ISLAND);
        break;
    case ITEM_FLY_ROUTE4:
        MapPreview_SetFlag(FLAG_WORLD_MAP_ROUTE4_POKEMON_CENTER_1F);
        break;
    case ITEM_FLY_ROUTE10:
        MapPreview_SetFlag(FLAG_WORLD_MAP_ROUTE10_POKEMON_CENTER_1F);
        break;
    case ITEM_COINS_10:
        AddCoins(10);
        break;
    case ITEM_COINS_20:
        AddCoins(20);
        break;
    case ITEM_COINS_40:
        AddCoins(40);
        break;
    case ITEM_COINS_100:
        AddCoins(100);
        break;
    case ITEM_PROG_PASS:
        AddProgressivePass(count);
        break;
    case ITEM_PROG_CARD_KEY:
        AddProgressiveCardKey(count);
        break;
    case ITEM_PROG_ROD:
    	AddProgressiveRod(count);
    	break;
    case ITEM_RUNNING_SHOES:
        FlagSet(FLAG_SYS_B_DASH);
        break;
    case ITEM_JUMPING_SHOES:
        FlagSet(FLAG_SYS_LEDGE_JUMP);
        break;
    case ITEM_POKEDEX:
        FlagSet(FLAG_SYS_POKEDEX_GET);
        break;
    default:
        break;
    }

    if (!gArchipelagoOptions.gymKeys &&
        HasRequiredBadgesOrGyms(gArchipelagoOptions.giovanniRequiresGyms, gArchipelagoOptions.giovanniRequiredCount) &&
        VarGet(VAR_MAP_SCENE_VIRIDIAN_CITY_GYM_DOOR) == 0)
        VarSet(VAR_MAP_SCENE_VIRIDIAN_CITY_GYM_DOOR, 1);

    return TRUE;
}

void AddProgressivePass(u16 count)
{
    while (count > 0)
    {
        if (gArchipelagoOptions.passesSplit)
        {
            switch (gSaveBlock2Ptr->progressivePassesCount)
            {
            case 0:
                AddBagItem(ITEM_ONE_PASS, 1);
                break;
            case 1:
                AddBagItem(ITEM_TWO_PASS, 1);
                break;
            case 2:
                AddBagItem(ITEM_THREE_PASS, 1);
                break;
            case 3:
                AddBagItem(ITEM_FOUR_PASS, 1);
                break;
            case 4:
                AddBagItem(ITEM_FIVE_PASS, 1);
                break;
            case 5:
                AddBagItem(ITEM_SIX_PASS, 1);
                break;
            case 6:
                AddBagItem(ITEM_SEVEN_PASS, 1);
                break;
            default:
                return;
            }
        }
        else
        {
            switch (gSaveBlock2Ptr->progressivePassesCount)
            {
            case 0:
                AddBagItem(ITEM_TRI_PASS, 1);
                break;
            case 1:
                AddBagItem(ITEM_RAINBOW_PASS, 1);
                break;
            default:
                return;
            }
        }

        gSaveBlock2Ptr->progressivePassesCount++;
        count--;
    }
}

void AddProgressiveCardKey(u16 count)
{
    while (count > 0)
    {
        switch (gSaveBlock2Ptr->progressiveCardKeyCount)
        {
        case 0:
            AddBagItem(ITEM_CARD_KEY_2F, 1);
            break;
        case 1:
            AddBagItem(ITEM_CARD_KEY_3F, 1);
            break;
        case 2:
            AddBagItem(ITEM_CARD_KEY_4F, 1);
            break;
        case 3:
            AddBagItem(ITEM_CARD_KEY_5F, 1);
            break;
        case 4:
            AddBagItem(ITEM_CARD_KEY_6F, 1);
            break;
        case 5:
            AddBagItem(ITEM_CARD_KEY_7F, 1);
            break;
        case 6:
            AddBagItem(ITEM_CARD_KEY_8F, 1);
            break;
        case 7:
            AddBagItem(ITEM_CARD_KEY_9F, 1);
            break;
        case 8:
            AddBagItem(ITEM_CARD_KEY_10F, 1);
            break;
        case 9:
            AddBagItem(ITEM_CARD_KEY_11F, 1);
            break;
        default:
            return;
        }

        gSaveBlock2Ptr->progressiveCardKeyCount++;
        count--;
    }
}

void AddProgressiveRod(u16 count)
{
    while (count > 0)
    {
        switch (gSaveBlock2Ptr->progressiveRod)
        {
        case 0:
            AddBagItem(ITEM_OLD_ROD, 1);
            break;
        case 1:
            AddBagItem(ITEM_GOOD_ROD, 1);
            break;
        case 2:
            AddBagItem(ITEM_SUPER_ROD, 1);
            break;
        default:
            return;
        }

        gSaveBlock2Ptr->progressiveRod++;
        count--;
    }
}

void SetPokemonCenterShopStartingFlags()
{
    u8 i = 0;

    if (FlagGet(FLAG_BETTER_SHOPS_ENABLED))
    {
        while(gPokemonCenterShopItems[i] != ITEM_NONE)
        {
            gSaveBlock2Ptr->centerShopItemFlags |= (1 << i);
            i++;
        }
    }
    else
    {
        gSaveBlock2Ptr->centerShopItemFlags |= (1 << 0);
        gSaveBlock2Ptr->centerShopItemFlags |= (1 << 3);
        gSaveBlock2Ptr->centerShopItemFlags |= (1 << 9);
        gSaveBlock2Ptr->centerShopItemFlags |= (1 << 10);
    }
}

void SetPokemonCenterShopFlag(u16 itemId)
{
    u8 i = 0;

    while(gPokemonCenterShopItems[i] != ITEM_NONE)
    {
        if (gPokemonCenterShopItems[i] == itemId)
        {
            gSaveBlock2Ptr->centerShopItemFlags |= (1 << i);
            break;
        }
        i++;
    }
}

bool8 RemoveBagItem(u16 itemId, u16 count)
{
    u8 i;
    u8 pocket;

    if (ItemId_GetPocket(itemId) == 0)
        return FALSE;

    if (itemId == ITEM_NONE)
        return FALSE;

    pocket = ItemId_GetPocket(itemId) - 1;
    // Check for item slots that contain the item
    for (i = 0; i < gBagPockets[pocket].capacity; i++)
    {
        if (gBagPockets[pocket].itemSlots[i].itemId == itemId)
        {
            u16 quantity;
            // Does this item slot contain enough of the item?
            quantity = GetBagItemQuantity(&gBagPockets[pocket].itemSlots[i].quantity);
            if (quantity >= count)
            {
                quantity -= count;
                SetBagItemQuantity(&gBagPockets[pocket].itemSlots[i].quantity, quantity);
                if (quantity == 0)
                    gBagPockets[pocket].itemSlots[i].itemId = ITEM_NONE;
                return TRUE;
            }
            // RS and Emerald check whether there is enough of the
            // item across all stacks.
            // For whatever reason, FR/LG assume there's only one
            // stack of the item.
            else
                return FALSE;
        }
    }
    return FALSE;
}

u8 GetPocketByItemId(u16 itemId)
{
    return ItemId_GetPocket(itemId); // wow such important
}

void ClearItemSlots(struct ItemSlot * slots, u8 capacity)
{
    u16 i;

    for (i = 0; i < capacity; i++)
    {
        slots[i].itemId = ITEM_NONE;
        SetBagItemQuantity(&slots[i].quantity, 0);
    }
}

void ClearPCItemSlots(void)
{
    u16 i;

    for (i = 0; i < PC_ITEMS_COUNT; i++)
    {
        gSaveBlock1Ptr->pcItems[i].itemId = ITEM_NONE;
        SetPcItemQuantity(&gSaveBlock1Ptr->pcItems[i].quantity, 0);
    }
}

void ClearBag(void)
{
    u16 i;

    for (i = 0; i < NUM_BAG_POCKETS; i++)
    {
        ClearItemSlots(gBagPockets[i].itemSlots, gBagPockets[i].capacity);
    }
}

s8 PCItemsGetFirstEmptySlot(void)
{
    s8 i;

    for (i = 0; i < PC_ITEMS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pcItems[i].itemId == ITEM_NONE)
            return i;
    }

    return -1;
}

u8 CountItemsInPC(void)
{
    u8 count = 0;
    u8 i;

    for (i = 0; i < PC_ITEMS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pcItems[i].itemId != ITEM_NONE)
            count++;
    }

    return count;
}

bool8 CheckPCHasItem(u16 itemId, u16 count)
{
    u8 i;
    u16 quantity;

    for (i = 0; i < PC_ITEMS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pcItems[i].itemId == itemId)
        {
            quantity = GetPcItemQuantity(&gSaveBlock1Ptr->pcItems[i].quantity);
            if (quantity >= count)
                return TRUE;
        }
    }

    return FALSE;
}

bool8 AddPCItem(u16 itemId, u16 count)
{
    u8 i;
    u16 quantity;
    s8 idx;

    for (i = 0; i < PC_ITEMS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pcItems[i].itemId == itemId)
        {
            quantity = GetPcItemQuantity(&gSaveBlock1Ptr->pcItems[i].quantity);
            if (quantity + count <= 999)
            {
                quantity += count;
                SetPcItemQuantity(&gSaveBlock1Ptr->pcItems[i].quantity, quantity);
                return TRUE;
            }
            else
                return FALSE;
        }
    }

    idx = PCItemsGetFirstEmptySlot();
    if (idx == -1)
        return FALSE;

    gSaveBlock1Ptr->pcItems[idx].itemId = itemId;
    SetPcItemQuantity(&gSaveBlock1Ptr->pcItems[idx].quantity, count);
    return TRUE;
}

void RemovePCItem(u16 itemId, u16 count)
{
    u32 i;
    u16 quantity;

    if (itemId == ITEM_NONE)
        return;

    for (i = 0; i < PC_ITEMS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->pcItems[i].itemId == itemId)
            break;
    }

    if (i != PC_ITEMS_COUNT)
    {
        quantity = GetPcItemQuantity(&gSaveBlock1Ptr->pcItems[i].quantity) - count;
        SetPcItemQuantity(&gSaveBlock1Ptr->pcItems[i].quantity, quantity);
        if (quantity == 0)
            gSaveBlock1Ptr->pcItems[i].itemId = ITEM_NONE;
    }
}

void ItemPcCompaction(void)
{
    u16 i, j;
    struct ItemSlot tmp;

    for (i = 0; i < PC_ITEMS_COUNT - 1; i++)
    {
        for (j = i + 1; j < PC_ITEMS_COUNT; j++)
        {
            if (gSaveBlock1Ptr->pcItems[i].itemId == ITEM_NONE)
            {
                tmp = gSaveBlock1Ptr->pcItems[i];
                gSaveBlock1Ptr->pcItems[i] = gSaveBlock1Ptr->pcItems[j];
                gSaveBlock1Ptr->pcItems[j] = tmp;
            }
        }
    }
}

void RegisteredItemHandleBikeSwap(void)
{
    switch (gSaveBlock1Ptr->registeredItem)
    {
    case ITEM_MACH_BIKE:
        gSaveBlock1Ptr->registeredItem = ITEM_ACRO_BIKE;
        break;
    case ITEM_ACRO_BIKE:
        gSaveBlock1Ptr->registeredItem = ITEM_MACH_BIKE;
        break;
    }
}

void SwapItemSlots(struct ItemSlot * a, struct ItemSlot * b)
{
    struct ItemSlot c;
    c = *a;
    *a = *b;
    *b = c;
}

void BagPocketCompaction(struct ItemSlot * slots, u8 capacity)
{
    u16 i, j;

    for (i = 0; i < capacity - 1; i++)
    {
        for (j = i + 1; j < capacity; j++)
        {
            if (GetBagItemQuantity(&slots[i].quantity) == 0)
            {
                SwapItemSlots(&slots[i], &slots[j]);
            }
        }
    }
}

void SortPocketAndPlaceHMsFirst(struct BagPocket * pocket)
{
    u16 i;
    u16 j = 0;
    u16 k;
    struct ItemSlot * buff;

    SortAndCompactBagPocket(pocket);

    for (i = 0; i < pocket->capacity; i++)
    {
        if (pocket->itemSlots[i].itemId == ITEM_NONE && GetBagItemQuantity(&pocket->itemSlots[i].quantity) == 0)
            return;
        if (pocket->itemSlots[i].itemId >= ITEM_HM01 && GetBagItemQuantity(&pocket->itemSlots[i].quantity) != 0)
        {
            for (j = i + 1; j < pocket->capacity; j++)
            {
                if (pocket->itemSlots[j].itemId == ITEM_NONE && GetBagItemQuantity(&pocket->itemSlots[j].quantity) == 0)
                    break;
            }
            break;
        }
    }

    for (k = 0; k < pocket->capacity; k++)
        pocket->itemSlots[k].quantity = GetBagItemQuantity(&pocket->itemSlots[k].quantity);
    buff = AllocZeroed(pocket->capacity * sizeof(struct ItemSlot));
    CpuCopy16(pocket->itemSlots + i, buff, (j - i) * sizeof(struct ItemSlot));
    CpuCopy16(pocket->itemSlots, buff + (j - i), i * sizeof(struct ItemSlot));
    CpuCopy16(buff, pocket->itemSlots, pocket->capacity * sizeof(struct ItemSlot));
    for (k = 0; k < pocket->capacity; k++)
        SetBagItemQuantity(&pocket->itemSlots[k].quantity, pocket->itemSlots[k].quantity);
    Free(buff);
}

void SortAndCompactBagPocket(struct BagPocket * pocket)
{
    u16 i, j;

    for (i = 0; i < pocket->capacity; i++)
    {
        for (j = i + 1; j < pocket->capacity; j++)
        {
            if (GetBagItemQuantity(&pocket->itemSlots[i].quantity) == 0 || (GetBagItemQuantity(&pocket->itemSlots[j].quantity) != 0 && pocket->itemSlots[i].itemId > pocket->itemSlots[j].itemId))
                SwapItemSlots(&pocket->itemSlots[i], &pocket->itemSlots[j]);
        }
    }
}

u16 BagGetItemIdByPocketPosition(u8 pocketId, u16 slotId)
{
    return gBagPockets[pocketId - 1].itemSlots[slotId].itemId;
}

u16 BagGetQuantityByPocketPosition(u8 pocketId, u16 slotId)
{
    return GetBagItemQuantity(&gBagPockets[pocketId - 1].itemSlots[slotId].quantity);
}

u16 BagGetQuantityByItemId(u16 itemId)
{
    u16 i;
    struct BagPocket * pocket = &gBagPockets[ItemId_GetPocket(itemId) - 1];

    for (i = 0; i < pocket->capacity; i++)
    {
        if (pocket->itemSlots[i].itemId == itemId)
            return GetBagItemQuantity(&pocket->itemSlots[i].quantity);
    }

    return 0;
}

void TrySetObtainedItemQuestLogEvent(u16 itemId)
{
    // Only some key items trigger this event
    if (itemId == ITEM_OAKS_PARCEL
     || itemId == ITEM_POKE_FLUTE
     || itemId == ITEM_SECRET_KEY
     || itemId == ITEM_BIKE_VOUCHER
     || itemId == ITEM_GOLD_TEETH
     || itemId == ITEM_OLD_AMBER
     || itemId == ITEM_CARD_KEY
     || itemId == ITEM_LIFT_KEY
     || itemId == ITEM_HELIX_FOSSIL
     || itemId == ITEM_DOME_FOSSIL
     || itemId == ITEM_SILPH_SCOPE
     || itemId == ITEM_BICYCLE
     || itemId == ITEM_TOWN_MAP
     || itemId == ITEM_VS_SEEKER
     || itemId == ITEM_TEACHY_TV
     || itemId == ITEM_RAINBOW_PASS
     || itemId == ITEM_TEA
     || itemId == ITEM_POWDER_JAR
     || itemId == ITEM_RUBY
     || itemId == ITEM_SAPPHIRE)
    {
        if (itemId != ITEM_TOWN_MAP || (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(PALLET_TOWN_RIVALS_HOUSE) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(PALLET_TOWN_RIVALS_HOUSE)))
        {
            struct QuestLogEvent_StoryItem * data = malloc(sizeof(*data));
            data->itemId = itemId;
            data->mapSec = gMapHeader.regionMapSectionId;
            SetQuestLogEvent(QL_EVENT_OBTAINED_STORY_ITEM, (const u16 *)data);
            free(data);
        }
    }
}

u16 SanitizeItemId(u16 itemId)
{
    if (itemId >= ITEMS_COUNT)
        return ITEM_NONE;
    return itemId;
}

const u8 * ItemId_GetName(u16 itemId)
{
    if (itemId == ITEM_PROG_CARD_KEY)
    {
        if (gSaveBlock2Ptr->progressiveCardKeyCount < NUM_CARD_KEYS)
            return gItems[ITEM_CARD_KEY_2F + gSaveBlock2Ptr->progressiveCardKeyCount].name;
        else
            return gItems[ITEM_CARD_KEY_11F].name;
    }
    else if (itemId == ITEM_PROG_PASS)
    {
        if (gArchipelagoOptions.passesSplit)
        {
            if (gSaveBlock2Ptr->progressivePassesCount < NUM_SPLIT_PASSES)
                return gItems[ITEM_ONE_PASS + gSaveBlock2Ptr->progressivePassesCount].name;
            else
                return gItems[ITEM_SEVEN_PASS].name;
        }
        else
        {
            if (gSaveBlock2Ptr->progressivePassesCount < NUM_PASSES)
                return gItems[ITEM_TRI_PASS + gSaveBlock2Ptr->progressivePassesCount].name;
            else
                return gItems[ITEM_RAINBOW_PASS].name;
        }
    }
    else if (itemId == ITEM_PROG_ROD)
    {
        if (gSaveBlock2Ptr->progressiveRod < NUM_RODS)
            return gItems[ITEM_OLD_ROD + gSaveBlock2Ptr->progressiveRod].name;
        else
            return gItems[ITEM_SUPER_ROD].name;
    }

    return gItems[SanitizeItemId(itemId)].name;
}

const u8 * ItemId_GetUniqueName(u16 itemId)
{
    if (itemId >= ITEM_FLY_PALLET && itemId <= ITEM_FLY_SIX_ISLAND)
        return gFlyUnlockNames[itemId - ITEM_FLY_PALLET];

    return ItemId_GetName(itemId);
}

// Unused
u16 ItemId_GetId(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].itemId;
}

u16 ItemId_GetPrice(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].price;
}

u8 ItemId_GetHoldEffect(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].holdEffect;
}

u8 ItemId_GetHoldEffectParam(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].holdEffectParam;
}

const u8 * ItemId_GetAPItemDescription(u16 locationId)
{
    u16 i;
    u16 itemNameOffset;
    u8 playerNameId;
    u8 newline[] = _("'s\n");
    u8 period[] = _(".");

    for (i = 0; i < NAME_TABLE_BUFFER_SIZE / 6; i++)
    {
        if ((gArchipelagoNameTable[(i * 6) + 0] | (gArchipelagoNameTable[(i * 6) + 1] << 8)) == locationId)
        {
            playerNameId = gArchipelagoNameTable[(i * 6) + 4] | (gArchipelagoNameTable[(i * 6) + 5] << 8);
            itemNameOffset = gArchipelagoNameTable[(i * 6) + 2] | (gArchipelagoNameTable[(i * 6) + 3] << 8);
            if (playerNameId == 0)
            {
            	StringCopy(gStringVar5, gText_APItemDescriptionSelf);
            }
            else
            {
                StringCopy(gStringVar5, gText_APItemDescriptionOther);
                StringAppend(gStringVar5, gArchipelagoPlayerNames + (playerNameId * 17));
                StringAppend(gStringVar5, newline);
            }
            StringAppend(gStringVar5, gArchipelagoItemNames + itemNameOffset);
            StringAppend(gStringVar5, period);
            return gStringVar5;
        }
        else if ((gArchipelagoNameTable[(i * 6) + 0] | (gArchipelagoNameTable[(i * 6) + 1] << 8)) > locationId)
        {
            break;
        }
    }

    return gItems[ITEM_ARCHIPELAGO_PROGRESSION].description;
}

const u8 * ItemId_GetDescription(u16 itemId)
{
    if (itemId >= ITEM_FLY_PALLET && itemId <= ITEM_FLY_SIX_ISLAND)
        return ItemId_GetFlyDescription(itemId);
    else if (itemId == ITEM_PROG_CARD_KEY)
    {
        if (gSaveBlock2Ptr->progressiveCardKeyCount < NUM_CARD_KEYS)
            return gItems[ITEM_CARD_KEY_2F + gSaveBlock2Ptr->progressiveCardKeyCount].description;
        else
            return gItems[ITEM_CARD_KEY_11F].description;
    }
    else if (itemId == ITEM_PROG_PASS)
    {
        if (gArchipelagoOptions.passesSplit)
        {
            if (gSaveBlock2Ptr->progressivePassesCount < NUM_SPLIT_PASSES)
                return gItems[ITEM_ONE_PASS + gSaveBlock2Ptr->progressivePassesCount].description;
            else
                return gItems[ITEM_SEVEN_PASS].description;
        }
        else
        {
            if (gSaveBlock2Ptr->progressivePassesCount < NUM_PASSES)
                return gItems[ITEM_TRI_PASS + gSaveBlock2Ptr->progressivePassesCount].description;
            else
                return gItems[ITEM_RAINBOW_PASS].description;
        }
    }
    else if (itemId == ITEM_PROG_ROD)
    {
        if (gSaveBlock2Ptr->progressiveRod < NUM_RODS)
            return gItems[ITEM_OLD_ROD + gSaveBlock2Ptr->progressiveRod].description;
        else
            return gItems[ITEM_SUPER_ROD].description;
    }
    return gItems[SanitizeItemId(itemId)].description;
}

const u8 * ItemId_GetFlyDescription(u16 itemId)
{
    u8 period[] = _(".");
    StringCopy(gStringVar5, gText_FlyItemDescription);
    StringAppend(gStringVar5, gFlyUnlockNames[itemId - ITEM_FLY_PALLET]);
    StringAppend(gStringVar5, period);
    return gStringVar5;
}

u8 ItemId_GetImportance(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].importance;
}

// Unused
u8 ItemId_GetRegistrability(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].registrability;
}

u8 ItemId_GetPocket(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].pocket;
}

u8 ItemId_GetType(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].type;
}

ItemUseFunc ItemId_GetFieldFunc(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].fieldUseFunc;
}

bool8 ItemId_GetBattleUsage(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].battleUsage;
}

ItemUseFunc ItemId_GetBattleFunc(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].battleUseFunc;
}

u8 ItemId_GetSecondaryId(u16 itemId)
{
    return gItems[SanitizeItemId(itemId)].secondaryId;
}

void ItemId_GetHoldEffectParam_Script(void)
{
    VarSet(VAR_RESULT, ItemId_GetHoldEffectParam(VarGet(VAR_0x8004)));
}
