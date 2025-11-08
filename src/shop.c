#include "global.h"
#include "gflib.h"
#include "archipelago.h"
#include "shop.h"
#include "event_data.h"
#include "menu.h"
#include "data.h"
#include "graphics.h"
#include "strings.h"
#include "list_menu.h"
#include "new_menu_helpers.h"
#include "party_menu.h"
#include "field_specials.h"
#include "field_weather.h"
#include "task.h"
#include "item.h"
#include "item_menu.h"
#include "overworld.h"
#include "field_fadetransition.h"
#include "scanline_effect.h"
#include "item_menu_icons.h"
#include "decompress.h"
#include "menu_indicators.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "event_object_movement.h"
#include "money.h"
#include "coins.h"
#include "quest_log.h"
#include "script.h"
#include "constants/flags.h"
#include "constants/shops.h"
#include "constants/songs.h"
#include "constants/items.h"
#include "constants/game_stat.h"
#include "constants/field_weather.h"

#define tItemCount data[1]
#define tItemId data[5]
#define tListTaskId data[7]

// mart types
enum
{
    MART_TYPE_REGULAR = 0,
    MART_TYPE_COIN,
    MART_TYPE_TMHM,
    MART_TYPE_TMHM_COIN,
    MART_TYPE_DECOR,
    MART_TYPE_DECOR2,
};

// shop view window NPC info enum
enum
{
    OBJECT_EVENT_ID,
    X_COORD,
    Y_COORD,
    ANIM_NUM
};

enum ItemBoughtFlags
{
    FLAG_GET_BOUGHT,
    FLAG_SET_BOUGHT,
};

struct ShopItem
{
    u16 item;
    u16 repeatableItem;
    u16 price;
    bool8 repeatable;
};

struct ShopData
{
    /*0x00*/ void (*callback)(void);
    /*0x04*/ const struct ShopItem *itemList;
    /*0x08*/ u32 itemPrice;
    /*0x0C*/ u16 selectedRow;
    /*0x0E*/ u16 scrollOffset;
    /*0x10*/ u16 itemCount;
    /*0x12*/ u16 itemsShowed;
    /*0x14*/ u16 maxQuantity;
    /*0x16*/ u16 martType:4;    // 0x1 if tm list
             u16 fontId:5;
             u16 itemSlot:2;
             u16 scrollArrows:5;
    /*0x18*/ u16 scrollArrowsOffset;
    /*0x20*/ u8 shopId;
};

static EWRAM_DATA s16 sViewportObjectEvents[OBJECT_EVENTS_COUNT][4] = {0};
static EWRAM_DATA struct ShopData sShopData = {0};
static EWRAM_DATA u8 sShopMenuWindowId = 0;
EWRAM_DATA u16 (*gShopTilemapBuffer1)[0x400] = {0};
EWRAM_DATA u16 (*gShopTilemapBuffer2)[0x400] = {0};
EWRAM_DATA u16 (*gShopTilemapBuffer3)[0x400] = {0};
EWRAM_DATA u16 (*gShopTilemapBuffer4)[0x400] = {0};
EWRAM_DATA struct ListMenuItem *sShopMenuListMenu = {0};
static EWRAM_DATA u8 (*sShopMenuItemStrings)[14] = {0};
EWRAM_DATA struct QuestLogEvent_Shop sHistory[2] = {0};

//Function Declarations
static u8 CreateShopMenu(u8 martType);
static u8 GetMartTypeFromItemList(u32 a0);
static void SetShopItemsForSale(u8 shopId);
static void SetShopMenuCallback(MainCallback callback);
static void Task_ShopMenu(u8 taskId);
static void Task_HandleShopMenuBuy(u8 taskId);
static void Task_HandleShopMenuSell(u8 taskId);
static void CB2_GoToSellMenu(void);
static void Task_HandleShopMenuQuit(u8 taskId);
static void ClearShopMenuWindow(void);
static void Task_GoToBuyOrSellMenu(u8 taskId);
static void MapPostLoadHook_ReturnToShopMenu(void);
static void Task_ReturnToShopMenu(u8 taskId);
static void ShowShopMenuAfterExitingBuyOrSellMenu(u8 taskId);
static void VBlankCB_BuyMenu(void);
static void CB2_InitBuyMenu(void);
static bool8 InitShopData(void);
static void BuyMenuInitBgs(void);
static void BuyMenuDecompressBgGraphics(void);
static void RecolorItemDescriptionBox(bool32 a0);
static void BuyMenuDrawGraphics(void);
static bool8 BuyMenuBuildListMenuTemplate(void);
static void PokeMartWriteNameAndIdAt(struct ListMenuItem *list, u16 index, u8 *dst);
static void BuyMenuPrintItemDescriptionAndShowItemIcon(s32 item, bool8 onInit, struct ListMenu *list, u16 index);
static void BuyMenuPrintPriceInList(u8 windowId, u32 item, u8 y, u16 index);
static void LoadTmHmNameInMart(s32 item);
static void BuyMenuPrintCursor(u8 listTaskId, u8 a1);
static void BuyMenuPrintCursorAtYPosition(u8 y, u8 a1);
static void BuyMenuFreeMemory(void);
static void SetShopExitCallback(void);
static void BuyMenuAddScrollIndicatorArrows(void);
static void BuyQuantityAddScrollIndicatorArrows(void);
static void BuyMenuRemoveScrollIndicatorArrows(void);
static void BuyMenuDrawMapView(void);
static void BuyMenuDrawMapBg(void);
static void BuyMenuDrawMapMetatile(s16 x, s16 y, const u16 *src, u8 metatileLayerType);
static void BuyMenuDrawMapMetatileLayer(u16 *dest, s16 offset1, s16 offset2, const u16 *src);
static void BuyMenuCollectObjectEventData(void);
static void BuyMenuDrawObjectEvents(void);
static void BuyMenuCopyTilemapData(void);
static void BuyMenuPrintItemQuantityAndPrice(u8 taskId);
static void Task_BuyMenu(u8 taskId);
static void Task_BuyHowManyDialogueInit(u8 taskId);
static void Task_BuyHowManyDialogueHandleInput(u8 taskId);
static void CreateBuyMenuConfirmPurchaseWindow(u8 taskId);
static void BuyMenuTryMakePurchase(u8 taskId);
static void BuyMenuSubtractMoney(u8 taskId);
static void Task_ReturnToItemListAfterItemPurchase(u8 taskId);
static void BuyMenuReturnToItemList(u8 taskId);
static void ExitBuyMenu(u8 taskId);
static void Task_ExitBuyMenu(u8 taskId);
static void DebugFunc_PrintPurchaseDetails(u8 taskId);
static void DebugFunc_PrintShopMenuHistoryBeforeClearMaybe(void);
static void RecordTransactionForQuestLog(void);

static const struct MenuAction sShopMenuActions_BuySellQuit[] =
{
    {gText_ShopBuy, {.void_u8 = Task_HandleShopMenuBuy}},
    {gText_ShopSell, {.void_u8 = Task_HandleShopMenuSell}},
    {gText_ShopQuit, {.void_u8 = Task_HandleShopMenuQuit}}
};

static const struct MenuAction sShopMenuActions_BuyQuit[] =
{
    {gText_ShopBuy, {.void_u8 = Task_HandleShopMenuBuy}},
    {gText_ShopLeave, {.void_u8 = Task_HandleShopMenuQuit}}
};

static const struct YesNoFuncTable sShopMenuActions_BuyReturn[] =
{
    BuyMenuTryMakePurchase,
    BuyMenuReturnToItemList
};

static const struct WindowTemplate sShopMenuWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 2,
    .tilemapTop = 1,
    .width = 12,
    .height = 6,
    .paletteNum = 15,
    .baseBlock = 8
};

static const struct WindowTemplate sShopMenuSmallWindowTemplate =
{
    .bg = 0,
    .tilemapLeft = 2,
    .tilemapTop = 1,
    .width = 12,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 8
};

static const struct BgTemplate sShopBuyMenuBgTemplates[4] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
    {
        .bg = 3,
        .charBaseIndex = 0,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    }
};

static const struct ShopItem sBetterShop[] = {
    {ITEM_POKE_BALL, ITEM_POKE_BALL, 200, TRUE},
    {ITEM_GREAT_BALL, ITEM_GREAT_BALL, 600, TRUE},
    {ITEM_ULTRA_BALL, ITEM_ULTRA_BALL, 1200, TRUE},
    {ITEM_POTION, ITEM_POTION, 300, TRUE},
    {ITEM_SUPER_POTION, ITEM_SUPER_POTION, 700, TRUE},
    {ITEM_HYPER_POTION, ITEM_HYPER_POTION, 1200, TRUE},
    {ITEM_MAX_POTION, ITEM_MAX_POTION, 2500, TRUE},
    {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE, 3000, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_ANTIDOTE, ITEM_ANTIDOTE, 100, TRUE},
    {ITEM_PARALYZE_HEAL, ITEM_PARALYZE_HEAL, 200, TRUE},
    {ITEM_AWAKENING, ITEM_AWAKENING, 250, TRUE},
    {ITEM_BURN_HEAL, ITEM_BURN_HEAL, 250, TRUE},
    {ITEM_ICE_HEAL, ITEM_ICE_HEAL, 250, TRUE},
    {ITEM_FULL_HEAL, ITEM_FULL_HEAL, 600, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_REPEL, ITEM_REPEL, 350, TRUE},
    {ITEM_SUPER_REPEL, ITEM_SUPER_REPEL, 500, TRUE},
    {ITEM_MAX_REPEL, ITEM_MAX_REPEL, 700, TRUE},
    {ITEM_X_ATTACK, ITEM_X_ATTACK, 500, TRUE},
    {ITEM_X_DEFEND, ITEM_X_DEFEND, 550, TRUE},
    {ITEM_X_SPEED, ITEM_X_SPEED, 350, TRUE},
    {ITEM_X_SPECIAL, ITEM_X_SPECIAL, 350, TRUE},
    {ITEM_X_ACCURACY, ITEM_X_ACCURACY, 950, TRUE},
    {ITEM_GUARD_SPEC, ITEM_GUARD_SPEC, 700, TRUE},
    {ITEM_DIRE_HIT, ITEM_DIRE_HIT, 650, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sViridianShop[] = {
    {ITEM_POKE_BALL, ITEM_POKE_BALL, 200, TRUE},
    {ITEM_POTION, ITEM_POTION, 300, TRUE},
    {ITEM_ANTIDOTE, ITEM_ANTIDOTE, 100, TRUE},
    {ITEM_PARALYZE_HEAL, ITEM_PARALYZE_HEAL, 200, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sPewterShop[] = {
    {ITEM_POKE_BALL, ITEM_POKE_BALL, 200, TRUE},
    {ITEM_POTION, ITEM_POTION, 300, TRUE},
    {ITEM_ANTIDOTE, ITEM_ANTIDOTE, 100, TRUE},
    {ITEM_PARALYZE_HEAL, ITEM_PARALYZE_HEAL, 200, TRUE},
    {ITEM_AWAKENING, ITEM_AWAKENING, 250, TRUE},
    {ITEM_BURN_HEAL, ITEM_BURN_HEAL, 250, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_REPEL, ITEM_REPEL, 350, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sCeruleanShop[] = {
    {ITEM_POKE_BALL, ITEM_POKE_BALL, 200, TRUE},
    {ITEM_POTION, ITEM_POTION, 300, TRUE},
    {ITEM_SUPER_POTION, ITEM_SUPER_POTION, 700, TRUE},
    {ITEM_ANTIDOTE, ITEM_ANTIDOTE, 100, TRUE},
    {ITEM_PARALYZE_HEAL, ITEM_PARALYZE_HEAL, 200, TRUE},
    {ITEM_AWAKENING, ITEM_AWAKENING, 250, TRUE},
    {ITEM_BURN_HEAL, ITEM_BURN_HEAL, 250, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_REPEL, ITEM_REPEL, 350, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sVermilionShop[] = {
    {ITEM_POKE_BALL, ITEM_POKE_BALL, 200, TRUE},
    {ITEM_SUPER_POTION, ITEM_SUPER_POTION, 700, TRUE},
    {ITEM_ANTIDOTE, ITEM_ANTIDOTE, 100, TRUE},
    {ITEM_PARALYZE_HEAL, ITEM_PARALYZE_HEAL, 200, TRUE},
    {ITEM_AWAKENING, ITEM_AWAKENING, 250, TRUE},
    {ITEM_ICE_HEAL, ITEM_ICE_HEAL, 250, TRUE},
    {ITEM_REPEL, ITEM_REPEL, 350, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sLavenderShop[] = {
    {ITEM_GREAT_BALL, ITEM_GREAT_BALL, 600, TRUE},
    {ITEM_SUPER_POTION, ITEM_SUPER_POTION, 700, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_ANTIDOTE, ITEM_ANTIDOTE, 100, TRUE},
    {ITEM_PARALYZE_HEAL, ITEM_PARALYZE_HEAL, 200, TRUE},
    {ITEM_BURN_HEAL, ITEM_BURN_HEAL, 250, TRUE},
    {ITEM_ICE_HEAL, ITEM_ICE_HEAL, 250, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_SUPER_REPEL, ITEM_SUPER_REPEL, 500, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sCeladonDeptItemShop[] = {
    {ITEM_GREAT_BALL, ITEM_GREAT_BALL, 600, TRUE},
    {ITEM_SUPER_POTION, ITEM_SUPER_POTION, 700, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_ANTIDOTE, ITEM_ANTIDOTE, 100, TRUE},
    {ITEM_PARALYZE_HEAL, ITEM_PARALYZE_HEAL, 200, TRUE},
    {ITEM_AWAKENING, ITEM_AWAKENING, 250, TRUE},
    {ITEM_BURN_HEAL, ITEM_BURN_HEAL, 250, TRUE},
    {ITEM_ICE_HEAL, ITEM_ICE_HEAL, 250, TRUE},
    {ITEM_SUPER_REPEL, ITEM_SUPER_REPEL, 500, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sCeladonDeptTMShop[] = {
    {ITEM_TM05, ITEM_TM05, 1000, TRUE},
    {ITEM_TM15, ITEM_TM15, 7500, TRUE},
    {ITEM_TM28, ITEM_TM28, 2000, TRUE},
    {ITEM_TM31, ITEM_TM31, 3000, TRUE},
    {ITEM_TM43, ITEM_TM43, 3000, TRUE},
    {ITEM_TM45, ITEM_TM45, 3000, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sCeladonDeptEvoShop[] = {
    {ITEM_POKE_DOLL, ITEM_POKE_DOLL, 1000, TRUE},
    {ITEM_RETRO_MAIL, ITEM_RETRO_MAIL, 50, TRUE},
    {ITEM_FIRE_STONE, ITEM_FIRE_STONE, 2100, FALSE},
    {ITEM_THUNDER_STONE, ITEM_THUNDER_STONE, 2100, FALSE},
    {ITEM_WATER_STONE, ITEM_WATER_STONE, 2100, FALSE},
    {ITEM_LEAF_STONE, ITEM_LEAF_STONE, 2100, FALSE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sCeladonDeptHeldShop[] = {
    {ITEM_KINGS_ROCK, ITEM_KINGS_ROCK, 2100, TRUE},
    {ITEM_METAL_COAT, ITEM_METAL_COAT, 2100, TRUE},
    {ITEM_DRAGON_SCALE, ITEM_DRAGON_SCALE, 2100, TRUE},
    {ITEM_UP_GRADE, ITEM_UP_GRADE, 2100, TRUE},
    {ITEM_DEEP_SEA_SCALE, ITEM_DEEP_SEA_SCALE, 2100, TRUE},
    {ITEM_DEEP_SEA_TOOTH, ITEM_DEEP_SEA_TOOTH, 2100, TRUE},
    {ITEM_HEART_SCALE, ITEM_HEART_SCALE, 2100, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sCeladonDeptBattleShop[] = {
    {ITEM_X_ATTACK, ITEM_X_ATTACK, 500, TRUE},
    {ITEM_X_DEFEND, ITEM_X_DEFEND, 550, TRUE},
    {ITEM_X_SPEED, ITEM_X_SPEED, 350, TRUE},
    {ITEM_X_SPECIAL, ITEM_X_SPECIAL, 350, TRUE},
    {ITEM_X_ACCURACY, ITEM_X_ACCURACY, 950, TRUE},
    {ITEM_GUARD_SPEC, ITEM_GUARD_SPEC, 700, TRUE},
    {ITEM_DIRE_HIT, ITEM_DIRE_HIT, 650, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sCeladonDeptVitaminShop[] = {
    {ITEM_HP_UP, ITEM_HP_UP, 9800, TRUE},
    {ITEM_PROTEIN, ITEM_PROTEIN, 9800, TRUE},
    {ITEM_IRON, ITEM_IRON, 9800, TRUE},
    {ITEM_CALCIUM, ITEM_CALCIUM, 9800, TRUE},
    {ITEM_ZINC, ITEM_ZINC, 9800, TRUE},
    {ITEM_CARBOS, ITEM_CARBOS, 9800, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sFuchsiaShop[] = {
    {ITEM_GREAT_BALL, ITEM_GREAT_BALL, 600, TRUE},
    {ITEM_ULTRA_BALL, ITEM_ULTRA_BALL, 1200, TRUE},
    {ITEM_SUPER_POTION, ITEM_SUPER_POTION, 700, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_FULL_HEAL, ITEM_FULL_HEAL, 600, TRUE},
    {ITEM_MAX_REPEL, ITEM_MAX_REPEL, 700, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sSaffronShop[] = {
    {ITEM_GREAT_BALL, ITEM_GREAT_BALL, 600, TRUE},
    {ITEM_HYPER_POTION, ITEM_HYPER_POTION, 1200, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_FULL_HEAL, ITEM_FULL_HEAL, 600, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_MAX_REPEL, ITEM_MAX_REPEL, 700, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sCinnabarShop[] = {
    {ITEM_GREAT_BALL, ITEM_GREAT_BALL, 600, TRUE},
    {ITEM_ULTRA_BALL, ITEM_ULTRA_BALL, 1200, TRUE},
    {ITEM_HYPER_POTION, ITEM_HYPER_POTION, 1200, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_FULL_HEAL, ITEM_FULL_HEAL, 600, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_MAX_REPEL, ITEM_MAX_REPEL, 700, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sIndigoShop[] = {
    {ITEM_GREAT_BALL, ITEM_GREAT_BALL, 600, TRUE},
    {ITEM_ULTRA_BALL, ITEM_ULTRA_BALL, 1200, TRUE},
    {ITEM_MAX_POTION, ITEM_MAX_POTION, 2500, TRUE},
    {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE, 3000, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_FULL_HEAL, ITEM_FULL_HEAL, 600, TRUE},
    {ITEM_MAX_REPEL, ITEM_MAX_REPEL, 700, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sTwoIslandShop[] = {
    {ITEM_GREAT_BALL, ITEM_GREAT_BALL, 600, TRUE},
    {ITEM_ULTRA_BALL, ITEM_ULTRA_BALL, 1200, TRUE},
    {ITEM_TIMER_BALL, ITEM_TIMER_BALL, 1000, TRUE},
    {ITEM_REPEAT_BALL, ITEM_REPEAT_BALL, 1000, TRUE},
    {ITEM_LEMONADE, ITEM_LEMONADE, 350, TRUE},
    {ITEM_SODA_POP, ITEM_SODA_POP, 300, TRUE},
    {ITEM_FRESH_WATER, ITEM_FRESH_WATER, 200, TRUE},
    {ITEM_MOOMOO_MILK, ITEM_MOOMOO_MILK, 500, TRUE},
    {ITEM_LAVA_COOKIE, ITEM_LAVA_COOKIE, 200, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sThreeIslandShop[] = {
    {ITEM_ULTRA_BALL, ITEM_ULTRA_BALL, 1200, TRUE},
    {ITEM_HYPER_POTION, ITEM_HYPER_POTION, 1200, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_FULL_HEAL, ITEM_FULL_HEAL, 600, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_MAX_REPEL, ITEM_MAX_REPEL, 700, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sFourIslandShop[] = {
    {ITEM_ULTRA_BALL, ITEM_ULTRA_BALL, 1200, TRUE},
    {ITEM_MAX_POTION, ITEM_MAX_POTION, 2500, TRUE},
    {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE, 3000, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_ICE_HEAL, ITEM_ICE_HEAL, 250, TRUE},
    {ITEM_FULL_HEAL, ITEM_FULL_HEAL, 600, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_MAX_REPEL, ITEM_MAX_REPEL, 700, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sSixIslandShop[] = {
    {ITEM_ULTRA_BALL, ITEM_ULTRA_BALL, 1200, TRUE},
    {ITEM_MAX_POTION, ITEM_MAX_POTION, 2500, TRUE},
    {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE, 3000, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_FULL_HEAL, ITEM_FULL_HEAL, 600, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_MAX_REPEL, ITEM_MAX_REPEL, 700, TRUE},
    {ITEM_DREAM_MAIL, ITEM_DREAM_MAIL, 50, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sSevenIslandShop[] = {
    {ITEM_GREAT_BALL, ITEM_GREAT_BALL, 600, TRUE},
    {ITEM_ULTRA_BALL, ITEM_ULTRA_BALL, 1200, TRUE},
    {ITEM_HYPER_POTION, ITEM_HYPER_POTION, 1200, TRUE},
    {ITEM_MAX_POTION, ITEM_MAX_POTION, 2500, TRUE},
    {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE, 3000, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_FULL_HEAL, ITEM_FULL_HEAL, 600, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_MAX_REPEL, ITEM_MAX_REPEL, 700, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sTrainerTowerShop[] = {
    {ITEM_GREAT_BALL, ITEM_GREAT_BALL, 600, TRUE},
    {ITEM_ULTRA_BALL, ITEM_ULTRA_BALL, 1200, TRUE},
    {ITEM_HYPER_POTION, ITEM_HYPER_POTION, 1200, TRUE},
    {ITEM_MAX_POTION, ITEM_MAX_POTION, 2500, TRUE},
    {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE, 3000, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_FULL_HEAL, ITEM_FULL_HEAL, 600, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_MAX_REPEL, ITEM_MAX_REPEL, 700, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sCeladonDeptVendingMachines[] = {
    {ITEM_FRESH_WATER, ITEM_FRESH_WATER, 200, TRUE},
    {ITEM_SODA_POP, ITEM_SODA_POP, 300, TRUE},
    {ITEM_LEMONADE, ITEM_LEMONADE, 350, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sCeladonGameCornerPrizeShop[] = {
    {ITEM_SMOKE_BALL, ITEM_SMOKE_BALL, 800, TRUE},
    {ITEM_MIRACLE_SEED, ITEM_MIRACLE_SEED, 1000, TRUE},
    {ITEM_CHARCOAL, ITEM_CHARCOAL, 1000, TRUE},
    {ITEM_MYSTIC_WATER, ITEM_MYSTIC_WATER, 1000, TRUE},
    {ITEM_YELLOW_FLUTE, ITEM_YELLOW_FLUTE, 1600, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sCeladonGameCornerTMPrizeShop[] = {
    {ITEM_TM13, ITEM_TM13, 4000, TRUE},
    {ITEM_TM23, ITEM_TM23, 3500, TRUE},
    {ITEM_TM24, ITEM_TM24, 4000, TRUE},
    {ITEM_TM30, ITEM_TM30, 4500, TRUE},
    {ITEM_TM35, ITEM_TM35, 4000, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

static const struct ShopItem sPokemonCenterShop[] = {
    {ITEM_POKE_BALL, ITEM_POKE_BALL, 200, TRUE},
    {ITEM_GREAT_BALL, ITEM_GREAT_BALL, 600, TRUE},
    {ITEM_ULTRA_BALL, ITEM_ULTRA_BALL, 1200, TRUE},
    {ITEM_POTION, ITEM_POTION, 300, TRUE},
    {ITEM_SUPER_POTION, ITEM_SUPER_POTION, 700, TRUE},
    {ITEM_HYPER_POTION, ITEM_HYPER_POTION, 1200, TRUE},
    {ITEM_MAX_POTION, ITEM_MAX_POTION, 2500, TRUE},
    {ITEM_FULL_RESTORE, ITEM_FULL_RESTORE, 3000, TRUE},
    {ITEM_REVIVE, ITEM_REVIVE, 1500, TRUE},
    {ITEM_ANTIDOTE, ITEM_ANTIDOTE, 100, TRUE},
    {ITEM_PARALYZE_HEAL, ITEM_PARALYZE_HEAL, 200, TRUE},
    {ITEM_AWAKENING, ITEM_AWAKENING, 250, TRUE},
    {ITEM_BURN_HEAL, ITEM_BURN_HEAL, 250, TRUE},
    {ITEM_ICE_HEAL, ITEM_ICE_HEAL, 250, TRUE},
    {ITEM_FULL_HEAL, ITEM_FULL_HEAL, 600, TRUE},
    {ITEM_ESCAPE_ROPE, ITEM_ESCAPE_ROPE, 550, TRUE},
    {ITEM_REPEL, ITEM_REPEL, 350, TRUE},
    {ITEM_SUPER_REPEL, ITEM_SUPER_REPEL, 500, TRUE},
    {ITEM_MAX_REPEL, ITEM_MAX_REPEL, 700, TRUE},
    {ITEM_X_ATTACK, ITEM_X_ATTACK, 500, TRUE},
    {ITEM_X_DEFEND, ITEM_X_DEFEND, 550, TRUE},
    {ITEM_X_SPEED, ITEM_X_SPEED, 350, TRUE},
    {ITEM_X_SPECIAL, ITEM_X_SPECIAL, 350, TRUE},
    {ITEM_X_ACCURACY, ITEM_X_ACCURACY, 950, TRUE},
    {ITEM_GUARD_SPEC, ITEM_GUARD_SPEC, 700, TRUE},
    {ITEM_DIRE_HIT, ITEM_DIRE_HIT, 650, TRUE},
    {ITEM_NONE, ITEM_NONE, 0, TRUE}
};

// Functions
static u8 CreateShopMenu(u8 martType)
{
    sShopData.martType = GetMartTypeFromItemList(martType);
    sShopData.selectedRow = 0;
    if (ContextNpcGetTextColor() == NPC_TEXT_COLOR_MALE)
        sShopData.fontId = FONT_MALE;
    else
        sShopData.fontId = FONT_FEMALE;

    if (sShopData.shopId == SHOP_CELADON_CITY_DEPT_VENDING_MACHINES ||
        sShopData.shopId == SHOP_CELADON_CITY_GAME_CORNER_PRIZE ||
        sShopData.shopId == SHOP_CELADON_CITY_GAME_CORNER_TM_PRIZE)
    {
        sShopMenuWindowId = AddWindow(&sShopMenuSmallWindowTemplate);
        SetStdWindowBorderStyle(sShopMenuWindowId, 0);
        PrintTextArray(sShopMenuWindowId, FONT_NORMAL, GetMenuCursorDimensionByFont(FONT_NORMAL, 0), 2, 16, 2, sShopMenuActions_BuyQuit);
        Menu_InitCursor(sShopMenuWindowId, FONT_NORMAL, 0, 2, 16, 2, 0);
    }
    else
    {
        sShopMenuWindowId = AddWindow(&sShopMenuWindowTemplate);
        SetStdWindowBorderStyle(sShopMenuWindowId, 0);
        PrintTextArray(sShopMenuWindowId, FONT_NORMAL, GetMenuCursorDimensionByFont(FONT_NORMAL, 0), 2, 16, 3, sShopMenuActions_BuySellQuit);
        Menu_InitCursor(sShopMenuWindowId, FONT_NORMAL, 0, 2, 16, 3, 0);
    }
    PutWindowTilemap(sShopMenuWindowId);
    CopyWindowToVram(sShopMenuWindowId, COPYWIN_MAP);
    return CreateTask(Task_ShopMenu, 8);
}

static u16 GetItem(u16 index)
{
    if (sShopData.shopId >= SHOPSANITY_FIRST_INDEX && sShopData.shopId <= SHOPSANITY_LAST_INDEX &&
        GetSetItemBought(index, FLAG_GET_BOUGHT) &&
        sShopData.itemList[index].repeatable)
    {
        return sShopData.itemList[index].repeatableItem;
    }
    return sShopData.itemList[index].item;
}

static u8 GetMartTypeFromItemList(u32 martType)
{
    u16 i;

    if (martType != MART_TYPE_REGULAR && martType != MART_TYPE_COIN)
        return martType;

    if (gArchipelagoOptions.isShopsanity)
        return martType;

    for (i = 0; i < sShopData.itemCount && GetItem(i) != 0; i++)
    {
        if (ItemId_GetPocket(GetItem(i)) == POCKET_TM_CASE)
        {
            if (martType == MART_TYPE_REGULAR)
                return MART_TYPE_TMHM;
            else
                return MART_TYPE_TMHM_COIN;
        }
    }

    return martType;
}

static const struct ShopItem* GetShopItemsFromShopId(u8 shopId)
{
    switch (shopId)
    {
    case SHOP_BETTER:
        return sBetterShop;
    case SHOP_VIRIDIAN_CITY:
        return sViridianShop;
    case SHOP_PEWTER_CITY:
        return sPewterShop;
    case SHOP_CERULEAN_CITY:
        return sCeruleanShop;
    case SHOP_VERMILION_CITY:
        return sVermilionShop;
    case SHOP_LAVENDER_TOWN:
        return sLavenderShop;
    case SHOP_CELADON_CITY_DEPT_ITEM:
        return sCeladonDeptItemShop;
    case SHOP_CELADON_CITY_DEPT_TM:
        return sCeladonDeptTMShop;
    case SHOP_CELADON_CITY_DEPT_EVO:
        return sCeladonDeptEvoShop;
    case SHOP_CELADON_CITY_DEPT_HELD:
        return sCeladonDeptHeldShop;
    case SHOP_CELADON_CITY_DEPT_BATTLE:
        return sCeladonDeptBattleShop;
    case SHOP_CELADON_CITY_DEPT_VITAMIN:
        return sCeladonDeptVitaminShop;
    case SHOP_FUCHSIA_CITY:
        return sFuchsiaShop;
    case SHOP_SAFFRON_CITY:
        return sSaffronShop;
    case SHOP_CINNABAR_ISLAND:
        return sCinnabarShop;
    case SHOP_INDIGO_PLATEAU:
        return sIndigoShop;
    case SHOP_TWO_ISLAND:
        return sTwoIslandShop;
    case SHOP_THREE_ISLAND:
        return sThreeIslandShop;
    case SHOP_FOUR_ISLAND:
        return sFourIslandShop;
    case SHOP_SIX_ISLAND:
        return sSixIslandShop;
    case SHOP_SEVEN_ISLAND:
        return sSevenIslandShop;
    case SHOP_TRAINER_TOWER:
        return sTrainerTowerShop;
    case SHOP_CELADON_CITY_DEPT_VENDING_MACHINES:
        return sCeladonDeptVendingMachines;
    case SHOP_CELADON_CITY_GAME_CORNER_PRIZE:
        return sCeladonGameCornerPrizeShop;
    case SHOP_CELADON_CITY_GAME_CORNER_TM_PRIZE:
        return sCeladonGameCornerTMPrizeShop;
    case SHOP_POKEMON_CENTER:
        return sPokemonCenterShop;
    default:
        break;
    }

    return sBetterShop;
}

static void SetShopItemsForSale(u8 shopId)
{
    sShopData.itemList = GetShopItemsFromShopId(shopId);
    sShopData.itemCount = 0;
    sShopData.shopId = shopId;
    if (GetItem(0) == 0)
        return;

    while (GetItem(sShopData.itemCount))
    {
        ++sShopData.itemCount;
    }
}

static u32 GetItemPrice(u16 index)
{
    return sShopData.itemList[index].price;
}

static void SetShopMenuCallback(void (*callback)(void))
{
    sShopData.callback = callback;
}

static void Task_ShopMenu(u8 taskId)
{
    s8 input = Menu_ProcessInputNoWrapAround();

    switch (input)
    {
    case MENU_NOTHING_CHOSEN:
        break;
    case MENU_B_PRESSED:
        PlaySE(SE_SELECT);
        Task_HandleShopMenuQuit(taskId);
        break;
    default:
        if (sShopData.shopId == SHOP_CELADON_CITY_DEPT_VENDING_MACHINES ||
            sShopData.shopId == SHOP_CELADON_CITY_GAME_CORNER_PRIZE ||
            sShopData.shopId == SHOP_CELADON_CITY_GAME_CORNER_TM_PRIZE)
            sShopMenuActions_BuyQuit[Menu_GetCursorPos()].func.void_u8(taskId);
        else
            sShopMenuActions_BuySellQuit[Menu_GetCursorPos()].func.void_u8(taskId);
        break;
    }
}

static void Task_HandleShopMenuBuy(u8 taskId)
{
    SetWordTaskArg(taskId, 0xE, (u32)CB2_InitBuyMenu);
    FadeScreen(FADE_TO_BLACK, 0);
    gTasks[taskId].func = Task_GoToBuyOrSellMenu;
}

static void Task_HandleShopMenuSell(u8 taskId)
{
    SetWordTaskArg(taskId, 0xE, (u32)CB2_GoToSellMenu);
    FadeScreen(FADE_TO_BLACK, 0);
    gTasks[taskId].func = Task_GoToBuyOrSellMenu;
}

static void CB2_GoToSellMenu(void)
{
    GoToBagMenu(ITEMMENULOCATION_SHOP, OPEN_BAG_LAST, CB2_ReturnToField);
    gFieldCallback = MapPostLoadHook_ReturnToShopMenu;
}

static void Task_HandleShopMenuQuit(u8 taskId)
{
    if (sShopData.shopId >= SHOPSANITY_FIRST_INDEX && sShopData.shopId <= SHOPSANITY_LAST_INDEX)
        FlagSet(SHOP_HINT_FLAGS_START + sShopData.shopId - 1);
    ClearShopMenuWindow();
    RecordTransactionForQuestLog();
    DestroyTask(taskId);
    if (sShopData.callback != NULL)
        sShopData.callback();
}

static void ClearShopMenuWindow(void)
{
    ClearStdWindowAndFrameToTransparent(sShopMenuWindowId, 2);
    RemoveWindow(sShopMenuWindowId);
}

static void Task_GoToBuyOrSellMenu(u8 taskId)
{
    if (gPaletteFade.active)
        return;

    SetMainCallback2((void *)GetWordTaskArg(taskId, 0xE));
    FreeAllWindowBuffers();
    DestroyTask(taskId);
}

static void MapPostLoadHook_ReturnToShopMenu(void)
{
    FadeInFromBlack();
    CreateTask(Task_ReturnToShopMenu, 8);
}

static void Task_ReturnToShopMenu(u8 taskId)
{
    if (IsWeatherNotFadingIn() != TRUE)
        return;

    if (sShopData.shopId == SHOP_CELADON_CITY_DEPT_VENDING_MACHINES)
        DisplayItemMessageOnField(taskId, GetMartFontId(), gText_AnythingElseToDo, ShowShopMenuAfterExitingBuyOrSellMenu);
    else
        DisplayItemMessageOnField(taskId, GetMartFontId(), gText_AnythingElseICanHelp, ShowShopMenuAfterExitingBuyOrSellMenu);
}

static void ShowShopMenuAfterExitingBuyOrSellMenu(u8 taskId)
{
    CreateShopMenu(sShopData.martType);
    DestroyTask(taskId);
}

void CB2_BuyMenu(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
    DoScheduledBgTilemapCopiesToVram();
}

static void VBlankCB_BuyMenu(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_InitBuyMenu(void)
{
    u8 taskId;

    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        CpuFastFill(0, (void *)OAM, 0x400);
        ScanlineEffect_Stop();
        ResetTempTileDataBuffers();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        ClearScheduledBgCopiesToVram();
        ResetItemMenuIconState();
        if (!(InitShopData()) || !(BuyMenuBuildListMenuTemplate()))
            return;
        BuyMenuInitBgs();
        FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 0x20, 0x20);
        FillBgTilemapBufferRect_Palette0(1, 0, 0, 0, 0x20, 0x20);
        FillBgTilemapBufferRect_Palette0(2, 0, 0, 0, 0x20, 0x20);
        FillBgTilemapBufferRect_Palette0(3, 0, 0, 0, 0x20, 0x20);
        if (sShopData.martType == MART_TYPE_TMHM || sShopData.martType == MART_TYPE_TMHM_COIN)
            BuyMenuInitWindows(TRUE);
        else
            BuyMenuInitWindows(FALSE);
        BuyMenuDecompressBgGraphics();
        gMain.state++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible())
            return;
        gMain.state++;
        break;
    default:
        sShopData.selectedRow = 0;
        sShopData.scrollOffset = 0;
        BuyMenuDrawGraphics();
        BuyMenuAddScrollIndicatorArrows();
        taskId = CreateTask(Task_BuyMenu, 8);
        gTasks[taskId].tListTaskId = ListMenuInit(&gMultiuseListMenuTemplate, 0, 0);
        BlendPalettes(PALETTES_ALL, 0x10, RGB_BLACK);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_BuyMenu);
        SetMainCallback2(CB2_BuyMenu);
        break;
    }
}

static bool8 InitShopData(void)
{
    gShopTilemapBuffer1 = Alloc(sizeof(*gShopTilemapBuffer1));
    if (gShopTilemapBuffer1 == NULL)
    {
        BuyMenuFreeMemory();
        SetShopExitCallback();
        return FALSE;
    }

    gShopTilemapBuffer2 = Alloc(sizeof(*gShopTilemapBuffer2));
    if (gShopTilemapBuffer2 == NULL)
    {
        BuyMenuFreeMemory();
        SetShopExitCallback();
        return FALSE;
    }

    gShopTilemapBuffer3 = Alloc(sizeof(*gShopTilemapBuffer3));
    if (gShopTilemapBuffer3 == NULL)
    {
        BuyMenuFreeMemory();
        SetShopExitCallback();
        return FALSE;
    }

    gShopTilemapBuffer4 = Alloc(sizeof(*gShopTilemapBuffer4));
    if (gShopTilemapBuffer4 == NULL)
    {
        BuyMenuFreeMemory();
        SetShopExitCallback();
        return FALSE;
    }

    return TRUE;
}

static void BuyMenuInitBgs(void)
{
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sShopBuyMenuBgTemplates, NELEMS(sShopBuyMenuBgTemplates));
    SetBgTilemapBuffer(1, gShopTilemapBuffer2);
    SetBgTilemapBuffer(2, gShopTilemapBuffer4);
    SetBgTilemapBuffer(3, gShopTilemapBuffer3);
    SetGpuReg(REG_OFFSET_BG0HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG0VOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG1HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG1VOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG2HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG2VOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG3HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG3VOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BLDCNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
}

static void BuyMenuDecompressBgGraphics(void)
{
    u16 *pal;

    DecompressAndCopyTileDataToVram(1, gBuyMenuFrame_Gfx, 0x480, 0x3DC, 0);
    if (sShopData.martType != MART_TYPE_TMHM && sShopData.martType != MART_TYPE_TMHM_COIN)
        LZDecompressWram(gBuyMenuFrame_Tilemap, gShopTilemapBuffer1);
    else
        LZDecompressWram(gBuyMenuFrame_TmHmTilemap, gShopTilemapBuffer1);

    pal = Alloc(2 * PLTT_SIZE_4BPP);
    LZDecompressWram(gBuyMenuFrame_Pal, pal);
    LoadPalette(&pal[0 * 16], BG_PLTT_ID(11), PLTT_SIZE_4BPP);
    LoadPalette(&pal[1 * 16], BG_PLTT_ID(6), PLTT_SIZE_4BPP);
    Free(pal);
}

static void RecolorItemDescriptionBox(bool32 a0)
{
    u8 paletteNum;

    if (a0 == FALSE)
        paletteNum = 0xB;
    else
        paletteNum = 0x6;

    if (sShopData.martType != MART_TYPE_TMHM && sShopData.martType != MART_TYPE_TMHM_COIN)
        SetBgTilemapPalette(1, 0, 14, 30, 6, paletteNum);
    else
        SetBgTilemapPalette(1, 0, 12, 30, 8, paletteNum);

    ScheduleBgCopyTilemapToVram(1);
}

static void BuyMenuDrawGraphics(void)
{
    BuyMenuDrawMapView();
    BuyMenuCopyTilemapData();
    if (sShopData.martType == MART_TYPE_COIN || sShopData.martType == MART_TYPE_TMHM_COIN)
        BuyMenuDrawCoinBox();
    else
        BuyMenuDrawMoneyBox();
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    ScheduleBgCopyTilemapToVram(2);
    ScheduleBgCopyTilemapToVram(3);
}

bool8 BuyMenuBuildListMenuTemplate(void)
{
    u16 i, v;

    sShopMenuListMenu = Alloc((sShopData.itemCount + 1) * sizeof(*sShopMenuListMenu));
    if (sShopMenuListMenu == NULL
     || (sShopMenuItemStrings = Alloc((sShopData.itemCount + 1) * sizeof(*sShopMenuItemStrings))) == NULL)
    {
        BuyMenuFreeMemory();
        SetShopExitCallback();
        return FALSE;
    }

    for (i = 0; i < sShopData.itemCount; i++)
    {
        PokeMartWriteNameAndIdAt(&sShopMenuListMenu[i], GetItem(i), sShopMenuItemStrings[i]);
    }
    StringCopy(sShopMenuItemStrings[i], gFameCheckerText_Cancel);
    sShopMenuListMenu[i].label = sShopMenuItemStrings[i];
    sShopMenuListMenu[i].index = -2;
    gMultiuseListMenuTemplate.items = sShopMenuListMenu;
    gMultiuseListMenuTemplate.totalItems = sShopData.itemCount + 1;
    gMultiuseListMenuTemplate.windowId = 4;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 9;
    gMultiuseListMenuTemplate.cursor_X = 1;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 2;
    gMultiuseListMenuTemplate.upText_Y = 2;
    gMultiuseListMenuTemplate.fontId = 2;
    gMultiuseListMenuTemplate.fillValue = 0;
    gMultiuseListMenuTemplate.cursorPal = GetFontAttribute(FONT_NORMAL, FONTATTR_COLOR_FOREGROUND);
    gMultiuseListMenuTemplate.cursorShadowPal = GetFontAttribute(FONT_NORMAL, FONTATTR_COLOR_SHADOW);
    gMultiuseListMenuTemplate.moveCursorFunc = BuyMenuPrintItemDescriptionAndShowItemIcon;
    gMultiuseListMenuTemplate.itemPrintFunc = BuyMenuPrintPriceInList;
    gMultiuseListMenuTemplate.scrollMultiple = 0;
    gMultiuseListMenuTemplate.cursorKind = 0;

    if (sShopData.martType == MART_TYPE_TMHM || sShopData.martType == MART_TYPE_TMHM_COIN)
        v = 5;
    else
        v = 6;

    if ((sShopData.itemCount + 1) > v)
        gMultiuseListMenuTemplate.maxShowed = v;
    else
        gMultiuseListMenuTemplate.maxShowed = sShopData.itemCount + 1;

    sShopData.itemsShowed = gMultiuseListMenuTemplate.maxShowed;
    return TRUE;
}

static void PokeMartWriteNameAndIdAt(struct ListMenuItem *list, u16 index, u8 *dst)
{
    CopyItemName(index, dst);
    list->label = dst;
    list->index = index;
}

static void BuyMenuPrintItemDescriptionAndShowItemIcon(s32 item, bool8 onInit, struct ListMenu *list, u16 index)
{
    const u8 *description;

    if (onInit != TRUE)
        PlaySE(SE_SELECT);

    if (item != INDEX_CANCEL)
    	if (item == ITEM_ARCHIPELAGO_PROGRESSION)
            description = ItemId_GetAPItemDescription(SHOPSANITY_FLAGS_START + ((sShopData.shopId - SHOPSANITY_FIRST_INDEX) * 16) + index);
    	else if (sShopData.martType != MART_TYPE_TMHM && sShopData.martType != MART_TYPE_TMHM_COIN && item >= ITEM_TM01 && item <= ITEM_TM50)
            description = gMoveNames[ItemIdToBattleMoveId(item)];
    	else
            description = ItemId_GetDescription(item);
    else
        description = gText_QuitShopping;

    FillWindowPixelBuffer(5, PIXEL_FILL(0));
    if (sShopData.martType != MART_TYPE_TMHM && sShopData.martType != MART_TYPE_TMHM_COIN)
    {
        DestroyItemMenuIcon(sShopData.itemSlot ^ 1);
        if (item != INDEX_CANCEL)
            CreateItemMenuIcon(item, sShopData.itemSlot);
        else
            CreateItemMenuIcon(ITEMS_COUNT, sShopData.itemSlot);

        sShopData.itemSlot ^= 1;
        BuyMenuPrint(5, FONT_NORMAL, description, 0, 3, 2, 1, 0, 0);
    }
    else //TM Mart
    {
        FillWindowPixelBuffer(6, PIXEL_FILL(0));
        LoadTmHmNameInMart(item);
        BuyMenuPrint(5, FONT_NORMAL, description, 2, 3, 1, 0, 0, 0);
    }
}

bool8 GetSetItemBought(u16 index, u8 caseId)
{
    u32 mask = 1 << index;

    switch (caseId)
    {
    case FLAG_GET_BOUGHT:
        if (sShopData.shopId == SHOP_POKEMON_CENTER)
            return (gSaveBlock2Ptr->centerShopItemFlags & mask) != 0;
        else
            return (gSaveBlock2Ptr->shopItemFlags[sShopData.shopId - SHOPSANITY_FIRST_INDEX] & mask) != 0;
    case FLAG_SET_BOUGHT:
        gSaveBlock2Ptr->shopItemFlags[sShopData.shopId - SHOPSANITY_FIRST_INDEX] |= mask;
        return TRUE;
    }

    return FALSE;
}

bool8 IsTwoIslandShopItemAvailable(u16 index)
{
    if ((index == 1 || index == 5) &&
        !FlagGet(FLAG_RESCUED_LOSTELLE))
        return FALSE;
    else if ((index == 4 || index == 7) &&
             (!FlagGet(FLAG_RESCUED_LOSTELLE) || !FlagGet(FLAG_SYS_GAME_CLEAR)))
        return FALSE;
    else if ((index == 2 || index == 3 || index == 8) &&
             (!FlagGet(FLAG_RESCUED_LOSTELLE) || !FlagGet(FLAG_SYS_GAME_CLEAR) || !FlagGet(FLAG_SYS_CAN_LINK_WITH_RS)))
        return FALSE;
    return TRUE;
}

static void BuyMenuPrintPriceInList(u8 windowId, u32 item, u8 y, u16 index)
{
    s32 x;
    u8 *loc;

    if (item != INDEX_CANCEL)
    {
        if (!sShopData.itemList[index].repeatable && GetSetItemBought(index, FLAG_GET_BOUGHT))
        {
            StringCopy(gStringVar1, gText_SoldOut);
            x = 8 - StringLength(gStringVar1);
            loc = gStringVar5;
            while (x-- != 0)
                *loc++ = 0;
            StringExpandPlaceholders(loc, gStringVar1);
            BuyMenuPrint(windowId, FONT_SMALL, gStringVar5, 0x5E, y, 0, 0, TEXT_SKIP_DRAW, 1);
        }
        else if((sShopData.shopId == SHOP_POKEMON_CENTER && !GetSetItemBought(index, FLAG_GET_BOUGHT)) ||
                (sShopData.shopId == SHOP_TWO_ISLAND && !IsTwoIslandShopItemAvailable(index)))
        {
            StringCopy(gStringVar1, gText_NA);
            x = 3 - StringLength(gStringVar1);
            loc = gStringVar5;
            while (x-- != 0)
                *loc++ = 0;
            StringExpandPlaceholders(loc, gStringVar1);
            BuyMenuPrint(windowId, FONT_SMALL, gStringVar5, 0x76, y, 0, 0, TEXT_SKIP_DRAW, 1);
        }
        else
        {
            ConvertIntToDecimalStringN(gStringVar1, GetItemPrice(index), 0, 5);
            x = 5 - StringLength(gStringVar1);
            loc = gStringVar5;
            while (x-- != 0)
                *loc++ = 0;
            if (sShopData.martType != MART_TYPE_COIN && sShopData.martType != MART_TYPE_TMHM_COIN)
            {
                StringExpandPlaceholders(loc, gText_PokedollarVar1);
                BuyMenuPrint(windowId, FONT_SMALL, gStringVar5, 0x64, y, 0, 0, TEXT_SKIP_DRAW, 1);
            }
            else
            {
                StringExpandPlaceholders(loc, gText_NoPokedollarVar1);
                BuyMenuPrint(windowId, FONT_SMALL, gStringVar5, 0x6E, y, 0, 0, TEXT_SKIP_DRAW, 1);
            }

        }
    }
}

static void LoadTmHmNameInMart(s32 item)
{
    if (item != INDEX_CANCEL)
    {
        ConvertIntToDecimalStringN(gStringVar1, item - ITEM_DEVON_SCOPE, 2, 2);
        StringCopy(gStringVar5, gText_NumberClear01);
        StringAppend(gStringVar5, gStringVar1);
        BuyMenuPrint(6, FONT_SMALL, gStringVar5, 0, 0, 0, 0, TEXT_SKIP_DRAW, 1);
        StringCopy(gStringVar5, gMoveNames[ItemIdToBattleMoveId(item)]);
        BuyMenuPrint(6, FONT_NORMAL, gStringVar5, 0, 0x10, 0, 0, 0, 1);
    }
    else
    {
        BuyMenuPrint(6, FONT_SMALL, gText_ThreeHyphens, 0, 0, 0, 0, TEXT_SKIP_DRAW, 1);
        BuyMenuPrint(6, FONT_NORMAL, gText_SevenHyphens, 0, 0x10, 0, 0, 0, 1);
    }
}

u8 GetMartFontId(void)
{
    return sShopData.fontId;
}

static void BuyMenuPrintCursor(u8 listTaskId, u8 a1)
{
    BuyMenuPrintCursorAtYPosition(ListMenuGetYCoordForPrintingArrowCursor(listTaskId), a1);
}

static void BuyMenuPrintCursorAtYPosition(u8 y, u8 a1)
{
    if (a1 == 0xFF)
    {
        FillWindowPixelRect(4, 0, 1, y, GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_WIDTH), GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT));
        CopyWindowToVram(4, COPYWIN_GFX);
    }
    else
    {
        BuyMenuPrint(4, FONT_NORMAL, gText_SelectorArrow2, 1, y, 0, 0, 0, a1);
    }
}

static void BuyMenuFreeMemory(void)
{
    if (gShopTilemapBuffer1 != NULL)
        Free(gShopTilemapBuffer1);

    if (gShopTilemapBuffer2 != NULL)
        Free(gShopTilemapBuffer2);

    if (gShopTilemapBuffer3 != NULL)
        Free(gShopTilemapBuffer3);

    if (gShopTilemapBuffer4 != NULL)
        Free(gShopTilemapBuffer4);

    if (sShopMenuListMenu != NULL)
        Free(sShopMenuListMenu);

    if (sShopMenuItemStrings != NULL)
        Free(sShopMenuItemStrings);

    FreeAllWindowBuffers();
}

static void SetShopExitCallback(void)
{
    gFieldCallback = MapPostLoadHook_ReturnToShopMenu;
    SetMainCallback2(CB2_ReturnToField);
}


static void BuyMenuAddScrollIndicatorArrows(void)
{
    if (sShopData.martType != MART_TYPE_TMHM && sShopData.martType != MART_TYPE_TMHM_COIN)
    {
        sShopData.scrollArrows = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 160, 8, 104,
            (sShopData.itemCount - sShopData.itemsShowed) + 1, 110, 110, &sShopData.scrollOffset);
    }
    else
    {
        sShopData.scrollArrows = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 160, 8, 88,
            (sShopData.itemCount - sShopData.itemsShowed) + 1, 110, 110, &sShopData.scrollOffset);
    }
}

static void BuyQuantityAddScrollIndicatorArrows(void)
{
    sShopData.scrollArrowsOffset = 1;
    sShopData.scrollArrows = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 0x98, 0x48, 0x68, 2, 0x6E, 0x6E, &sShopData.scrollArrowsOffset);
}

static void BuyMenuRemoveScrollIndicatorArrows(void)
{
    if ((sShopData.scrollArrows) == 0x1F)
        return;

    RemoveScrollIndicatorArrowPair(sShopData.scrollArrows);
    sShopData.scrollArrows = 0x1F;
}

static void BuyMenuDrawMapView(void)
{
    BuyMenuCollectObjectEventData();
    BuyMenuDrawObjectEvents();
    BuyMenuDrawMapBg();
}

static void BuyMenuDrawMapBg(void)
{
    s16 i, j, x, y;
    const struct MapLayout *mapLayout;
    u16 metatile;
    u8 metatileLayerType;

    mapLayout = gMapHeader.mapLayout;
    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    x -= 2;
    y -= 3;

    for (j = 0; j < 10; j++)
    {
        for (i = 0; i < 5; i++)
        {
            metatile = MapGridGetMetatileIdAt(x + i, y + j);
            metatileLayerType = MapGridGetMetatileLayerTypeAt(x + i, y + j);

            if (metatile < NUM_METATILES_IN_PRIMARY)
                BuyMenuDrawMapMetatile(i, j, mapLayout->primaryTileset->metatiles + metatile * 8, metatileLayerType);
            else
                BuyMenuDrawMapMetatile(i, j, mapLayout->secondaryTileset->metatiles + ((metatile - NUM_METATILES_IN_PRIMARY) * 8), metatileLayerType);
        }
    }
}

static void BuyMenuDrawMapMetatile(s16 x, s16 y, const u16 *src, u8 metatileLayerType)
{
    u16 offset1 = x * 2;
    u16 offset2 = y * 64 + 64;

    switch (metatileLayerType)
    {
    case METATILE_LAYER_TYPE_NORMAL:
        BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer4, offset1, offset2, src);
        BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer2, offset1, offset2, src + 4);
        break;
    case METATILE_LAYER_TYPE_COVERED:
        BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer3, offset1, offset2, src);
        BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer4, offset1, offset2, src + 4);
        break;
    case METATILE_LAYER_TYPE_SPLIT:
        BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer3, offset1, offset2, src);
        BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer2, offset1, offset2, src + 4);
        break;
    }
}

static void BuyMenuDrawMapMetatileLayer(u16 *dest, s16 offset1, s16 offset2, const u16 *src)
{
    dest[offset1 + offset2] = src[0]; // top left
    dest[offset1 + offset2 + 1] = src[1]; // top right
    dest[offset1 + offset2 + 32] = src[2]; // bottom left
    dest[offset1 + offset2 + 33] = src[3]; // bottom right
}

static void BuyMenuCollectObjectEventData(void)
{
    s16 facingX, facingY;
    u8 x, y, elevation;
    u8 num = 0;

    GetXYCoordsOneStepInFrontOfPlayer(&facingX, &facingY);
    elevation = PlayerGetElevation();

    for (y = 0; y < OBJECT_EVENTS_COUNT; y++)
        sViewportObjectEvents[y][OBJECT_EVENT_ID] = OBJECT_EVENTS_COUNT;

    for (y = 0; y < 5; y++)
    {
        for (x = 0; x < 7; x++)
        {
            u8 eventObjId = GetObjectEventIdByPosition(facingX - 3 + x, facingY - 2 + y, elevation);
            if (eventObjId != OBJECT_EVENTS_COUNT)
            {
                sViewportObjectEvents[num][OBJECT_EVENT_ID] = eventObjId;
                sViewportObjectEvents[num][X_COORD] = x;
                sViewportObjectEvents[num][Y_COORD] = y;

                switch (gObjectEvents[eventObjId].facingDirection)
                {
                    case DIR_SOUTH:
                        sViewportObjectEvents[num][ANIM_NUM] = 0;
                        break;
                    case DIR_NORTH:
                        sViewportObjectEvents[num][ANIM_NUM] = 1;
                        break;
                    case DIR_WEST:
                        sViewportObjectEvents[num][ANIM_NUM] = 2;
                        break;
                    case DIR_EAST:
                    default:
                        sViewportObjectEvents[num][ANIM_NUM] = 3;
                        break;
                }
                num++;
            }
        }
    }
}

static void BuyMenuDrawObjectEvents(void)
{
    u8 i, spriteId;
    const struct ObjectEventGraphicsInfo *graphicsInfo;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (sViewportObjectEvents[i][OBJECT_EVENT_ID] == OBJECT_EVENTS_COUNT)
            continue;

        graphicsInfo = GetObjectEventGraphicsInfo(gObjectEvents[sViewportObjectEvents[i][OBJECT_EVENT_ID]].graphicsId);
        spriteId = CreateObjectGraphicsSprite(
            gObjectEvents[sViewportObjectEvents[i][OBJECT_EVENT_ID]].graphicsId,
            SpriteCallbackDummy,
            (u16)sViewportObjectEvents[i][X_COORD] * 16 - 8,
            (u16)sViewportObjectEvents[i][Y_COORD] * 16 + 48 - graphicsInfo->height / 2,
            2);
        StartSpriteAnim(&gSprites[spriteId], sViewportObjectEvents[i][ANIM_NUM]);
    }
}

static void BuyMenuCopyTilemapData(void)
{
    s16 i;
    u16 *dst = *gShopTilemapBuffer2;
    u16 *src = *gShopTilemapBuffer1;

    for (i = 0; i < 0x400; i++)
    {
        if (src[i] == 0)
            continue;
        dst[i] = src[i] + 0xb3dc;
    }
}

static void BuyMenuPrintItemQuantityAndPrice(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    FillWindowPixelBuffer(3, PIXEL_FILL(1));
    if (sShopData.martType == MART_TYPE_COIN || sShopData.martType == MART_TYPE_TMHM_COIN)
        PrintCoinAmount(3, 0x23, 0xA, sShopData.itemPrice, TEXT_SKIP_DRAW);
    else
        PrintMoneyAmount(3, 0x36, 0xA, sShopData.itemPrice, TEXT_SKIP_DRAW);
    ConvertIntToDecimalStringN(gStringVar1, tItemCount, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringExpandPlaceholders(gStringVar5, gText_TimesStrVar1);
    BuyMenuPrint(3, FONT_SMALL, gStringVar5, 2, 0xA, 0, 0, 0, 1);
}

static void Task_BuyMenu(u8 taskId)
{
    u16 index;
    s16 *data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        s32 itemId = ListMenu_ProcessInput(tListTaskId);
        ListMenuGetScrollAndRow(tListTaskId, &sShopData.scrollOffset, &sShopData.selectedRow);
        ListMenuGetCurrentItemArrayId(tListTaskId, &index);
        switch (itemId)
        {
        case LIST_NOTHING_CHOSEN:
            break;
        case LIST_CANCEL:
            PlaySE(SE_SELECT);
            ExitBuyMenu(taskId);
            break;
        default:
            PlaySE(SE_SELECT);
            tItemId = itemId;
            ClearWindowTilemap(5);
            BuyMenuRemoveScrollIndicatorArrows();
            BuyMenuPrintCursor(tListTaskId, 2);
            RecolorItemDescriptionBox(1);
            sShopData.itemPrice = GetItemPrice(index);
            if (!sShopData.itemList[index].repeatable && GetSetItemBought(index, FLAG_GET_BOUGHT))
            {
                BuyMenuDisplayMessage(taskId, gText_SorryWereOutOfThis, BuyMenuReturnToItemList);
            }
            else if((sShopData.shopId == SHOP_POKEMON_CENTER && !GetSetItemBought(index, FLAG_GET_BOUGHT)) ||
                    (sShopData.shopId == SHOP_TWO_ISLAND && !IsTwoIslandShopItemAvailable(index)))
            {
                BuyMenuDisplayMessage(taskId, gText_SorryDontSellYet, BuyMenuReturnToItemList);
            }
            else if ((sShopData.martType == MART_TYPE_REGULAR || sShopData.martType == MART_TYPE_TMHM) &&
                     !IsEnoughMoney(&gSaveBlock1Ptr->money, sShopData.itemPrice))
            {
                BuyMenuDisplayMessage(taskId, gText_YouDontHaveMoney, BuyMenuReturnToItemList);
            }
            else if ((sShopData.martType == MART_TYPE_COIN || sShopData.martType == MART_TYPE_TMHM_COIN) &&
                     !IsEnoughCoins(sShopData.itemPrice))
            {
                BuyMenuDisplayMessage(taskId, gText_YouDontHaveCoins, BuyMenuReturnToItemList);
            }
            else
            {
                CopyItemName(itemId, gStringVar1);
                if (!sShopData.itemList[index].repeatable || GetItem(index) == ITEM_ARCHIPELAGO_PROGRESSION)
                {
                    tItemCount = 1;
                    PlaySE(SE_SELECT);
                    BuyMenuRemoveScrollIndicatorArrows();
                    ClearStdWindowAndFrameToTransparent(3, FALSE);
                    ClearStdWindowAndFrameToTransparent(1, FALSE);
                    ClearWindowTilemap(3);
                    ClearWindowTilemap(1);
                    PutWindowTilemap(4);
                    CopyItemName(tItemId, gStringVar1);
                    ConvertIntToDecimalStringN(gStringVar2, tItemCount, STR_CONV_MODE_LEFT_ALIGN, 2);
                    ConvertIntToDecimalStringN(gStringVar3, sShopData.itemPrice, STR_CONV_MODE_LEFT_ALIGN, 8);
                    if (sShopData.martType == MART_TYPE_COIN || sShopData.martType == MART_TYPE_TMHM_COIN)
                        BuyMenuDisplayMessage(taskId, gText_Var1AndYouWantedVar2Coins, CreateBuyMenuConfirmPurchaseWindow);
                    else
                        BuyMenuDisplayMessage(taskId, gText_Var1AndYouWantedVar2, CreateBuyMenuConfirmPurchaseWindow);
                }
                else
                    BuyMenuDisplayMessage(taskId, gText_Var1CertainlyHowMany, Task_BuyHowManyDialogueInit);
            }
            break;
        }
    }
}

static void Task_BuyHowManyDialogueInit(u8 taskId)
{
    u16 index;
    s16 *data = gTasks[taskId].data;
    u16 quantityInBag = BagGetQuantityByItemId(tItemId);
    u16 maxQuantity;

    ListMenuGetCurrentItemArrayId(tListTaskId, &index);
    BuyMenuQuantityBoxThinBorder(1, 0);
    ConvertIntToDecimalStringN(gStringVar1, quantityInBag, STR_CONV_MODE_RIGHT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar5, gText_InBagVar1);
    BuyMenuPrint(1, FONT_NORMAL, gStringVar5, 0, 2, 0, 0, 0, 1);
    tItemCount = 1;
    BuyMenuQuantityBoxNormalBorder(3, 0);
    BuyMenuPrintItemQuantityAndPrice(taskId);
    ScheduleBgCopyTilemapToVram(0);
    if (sShopData.martType == MART_TYPE_COIN || sShopData.martType == MART_TYPE_TMHM_COIN)
        maxQuantity = GetCoins() / GetItemPrice(index);
    else
        maxQuantity = GetMoney(&gSaveBlock1Ptr->money) / GetItemPrice(index);
    if (maxQuantity > 99)
        sShopData.maxQuantity = 99;
    else
        sShopData.maxQuantity = (u8)maxQuantity;

    if (maxQuantity != 1)
        BuyQuantityAddScrollIndicatorArrows();

    gTasks[taskId].func = Task_BuyHowManyDialogueHandleInput;
}

static void Task_BuyHowManyDialogueHandleInput(u8 taskId)
{
    u16 index;
    s16 *data = gTasks[taskId].data;

    if (AdjustQuantityAccordingToDPadInput(&tItemCount, sShopData.maxQuantity) == TRUE)
    {
        ListMenuGetCurrentItemArrayId(tListTaskId, &index);
        sShopData.itemPrice = GetItemPrice(index) * tItemCount;
        BuyMenuPrintItemQuantityAndPrice(taskId);
    }
    else
    {
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            BuyMenuRemoveScrollIndicatorArrows();
            ClearStdWindowAndFrameToTransparent(3, FALSE);
            ClearStdWindowAndFrameToTransparent(1, FALSE);
            ClearWindowTilemap(3);
            ClearWindowTilemap(1);
            PutWindowTilemap(4);
            CopyItemName(tItemId, gStringVar1);
            ConvertIntToDecimalStringN(gStringVar2, tItemCount, STR_CONV_MODE_LEFT_ALIGN, 2);
            ConvertIntToDecimalStringN(gStringVar3, sShopData.itemPrice, STR_CONV_MODE_LEFT_ALIGN, 8);
            if (sShopData.martType == MART_TYPE_COIN || sShopData.martType == MART_TYPE_TMHM_COIN)
                BuyMenuDisplayMessage(taskId, gText_Var1AndYouWantedVar2Coins, CreateBuyMenuConfirmPurchaseWindow);
            else
                BuyMenuDisplayMessage(taskId, gText_Var1AndYouWantedVar2, CreateBuyMenuConfirmPurchaseWindow);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            BuyMenuRemoveScrollIndicatorArrows();
            ClearStdWindowAndFrameToTransparent(3, FALSE);
            ClearStdWindowAndFrameToTransparent(1, FALSE);
            ClearWindowTilemap(3);
            ClearWindowTilemap(1);
            BuyMenuReturnToItemList(taskId);
        }
    }
}

static void CreateBuyMenuConfirmPurchaseWindow(u8 taskId)
{
    BuyMenuConfirmPurchase(taskId, sShopMenuActions_BuyReturn);
}

static void BuyMenuTryMakePurchase(u8 taskId)
{
    u16 index;
    s16 *data = gTasks[taskId].data;

    PutWindowTilemap(4);
    if (AddBagItem(tItemId, tItemCount) == TRUE)
    {
        if (tItemId == ITEM_FRESH_WATER)
            FlagSet(FLAG_PURCHASED_FRESH_WATER);
        if (tItemId == ITEM_SODA_POP)
            FlagSet(FLAG_PURCHASED_SODA_POP);
        if (tItemId == ITEM_LEMONADE)
            FlagSet(FLAG_PURCHASED_LEMONADE);

        if (sShopData.shopId >= SHOPSANITY_FIRST_INDEX && sShopData.shopId <= SHOPSANITY_LAST_INDEX)
        {
            ListMenuGetCurrentItemArrayId(tListTaskId, &index);
            GetSetItemBought(index, FLAG_SET_BOUGHT);
        }

        BuyMenuDisplayMessage(taskId, gText_HereYouGoThankYou, BuyMenuSubtractMoney);
        DebugFunc_PrintPurchaseDetails(taskId);
        RecordItemTransaction(taskId, tItemId, tItemCount, QL_EVENT_BOUGHT_ITEM - QL_EVENT_USED_POKEMART);
    }
    else
    {
        BuyMenuDisplayMessage(taskId, gText_NoMoreRoomForThis, BuyMenuReturnToItemList);
    }
}

static void BuyMenuSubtractMoney(u8 taskId)
{
    IncrementGameStat(GAME_STAT_SHOPPED);
    PlaySE(SE_SHOP);
    if (sShopData.martType == MART_TYPE_COIN || sShopData.martType == MART_TYPE_TMHM_COIN)
    {
        RemoveCoins(sShopData.itemPrice);
        PrintCoinsString_Parameterized(0, GetCoins(), 0x10, 0xC, 0);
    }
    else
    {
        RemoveMoney(&gSaveBlock1Ptr->money, sShopData.itemPrice);
        PrintMoneyAmountInMoneyBox(0, GetMoney(&gSaveBlock1Ptr->money), 0);
    }
    gTasks[taskId].func = Task_ReturnToItemListAfterItemPurchase;
}

static void Task_ReturnToItemListAfterItemPurchase(u8 taskId)
{
    u16 i;
    s16 *data = gTasks[taskId].data;
    struct ListMenu *list = (struct ListMenu *)gTasks[tListTaskId].data;

    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        for (i = 0; i < sShopData.itemCount; i++)
        {
            PokeMartWriteNameAndIdAt(&sShopMenuListMenu[i], GetItem(i), sShopMenuItemStrings[i]);
        }
        RedrawListMenu(tListTaskId);
        BuyMenuPrintItemDescriptionAndShowItemIcon(list->template.items[list->cursorPos + list->itemsAbove].index, FALSE, NULL, list->cursorPos + list->itemsAbove);
        BuyMenuReturnToItemList(taskId);
    }
}

static void BuyMenuReturnToItemList(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    ClearDialogWindowAndFrameToTransparent(2, FALSE);
    BuyMenuPrintCursor(tListTaskId, 1);
    RecolorItemDescriptionBox(0);
    PutWindowTilemap(4);
    PutWindowTilemap(5);
    if (sShopData.martType == MART_TYPE_TMHM || sShopData.martType == MART_TYPE_TMHM_COIN)
        PutWindowTilemap(6);

    ScheduleBgCopyTilemapToVram(0);
    BuyMenuAddScrollIndicatorArrows();
    gTasks[taskId].func = Task_BuyMenu;
}

static void ExitBuyMenu(u8 taskId)
{
    gFieldCallback = MapPostLoadHook_ReturnToShopMenu;
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_ExitBuyMenu;
}

static void Task_ExitBuyMenu(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        DestroyListMenuTask(tListTaskId, NULL, NULL);
        BuyMenuFreeMemory();
        SetMainCallback2(CB2_ReturnToField);
        DestroyTask(taskId);
    }
}

static void DebugFunc_PrintPurchaseDetails(u8 taskId)
{
}

static void DebugFunc_PrintShopMenuHistoryBeforeClearMaybe(void)
{
}

// Records a transaction during a single shopping session.
// This is for the Quest Log to save information about the player's purchases/sales when they finish.
void RecordItemTransaction(u8 taskId, u16 itemId, u16 quantity, u8 logEventId)
{
    u16 index;
    struct QuestLogEvent_Shop *history;
    s16 *data = gTasks[taskId].data;

    // There should only be a single entry for buying/selling respectively,
    // so if one has already been created then get it first.
    if (sHistory[0].logEventId == logEventId)
    {
        history = &sHistory[0];
    }
    else if (sHistory[1].logEventId == logEventId)
    {
        history = &sHistory[1];
    }
    else
    {
        // First transaction of this type, save it in an empty slot
        if (sHistory[0].logEventId == 0)
            history = &sHistory[0];
        else
            history = &sHistory[1];
        history->logEventId = logEventId;
    }

    // Set flag if this isn't the first time we've bought/sold in this session
    if (history->lastItemId != ITEM_NONE)
        history->hasMultipleTransactions = TRUE;

    history->lastItemId = itemId;

    // Add to number of items bought/sold
    if (history->itemQuantity < 999)
    {
        history->itemQuantity += quantity;
        if (history->itemQuantity > 999)
            history->itemQuantity = 999;
    }

    // Add to amount of money spent buying or earned selling
    if (history->totalMoney < 999999)
    {
        ListMenuGetCurrentItemArrayId(tListTaskId, &index);
        // logEventId will either be 1 (bought) or 2 (sold)
        // so for buying it will add the full price and selling will add half price
        history->totalMoney += (GetItemPrice(index) >> (logEventId - 1)) * quantity;
        if (history->totalMoney > 999999)
            history->totalMoney = 999999;
    }
}

// Will record QL_EVENT_BOUGHT_ITEM and/or QL_EVENT_SOLD_ITEM, or nothing.
static void RecordTransactionForQuestLog(void)
{
    u16 eventId = sHistory[0].logEventId;
    if (eventId != 0)
        SetQuestLogEvent(eventId + QL_EVENT_USED_POKEMART, (const u16 *)&sHistory[0]);

    eventId = sHistory[1].logEventId;
    if (eventId != 0)
        SetQuestLogEvent(eventId + QL_EVENT_USED_POKEMART, (const u16 *)&sHistory[1]);
}

void CreatePokemartMenu(u8 shopId)
{
    SetShopItemsForSale(shopId);
    CreateShopMenu(MART_TYPE_REGULAR);
    SetShopMenuCallback(ScriptContext_Enable);
    DebugFunc_PrintShopMenuHistoryBeforeClearMaybe();
    memset(&sHistory, 0, sizeof(sHistory));
    sHistory[0].mapSec = gMapHeader.regionMapSectionId;
    sHistory[1].mapSec = gMapHeader.regionMapSectionId;
}

void CreateCoinPokemartMenu(u8 shopId)
{
    SetShopItemsForSale(shopId);
    CreateShopMenu(MART_TYPE_COIN);
    SetShopMenuCallback(ScriptContext_Enable);
    DebugFunc_PrintShopMenuHistoryBeforeClearMaybe();
    memset(&sHistory, 0, sizeof(sHistory));
    sHistory[0].mapSec = gMapHeader.regionMapSectionId;
    sHistory[1].mapSec = gMapHeader.regionMapSectionId;
}

void CreateDecorationShop1Menu(u8 shopId)
{
    SetShopItemsForSale(shopId);
    CreateShopMenu(MART_TYPE_DECOR);
    SetShopMenuCallback(ScriptContext_Enable);
}

void CreateDecorationShop2Menu(u8 shopId)
{
    SetShopItemsForSale(shopId);
    CreateShopMenu(MART_TYPE_DECOR2);
    SetShopMenuCallback(ScriptContext_Enable);
}

