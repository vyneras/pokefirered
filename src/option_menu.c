#include "global.h"
#include "gflib.h"
#include "event_data.h"
#include "scanline_effect.h"
#include "text.h"
#include "text_window_graphics.h"
#include "menu.h"
#include "task.h"
#include "overworld.h"
#include "help_system.h"
#include "text_window.h"
#include "strings.h"
#include "string_util.h"
#include "field_fadetransition.h"
#include "gba/m4a_internal.h"

#define PAGE_COUNT 4

// can't include the one in menu_helpers.h since Task_OptionMenu needs bool32 for matching
bool32 IsActiveOverworldLinkBusy(void);

// Menu items
enum
{
    MENUITEM_TEXTSPEED = 0,
    MENUITEM_TURBOA,
    MENUITEM_AUTORUN,
    MENUITEM_BUTTONMODE,
    MENUITEM_FRAMETYPE,
    MENUITEM_COUNT
};

enum
{
    MENUITEM_BATTLESCENE = 0,
    MENUITEM_BATTLESTYLE,
    MENUITEM_SHOWEFFECTIVENESS,
    MENUITEM_EXPERIENCE,
    MENUITEM_EXPERIENCE_DISTRIBUTION,
    MENUITEM_COUNT2
};

enum
{
    MENUITEM_SOUND = 0,
    MENUITEM_LOWHPBEEP,
    MENUITEM_SKIPFANFARES,
    MENUITEM_BIKEMUSIC,
    MENUITEM_SURFMUSIC,
    MENUITEM_COUNT3
};

enum
{
    MENUITEM_GUARANTEEDCATCH = 0,
    MENUITEM_GUARANTEEDRUN,
    MENUITEM_ENCOUNTERRATES,
    MENUITEM_ENCOUNTERMODE,
    MENUITEM_BLINDTRAINERS,
    MENUITEM_SKIPNICKNAMES,
    MENUITEM_ITEMMESSAGES,
    MENUITEM_COUNT4
};

// Window Ids
enum
{
    WIN_TEXT_OPTION,
    WIN_OPTIONS
};

// RAM symbols
struct OptionMenu
{
    /*0x00*/ u16 generalOptions[MENUITEM_COUNT];
    /*0x08*/ u16 battleOptions[MENUITEM_COUNT2];
    /*0x10*/ u16 soundOptions[MENUITEM_COUNT3];
    /*0x1A*/ u16 qualityOptions[MENUITEM_COUNT4];
    /*0x22*/ u16 cursorPos;
    /*0x24*/ u8 loadState;
    /*0x25*/ u8 state;
    /*0x26*/ u8 loadPaletteState;
    /*0x27*/ u8 currentPage;
};

static EWRAM_DATA struct OptionMenu *sOptionMenuPtr = NULL;

//Function Declarataions
static void CB2_InitOptionMenu(void);
static void VBlankCB_OptionMenu(void);
static void OptionMenu_InitCallbacks(void);
static void OptionMenu_SetVBlankCallback(void);
static void CB2_OptionMenu(void);
static void SetOptionMenuTask(void);
static void InitOptionMenuBg(void);
static void OptionMenu_Page(void);
static void OptionMenu_ResetSpriteData(void);
static bool8 LoadOptionMenuPalette(void);
static void Task_OptionMenu(u8 taskId);
static u8 OptionMenu_ProcessInput(void);
static void BufferOptionMenuString(u8 selection);
static void CloseAndSaveOptionMenu(u8 taskId);
static void PrintOptionMenuHeader(void);
static void DrawOptionMenuBg(void);
static void LoadOptionMenuItemNames(void);
static void UpdateSettingSelectionDisplay(u16 selection);

// Data Definitions
static const struct WindowTemplate sOptionMenuWinTemplates[] =
{
    {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 3,
        .width = 26,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 7,
        .width = 26,
        .height = 12,
        .paletteNum = 1,
        .baseBlock = 0x36
    },
    {
        .bg = 2,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x16e
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
   {
       .bg = 1,
       .charBaseIndex = 1,
       .mapBaseIndex = 30,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 0,
       .baseTile = 0
   },
   {
       .bg = 0,
       .charBaseIndex = 1,
       .mapBaseIndex = 31,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
       .baseTile = 0
   },
   {
       .bg = 2,
       .charBaseIndex = 1,
       .mapBaseIndex = 29,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 2,
       .baseTile = 0
   },
};

static const u16 sOptionMenuPalette[] = INCBIN_U16("graphics/misc/option_menu.gbapal");
static const u16 sOptionMenuItemCounts[MENUITEM_COUNT] = {4, 4, 2, 3, 10};
static const u16 sOptionMenu2ItemCounts[MENUITEM_COUNT2] = {2, 2, 2, 1001, 3};
static const u16 sOptionMenu3ItemCounts[MENUITEM_COUNT3] = {2, 2, 2, 2, 2};
static const u16 sOptionMenu4ItemCounts[MENUITEM_COUNT4] = {2, 2, 2, 3, 2, 2, 3};

static const u8 *const sOptionMenuItemsNames[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]   = gText_TextSpeed,
    [MENUITEM_TURBOA]      = gText_TurboButton,
    [MENUITEM_AUTORUN]     = gText_AutoRun,
    [MENUITEM_BUTTONMODE]  = gText_ButtonMode,
    [MENUITEM_FRAMETYPE]   = gText_Frame
};

static const u8 *const sOptionMenu2ItemsNames[MENUITEM_COUNT2] =
{
    [MENUITEM_BATTLESCENE]             = gText_BattleScene,
    [MENUITEM_BATTLESTYLE]             = gText_BattleStyle,
    [MENUITEM_SHOWEFFECTIVENESS]       = gText_ShowEffectiveness,
    [MENUITEM_EXPERIENCE]              = gText_Experience,
    [MENUITEM_EXPERIENCE_DISTRIBUTION] = gText_ExperienceDistribution
};

static const u8 *const sOptionMenu3ItemsNames[MENUITEM_COUNT3] =
{
    [MENUITEM_SOUND]        = gText_Sound,
    [MENUITEM_LOWHPBEEP]    = gText_LowHPBeep,
    [MENUITEM_SKIPFANFARES] = gText_SkipFanfares,
    [MENUITEM_BIKEMUSIC]    = gText_BikeMusic,
    [MENUITEM_SURFMUSIC]    = gText_SurfMusic
};

static const u8 *const sOptionMenu4ItemsNames[MENUITEM_COUNT4] =
{
    [MENUITEM_GUARANTEEDCATCH] = gText_GuaranteedCatch,
    [MENUITEM_GUARANTEEDRUN]   = gText_GuaranteedRun,
    [MENUITEM_ENCOUNTERRATES]  = gText_EncounterRates,
    [MENUITEM_ENCOUNTERMODE]   = gText_EncounterMode,
    [MENUITEM_BLINDTRAINERS]   = gText_BlindTrainers,
    [MENUITEM_SKIPNICKNAMES]   = gText_SkipNicknames,
    [MENUITEM_ITEMMESSAGES]    = gText_ItemMessages
};

static const u8 *const sTextSpeedOptions[] =
{
    gText_TextSpeedSlow,
    gText_TextSpeedMid,
    gText_TextSpeedFast,
    gText_TextSpeedInstant
};

static const u8 *const sTurboAOptions[] =
{
    gText_BattleSceneOff,
    gText_TurboA,
    gText_TurboB,
    gText_TurboAB
};

static const u8 *const sAutoRunOptions[] =
{
    gText_BattleSceneOff,
    gText_BattleSceneOn
};

static const u8 *const sButtonTypeOptions[] =
{
    gText_ButtonTypeHelp,
	gText_ButtonTypeLR,
	gText_ButtonTypeLEqualsA
};

static const u8 *const sBattleSceneOptions[] =
{
    gText_BattleSceneOff,
    gText_BattleSceneOn
};

static const u8 *const sBattleStyleOptions[] =
{
    gText_BattleStyleShift,
    gText_BattleStyleSet
};

static const u8 *const sShowEffectivenessOptions[] =
{
    gText_BattleSceneOff,
    gText_BattleSceneOn
};

static const u8 *const sExperienceDistributionOptions[] =
{
    gText_ExperienceDistributionGen3,
    gText_ExperienceDistributionGen6,
    gText_ExperienceDistributionGen8
};

static const u8 *const sSoundOptions[] =
{
    gText_SoundMono,
    gText_SoundStereo
};

static const u8 *const sLowHPBeepOptions[] =
{
    gText_BattleSceneOff,
    gText_BattleSceneOn
};

static const u8 *const sSkipFanfaresOptions[] =
{
    gText_BattleSceneOff,
    gText_BattleSceneOn
};

static const u8 *const sBikeMusicOptions[] =
{
    gText_BattleSceneOff,
    gText_BattleSceneOn
};

static const u8 *const sSurfMusicOptions[] =
{
    gText_BattleSceneOff,
    gText_BattleSceneOn
};

static const u8 *const sGuaranteedCatchOptions[] =
{
    gText_BattleSceneOff,
    gText_BattleSceneOn
};

static const u8 *const sGuaranteedRunOptions[] =
{
    gText_BattleSceneOff,
    gText_BattleSceneOn
};

static const u8 *const sEncounterRatesOptions[] =
{
    gText_EncounterRatesVanilla,
    gText_EncounterRatesNormalized
};

static const u8 *const sEncounterModeOptions[] =
{
    gText_EncounterModeRandom,
    gText_EncounterModeBoost,
    gText_EncounterModeRotate
};

static const u8 *const sBlindTrainersOptions[] =
{
    gText_BattleSceneOff,
    gText_BattleSceneOn
};

static const u8 *const sSkipNicknamesOptions[] =
{
    gText_BattleSceneOff,
    gText_BattleSceneOn
};

static const u8 *const sItemMessagesOptions[] =
{
    gText_ItemMessagesAll,
    gText_ItemMessagesProgression,
    gText_ItemMessagesNone
};

static const u8 sOptionMenuPageTextColor[] = {TEXT_DYNAMIC_COLOR_6, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};
static const u8 sOptionMenuTextColor[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_RED, TEXT_COLOR_RED};

// Functions
static void CB2_InitOptionMenu(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_OptionMenu(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_OptionsMenuFromStartMenu(void)
{
    u8 i;
    
    if (gMain.savedCallback == NULL)
        gMain.savedCallback = CB2_ReturnToFieldWithOpenMenu;
    sOptionMenuPtr = AllocZeroed(sizeof(struct OptionMenu));
    sOptionMenuPtr->loadState = 0;
    sOptionMenuPtr->loadPaletteState = 0;
    sOptionMenuPtr->state = 0;
    sOptionMenuPtr->cursorPos = 0;
    sOptionMenuPtr->currentPage = 1;
    sOptionMenuPtr->generalOptions[MENUITEM_TEXTSPEED] = gSaveBlock2Ptr->optionsTextSpeed;
    sOptionMenuPtr->generalOptions[MENUITEM_TURBOA] = gSaveBlock2Ptr->optionsTurboA;
    sOptionMenuPtr->generalOptions[MENUITEM_AUTORUN] = gSaveBlock2Ptr->optionsAutoRun;
    sOptionMenuPtr->generalOptions[MENUITEM_BUTTONMODE] = gSaveBlock2Ptr->optionsButtonMode;
    sOptionMenuPtr->generalOptions[MENUITEM_FRAMETYPE] = gSaveBlock2Ptr->optionsWindowFrameType;
    sOptionMenuPtr->battleOptions[MENUITEM_BATTLESCENE] = gSaveBlock2Ptr->optionsBattleScene;
    sOptionMenuPtr->battleOptions[MENUITEM_BATTLESTYLE] = gSaveBlock2Ptr->optionsBattleStyle;
    sOptionMenuPtr->battleOptions[MENUITEM_SHOWEFFECTIVENESS] = gSaveBlock2Ptr->optionsShowEffectiveness;
    sOptionMenuPtr->battleOptions[MENUITEM_EXPERIENCE] = gSaveBlock2Ptr->optionsExpMultiplier;
    sOptionMenuPtr->battleOptions[MENUITEM_EXPERIENCE_DISTRIBUTION] = gSaveBlock2Ptr->optionsExpDistribution;
    sOptionMenuPtr->soundOptions[MENUITEM_SOUND] = gSaveBlock2Ptr->optionsSound;
    sOptionMenuPtr->soundOptions[MENUITEM_LOWHPBEEP] = gSaveBlock2Ptr->optionsLowHPBeep;
    sOptionMenuPtr->soundOptions[MENUITEM_SKIPFANFARES] = gSaveBlock2Ptr->optionsSkipFanfares;
    sOptionMenuPtr->soundOptions[MENUITEM_BIKEMUSIC] = gSaveBlock2Ptr->optionsBikeMusic;
    sOptionMenuPtr->soundOptions[MENUITEM_SURFMUSIC] = gSaveBlock2Ptr->optionsSurfMusic;
    sOptionMenuPtr->qualityOptions[MENUITEM_GUARANTEEDCATCH] = gSaveBlock2Ptr->optionsGuaranteedCatch;
    sOptionMenuPtr->qualityOptions[MENUITEM_GUARANTEEDRUN] = gSaveBlock2Ptr->optionsGuaranteedRun;
    sOptionMenuPtr->qualityOptions[MENUITEM_ENCOUNTERRATES] = gSaveBlock2Ptr->optionsEncounterRates;
    sOptionMenuPtr->qualityOptions[MENUITEM_ENCOUNTERMODE] = gSaveBlock2Ptr->optionsEncounterMode;
    sOptionMenuPtr->qualityOptions[MENUITEM_BLINDTRAINERS] = gSaveBlock2Ptr->optionsBlindTrainers;
    sOptionMenuPtr->qualityOptions[MENUITEM_SKIPNICKNAMES] = gSaveBlock2Ptr->optionsSkipNicknames;
    sOptionMenuPtr->qualityOptions[MENUITEM_ITEMMESSAGES] = gSaveBlock2Ptr->optionsItemMessages;
    
    for (i = 0; i < MENUITEM_COUNT - 1; i++)
    {
        if (sOptionMenuPtr->generalOptions[i] > (sOptionMenuItemCounts[i]) - 1)
            sOptionMenuPtr->generalOptions[i] = 0;
    }
    for (i = 0; i < MENUITEM_COUNT2 - 1; i++)
    {
        if (sOptionMenuPtr->battleOptions[i] > (sOptionMenu2ItemCounts[i]) - 1)
            sOptionMenuPtr->battleOptions[i] = 0;
    }
    for (i = 0; i < MENUITEM_COUNT3 - 1; i++)
    {
        if (sOptionMenuPtr->soundOptions[i] > (sOptionMenu3ItemCounts[i]) - 1)
            sOptionMenuPtr->soundOptions[i] = 0;
    }
    for (i = 0; i < MENUITEM_COUNT4 - 1; i++)
    {
        if (sOptionMenuPtr->qualityOptions[i] > (sOptionMenu4ItemCounts[i]) - 1)
            sOptionMenuPtr->qualityOptions[i] = 0;
    }
    FlagSet(FLAG_SYS_IN_OPTIONS_MENU);
    SetHelpContext(HELPCONTEXT_OPTIONS);
    SetMainCallback2(CB2_OptionMenu);
}

static void OptionMenu_InitCallbacks(void)
{
    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
}

static void OptionMenu_SetVBlankCallback(void)
{
    SetVBlankCallback(VBlankCB_OptionMenu);
}

static void CB2_OptionMenu(void)
{
    u8 i, state;
    state = sOptionMenuPtr->state;
    switch (state)
    {
    case 0:
        OptionMenu_InitCallbacks();
        break;
    case 1:
        InitOptionMenuBg();
        break;
    case 2:
        OptionMenu_ResetSpriteData();
        break;
    case 3:
        if (LoadOptionMenuPalette() != TRUE)
            return;
        break;
    case 4:
        PrintOptionMenuHeader();
        break;
    case 5:
        DrawOptionMenuBg();
        break;
    case 6:
        LoadOptionMenuItemNames();
        break;
    case 7:
        switch (sOptionMenuPtr->currentPage)
        {
        case 1:
           for (i = 0; i < MENUITEM_COUNT; i++)
                BufferOptionMenuString(i);
           break;
        case 2:
           for (i = 0; i < MENUITEM_COUNT2; i++)
                BufferOptionMenuString(i);
           break;
        case 3:
           for (i = 0; i < MENUITEM_COUNT3; i++)
                BufferOptionMenuString(i);
           break;
        case 4:
           for (i = 0; i < MENUITEM_COUNT4; i++)
                BufferOptionMenuString(i);
           break;
        }
        break;
    case 8:
        UpdateSettingSelectionDisplay(sOptionMenuPtr->cursorPos);
        break;
    case 9:
        OptionMenu_Page();
        break;
    default:
        SetOptionMenuTask();
		break;
    }
    sOptionMenuPtr->state++;
}

static void SetOptionMenuTask(void)
{
    CreateTask(Task_OptionMenu, 0);
    SetMainCallback2(CB2_InitOptionMenu);
}

static void InitOptionMenuBg(void)
{
    void *dest = (void *)VRAM;
    DmaClearLarge16(3, dest, VRAM_SIZE, 0x1000);    
    DmaClear32(3, (void *)OAM, OAM_SIZE);
    DmaClear16(3, (void *)PLTT, PLTT_SIZE);    
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sOptionMenuBgTemplates, NELEMS(sOptionMenuBgTemplates));
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);
    InitWindows(sOptionMenuWinTemplates);
    DeactivateAllTextPrinters();
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_EFFECT_LIGHTEN);
    SetGpuReg(REG_OFFSET_BLDY, BLDCNT_TGT1_BG1);
    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_CLR);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
};

static void OptionMenu_Page(void)
{
    s32 x;
    x = 0xE4;
    FillWindowPixelBuffer(2, PIXEL_FILL(15));
    switch (sOptionMenuPtr->currentPage)
    {
    case 1:
        x -= GetStringWidth(FONT_SMALL, gText_OptionPage1, 0);
        AddTextPrinterParameterized3(2, FONT_SMALL, x, 0, sOptionMenuPageTextColor, 0, gText_OptionPage1);
        break;
    case 2:
        x -= GetStringWidth(FONT_SMALL, gText_OptionPage2, 0);
        AddTextPrinterParameterized3(2, FONT_SMALL, x, 0, sOptionMenuPageTextColor, 0, gText_OptionPage2);
        break;
    case 3:
        x -= GetStringWidth(FONT_SMALL, gText_OptionPage3, 0);
        AddTextPrinterParameterized3(2, FONT_SMALL, x, 0, sOptionMenuPageTextColor, 0, gText_OptionPage3);
        break;
    case 4:
        x -= GetStringWidth(FONT_SMALL, gText_OptionPage4, 0);
        AddTextPrinterParameterized3(2, FONT_SMALL, x, 0, sOptionMenuPageTextColor, 0, gText_OptionPage4);
        break;
    }
    PutWindowTilemap(2);
    CopyWindowToVram(2, COPYWIN_FULL);
}

static void OptionMenu_ResetSpriteData(void)
{
    ResetSpriteData();
    ResetPaletteFade();
    FreeAllSpritePalettes();
    ResetTasks();
    ScanlineEffect_Stop();
}

static bool8 LoadOptionMenuPalette(void)
{
    switch (sOptionMenuPtr->loadPaletteState)
    {
    case 0:
        LoadBgTiles(1, GetUserWindowGraphics(sOptionMenuPtr->generalOptions[MENUITEM_FRAMETYPE])->tiles, 0x120, 0x1AA);
        break;
    case 1:
        LoadPalette(GetUserWindowGraphics(sOptionMenuPtr->generalOptions[MENUITEM_FRAMETYPE])->palette, BG_PLTT_ID(2), PLTT_SIZE_4BPP);
        break;
    case 2:
        LoadPalette(sOptionMenuPalette, BG_PLTT_ID(1), sizeof(sOptionMenuPalette));
        LoadPalette(GetTextWindowPalette(2), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        break;
    case 3:
        LoadStdWindowGfxOnBg(1, 0x1B3, BG_PLTT_ID(3));
        break;
    default:
        return TRUE;
    }
    sOptionMenuPtr->loadPaletteState++;
    return FALSE;
}

static void Task_OptionMenu(u8 taskId)
{
    switch (sOptionMenuPtr->loadState)
    {
    case 0:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        OptionMenu_SetVBlankCallback();
        sOptionMenuPtr->loadState++;
        break;
    case 1:
        if (gPaletteFade.active)
            return;
        sOptionMenuPtr->loadState++;
        break;
    case 2:
        if (((bool32)IsActiveOverworldLinkBusy()) == TRUE)
            break;
        switch (OptionMenu_ProcessInput())
        {
        case 0:
            break;
        case 1:
            sOptionMenuPtr->loadState++;
            break;
        case 2:
            LoadBgTiles(1, GetUserWindowGraphics(sOptionMenuPtr->generalOptions[MENUITEM_FRAMETYPE])->tiles, 0x120, 0x1AA);
            LoadPalette(GetUserWindowGraphics(sOptionMenuPtr->generalOptions[MENUITEM_FRAMETYPE])->palette, BG_PLTT_ID(2), PLTT_SIZE_4BPP);
            BufferOptionMenuString(sOptionMenuPtr->cursorPos);
            break;
        case 3:
            UpdateSettingSelectionDisplay(sOptionMenuPtr->cursorPos);
            break;
        case 4:
            BufferOptionMenuString(sOptionMenuPtr->cursorPos);
            break;
        case 5:
            if (sOptionMenuPtr->currentPage == 4)
            {
                sOptionMenuPtr->currentPage = 1;
            }
            else
            {
                sOptionMenuPtr->currentPage++;
            }
            PrintOptionMenuHeader();
            sOptionMenuPtr->state = 6;
            sOptionMenuPtr->loadState = 1;
            sOptionMenuPtr->cursorPos = 0;
            DestroyTask(taskId);
            SetMainCallback2(CB2_OptionMenu);
            break;
        case 6:
            if (sOptionMenuPtr->currentPage == 1)
            {
                sOptionMenuPtr->currentPage = 4;
            }
            else
            {
                sOptionMenuPtr->currentPage--;
            }
            PrintOptionMenuHeader();
            sOptionMenuPtr->state = 6;
            sOptionMenuPtr->loadState = 1;
            sOptionMenuPtr->cursorPos = 0;
            DestroyTask(taskId);
            SetMainCallback2(CB2_OptionMenu);
            break;
        }
        break;
    case 3:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        sOptionMenuPtr->loadState++;
        break;
    case 4:
        if (gPaletteFade.active)
            return;
        sOptionMenuPtr->loadState++;
        break;
    case 5:
        CloseAndSaveOptionMenu(taskId);
        break;
    }
}

static u8 OptionMenu_ProcessInput(void)
{ 
    u16 current;
    u16 *curr;
    if (JOY_REPT(DPAD_RIGHT))
    {
        switch (sOptionMenuPtr->currentPage)
        {
        case 1:
            current = sOptionMenuPtr->generalOptions[sOptionMenuPtr->cursorPos];
            if (current == (sOptionMenuItemCounts[sOptionMenuPtr->cursorPos] - 1))
                sOptionMenuPtr->generalOptions[sOptionMenuPtr->cursorPos] = 0;
            else
                sOptionMenuPtr->generalOptions[sOptionMenuPtr->cursorPos] = current + 1;
            if (sOptionMenuPtr->cursorPos == MENUITEM_FRAMETYPE)
                return 2;
            else
                return 4;
        case 2:
            current = sOptionMenuPtr->battleOptions[sOptionMenuPtr->cursorPos];
            if (current == (sOptionMenu2ItemCounts[sOptionMenuPtr->cursorPos] - 1))
                sOptionMenuPtr->battleOptions[sOptionMenuPtr->cursorPos] = 0;
            else if (sOptionMenuPtr->cursorPos == MENUITEM_EXPERIENCE)
                sOptionMenuPtr->battleOptions[sOptionMenuPtr->cursorPos] = current + 10;
            else
                sOptionMenuPtr->battleOptions[sOptionMenuPtr->cursorPos] = current + 1;
            return 4;
        case 3:
            current = sOptionMenuPtr->soundOptions[sOptionMenuPtr->cursorPos];
            if (current == (sOptionMenu3ItemCounts[sOptionMenuPtr->cursorPos] - 1))
                sOptionMenuPtr->soundOptions[sOptionMenuPtr->cursorPos] = 0;
            else
                sOptionMenuPtr->soundOptions[sOptionMenuPtr->cursorPos] = current + 1;
            return 4;
        case 4:
            current = sOptionMenuPtr->qualityOptions[sOptionMenuPtr->cursorPos];
            if (current == (sOptionMenu4ItemCounts[sOptionMenuPtr->cursorPos] - 1))
                sOptionMenuPtr->qualityOptions[sOptionMenuPtr->cursorPos] = 0;
            else
                sOptionMenuPtr->qualityOptions[sOptionMenuPtr->cursorPos] = current + 1;
            return 4;
        }
    }
    else if (JOY_REPT(DPAD_LEFT))
    {
        switch (sOptionMenuPtr->currentPage)
        {
        case 1:
            current = sOptionMenuPtr->generalOptions[sOptionMenuPtr->cursorPos];
            if (current == 0)
                sOptionMenuPtr->generalOptions[sOptionMenuPtr->cursorPos] = sOptionMenuItemCounts[sOptionMenuPtr->cursorPos] - 1;
            else
               sOptionMenuPtr->generalOptions[sOptionMenuPtr->cursorPos] = current - 1;
            if (sOptionMenuPtr->cursorPos == MENUITEM_FRAMETYPE)
                return 2;
            else
                return 4;
        case 2:
            current = sOptionMenuPtr->battleOptions[sOptionMenuPtr->cursorPos];
            if (current == 0)
                sOptionMenuPtr->battleOptions[sOptionMenuPtr->cursorPos] = sOptionMenu2ItemCounts[sOptionMenuPtr->cursorPos] - 1;
            else if (sOptionMenuPtr->cursorPos == MENUITEM_EXPERIENCE)
                sOptionMenuPtr->battleOptions[sOptionMenuPtr->cursorPos] = current - 10;
            else
                sOptionMenuPtr->battleOptions[sOptionMenuPtr->cursorPos] = current - 1;
            return 4;
        case 3:
            current = sOptionMenuPtr->soundOptions[sOptionMenuPtr->cursorPos];
            if (current == 0)
                sOptionMenuPtr->soundOptions[sOptionMenuPtr->cursorPos] = sOptionMenu3ItemCounts[sOptionMenuPtr->cursorPos] - 1;
            else
               sOptionMenuPtr->soundOptions[sOptionMenuPtr->cursorPos] = current - 1;
            return 4;
        case 4:
            current = sOptionMenuPtr->qualityOptions[sOptionMenuPtr->cursorPos];
            if (current == 0)
                sOptionMenuPtr->qualityOptions[sOptionMenuPtr->cursorPos] = sOptionMenu4ItemCounts[sOptionMenuPtr->cursorPos] - 1;
            else
               sOptionMenuPtr->qualityOptions[sOptionMenuPtr->cursorPos] = current - 1;
            return 4;
        }
    }
    else if (JOY_REPT(DPAD_UP))
    {
        if (sOptionMenuPtr->cursorPos == MENUITEM_TEXTSPEED && sOptionMenuPtr->currentPage == 1)
            sOptionMenuPtr->cursorPos = MENUITEM_FRAMETYPE;
        else if (sOptionMenuPtr->cursorPos == MENUITEM_BATTLESCENE && sOptionMenuPtr->currentPage == 2)
            sOptionMenuPtr->cursorPos = MENUITEM_EXPERIENCE_DISTRIBUTION;
        else if (sOptionMenuPtr->cursorPos == MENUITEM_SOUND && sOptionMenuPtr->currentPage == 3)
            sOptionMenuPtr->cursorPos = MENUITEM_SURFMUSIC;
        else if (sOptionMenuPtr->cursorPos == MENUITEM_GUARANTEEDCATCH && sOptionMenuPtr->currentPage == 4)
            sOptionMenuPtr->cursorPos = MENUITEM_ITEMMESSAGES;
        else
            sOptionMenuPtr->cursorPos = sOptionMenuPtr->cursorPos - 1;
        return 3;        
    }
    else if (JOY_REPT(DPAD_DOWN))
    {
        if (sOptionMenuPtr->cursorPos == MENUITEM_FRAMETYPE && sOptionMenuPtr->currentPage == 1)
            sOptionMenuPtr->cursorPos = MENUITEM_TEXTSPEED;
        else if (sOptionMenuPtr->cursorPos == MENUITEM_EXPERIENCE_DISTRIBUTION && sOptionMenuPtr->currentPage == 2)
            sOptionMenuPtr->cursorPos = MENUITEM_BATTLESCENE;
        else if (sOptionMenuPtr->cursorPos == MENUITEM_SURFMUSIC && sOptionMenuPtr->currentPage == 3)
            sOptionMenuPtr->cursorPos = MENUITEM_SOUND;
        else if (sOptionMenuPtr->cursorPos == MENUITEM_ITEMMESSAGES && sOptionMenuPtr->currentPage == 4)
            sOptionMenuPtr->cursorPos = MENUITEM_GUARANTEEDCATCH;
        else
            sOptionMenuPtr->cursorPos = sOptionMenuPtr->cursorPos + 1;
        return 3;
    }
    else if (JOY_NEW(R_BUTTON))
    {
        return 5;
    }
    else if (JOY_NEW(L_BUTTON))
    {
        return 6;
    }
    else if (JOY_NEW(B_BUTTON) || JOY_NEW(A_BUTTON))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void BufferOptionMenuString(u8 selection)
{
    u8 str[20];
    u8 buf[12];
    u8 dst[3];
    u8 x, y;
    
    memcpy(dst, sOptionMenuTextColor, 3);
    x = 0x82;
    y = ((GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT) - 1) * selection) + 2;
    FillWindowPixelRect(1, 1, x, y, 0x46, GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT));

    switch (sOptionMenuPtr->currentPage)
    {
    case 1:
        switch (selection)
        {
        case MENUITEM_TEXTSPEED:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sTextSpeedOptions[sOptionMenuPtr->generalOptions[selection]]);
            break;
        case MENUITEM_TURBOA:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sTurboAOptions[sOptionMenuPtr->generalOptions[selection]]);
            break;
        case MENUITEM_AUTORUN:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sAutoRunOptions[sOptionMenuPtr->generalOptions[selection]]);
            break;
        case MENUITEM_BUTTONMODE:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sButtonTypeOptions[sOptionMenuPtr->generalOptions[selection]]);
            break;
        case MENUITEM_FRAMETYPE:
            StringCopy(str, gText_FrameType);
            ConvertIntToDecimalStringN(buf, sOptionMenuPtr->generalOptions[selection] + 1, 1, 2);
            StringAppendN(str, buf, 3);
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, str);
            break;
        }
        break;
    case 2:
        switch (selection)
        {
        case MENUITEM_BATTLESCENE:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sBattleSceneOptions[sOptionMenuPtr->battleOptions[selection]]);
            break;
        case MENUITEM_BATTLESTYLE:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sBattleStyleOptions[sOptionMenuPtr->battleOptions[selection]]);
            break;
        case MENUITEM_SHOWEFFECTIVENESS:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sShowEffectivenessOptions[sOptionMenuPtr->battleOptions[selection]]);
            break;
        case MENUITEM_EXPERIENCE:
            ConvertIntToDecimalStringN(buf, sOptionMenuPtr->battleOptions[selection], 0, 4);
            StringCopyN(str, buf, 5);
            StringAppend(str, gText_Percent);
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, str);
            break;
        case MENUITEM_EXPERIENCE_DISTRIBUTION:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sExperienceDistributionOptions[sOptionMenuPtr->battleOptions[selection]]);
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (selection)
        {
        case MENUITEM_SOUND:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sSoundOptions[sOptionMenuPtr->soundOptions[selection]]);
            break;
        case MENUITEM_LOWHPBEEP:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sLowHPBeepOptions[sOptionMenuPtr->soundOptions[selection]]);
            break;
        case MENUITEM_SKIPFANFARES:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sSkipFanfaresOptions[sOptionMenuPtr->soundOptions[selection]]);
            break;
        case MENUITEM_BIKEMUSIC:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sBikeMusicOptions[sOptionMenuPtr->soundOptions[selection]]);
            break;
        case MENUITEM_SURFMUSIC:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sSurfMusicOptions[sOptionMenuPtr->soundOptions[selection]]);
            break;
        default:
            break;
        }
        break;
    case 4:
        switch (selection)
        {
        case MENUITEM_GUARANTEEDCATCH:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sGuaranteedCatchOptions[sOptionMenuPtr->qualityOptions[selection]]);
            break;
        case MENUITEM_GUARANTEEDRUN:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sGuaranteedRunOptions[sOptionMenuPtr->qualityOptions[selection]]);
            break;
        case MENUITEM_ENCOUNTERRATES:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sEncounterRatesOptions[sOptionMenuPtr->qualityOptions[selection]]);
            break;
        case MENUITEM_ENCOUNTERMODE:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sEncounterModeOptions[sOptionMenuPtr->qualityOptions[selection]]);
            break;
        case MENUITEM_BLINDTRAINERS:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sBlindTrainersOptions[sOptionMenuPtr->qualityOptions[selection]]);
            break;
        case MENUITEM_SKIPNICKNAMES:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sSkipNicknamesOptions[sOptionMenuPtr->qualityOptions[selection]]);
            break;
        case MENUITEM_ITEMMESSAGES:
            AddTextPrinterParameterized3(1, FONT_NORMAL, x, y, dst, -1, sItemMessagesOptions[sOptionMenuPtr->qualityOptions[selection]]);
            break;
        default:
            break;
        }
        break;
    }

    PutWindowTilemap(1);
    CopyWindowToVram(1, COPYWIN_FULL);
}

static void CloseAndSaveOptionMenu(u8 taskId)
{
    gFieldCallback = FieldCB_DefaultWarpExit;
    SetMainCallback2(gMain.savedCallback);
    FreeAllWindowBuffers();
    gSaveBlock2Ptr->optionsTextSpeed = sOptionMenuPtr->generalOptions[MENUITEM_TEXTSPEED];
    gSaveBlock2Ptr->optionsTurboA = sOptionMenuPtr->generalOptions[MENUITEM_TURBOA];
    gSaveBlock2Ptr->optionsAutoRun = sOptionMenuPtr->generalOptions[MENUITEM_AUTORUN];
    gSaveBlock2Ptr->optionsButtonMode = sOptionMenuPtr->generalOptions[MENUITEM_BUTTONMODE];
    gSaveBlock2Ptr->optionsWindowFrameType = sOptionMenuPtr->generalOptions[MENUITEM_FRAMETYPE];
    gSaveBlock2Ptr->optionsBattleScene = sOptionMenuPtr->battleOptions[MENUITEM_BATTLESCENE];
    gSaveBlock2Ptr->optionsBattleStyle = sOptionMenuPtr->battleOptions[MENUITEM_BATTLESTYLE];
    gSaveBlock2Ptr->optionsShowEffectiveness = sOptionMenuPtr->battleOptions[MENUITEM_SHOWEFFECTIVENESS];
    gSaveBlock2Ptr->optionsExpMultiplier = sOptionMenuPtr->battleOptions[MENUITEM_EXPERIENCE];
    gSaveBlock2Ptr->optionsExpDistribution = sOptionMenuPtr->battleOptions[MENUITEM_EXPERIENCE_DISTRIBUTION];
    gSaveBlock2Ptr->optionsSound = sOptionMenuPtr->soundOptions[MENUITEM_SOUND];
    gSaveBlock2Ptr->optionsLowHPBeep = sOptionMenuPtr->soundOptions[MENUITEM_LOWHPBEEP];
    gSaveBlock2Ptr->optionsSkipFanfares = sOptionMenuPtr->soundOptions[MENUITEM_SKIPFANFARES];
    gSaveBlock2Ptr->optionsBikeMusic = sOptionMenuPtr->soundOptions[MENUITEM_BIKEMUSIC];
    gSaveBlock2Ptr->optionsSurfMusic = sOptionMenuPtr->soundOptions[MENUITEM_SURFMUSIC];
    gSaveBlock2Ptr->optionsGuaranteedCatch = sOptionMenuPtr->qualityOptions[MENUITEM_GUARANTEEDCATCH];
    gSaveBlock2Ptr->optionsGuaranteedRun = sOptionMenuPtr->qualityOptions[MENUITEM_GUARANTEEDRUN];
    gSaveBlock2Ptr->optionsEncounterRates = sOptionMenuPtr->qualityOptions[MENUITEM_ENCOUNTERRATES];
    gSaveBlock2Ptr->optionsEncounterMode = sOptionMenuPtr->qualityOptions[MENUITEM_ENCOUNTERMODE];
    gSaveBlock2Ptr->optionsBlindTrainers = sOptionMenuPtr->qualityOptions[MENUITEM_BLINDTRAINERS];
    gSaveBlock2Ptr->optionsSkipNicknames = sOptionMenuPtr->qualityOptions[MENUITEM_SKIPNICKNAMES];
    gSaveBlock2Ptr->optionsItemMessages = sOptionMenuPtr->qualityOptions[MENUITEM_ITEMMESSAGES];
    SetPokemonCryStereo(gSaveBlock2Ptr->optionsSound);
    FREE_AND_SET_NULL(sOptionMenuPtr);
    FlagClear(FLAG_SYS_IN_OPTIONS_MENU);
    DestroyTask(taskId);
}

static void PrintOptionMenuHeader(void)
{
    FillWindowPixelBuffer(0, PIXEL_FILL(1));
    switch (sOptionMenuPtr->currentPage)
    {
    case 1:
        AddTextPrinterParameterized(WIN_TEXT_OPTION, FONT_NORMAL, gText_GeneralOptions, 8, 1, TEXT_SKIP_DRAW, NULL);
        break;
    case 2:
        AddTextPrinterParameterized(WIN_TEXT_OPTION, FONT_NORMAL, gText_BattleOptions, 8, 1, TEXT_SKIP_DRAW, NULL);
        break;
    case 3:
        AddTextPrinterParameterized(WIN_TEXT_OPTION, FONT_NORMAL, gText_SoundOptions, 8, 1, TEXT_SKIP_DRAW, NULL);
        break;
    case 4:
        AddTextPrinterParameterized(WIN_TEXT_OPTION, FONT_NORMAL, gText_QOLOptions, 8, 1, TEXT_SKIP_DRAW, NULL);
        break;
    }
    PutWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_FULL);
}

static void DrawOptionMenuBg(void)
{
    u8 h;
    h = 2;
    
    FillBgTilemapBufferRect(1, 0x1B3, 1, 2, 1, 1, 3);
    FillBgTilemapBufferRect(1, 0x1B4, 2, 2, 0x1B, 1, 3);
    FillBgTilemapBufferRect(1, 0x1B5, 0x1C, 2, 1, 1, 3);
    FillBgTilemapBufferRect(1, 0x1B6, 1, 3, 1, h, 3);
    FillBgTilemapBufferRect(1, 0x1B8, 0x1C, 3, 1, h, 3);
    FillBgTilemapBufferRect(1, 0x1B9, 1, 5, 1, 1, 3);
    FillBgTilemapBufferRect(1, 0x1BA, 2, 5, 0x1B, 1, 3);
    FillBgTilemapBufferRect(1, 0x1BB, 0x1C, 5, 1, 1, 3);
    FillBgTilemapBufferRect(1, 0x1AA, 1, 6, 1, 1, h);
    FillBgTilemapBufferRect(1, 0x1AB, 2, 6, 0x1A, 1, h);
    FillBgTilemapBufferRect(1, 0x1AC, 0x1C, 6, 1, 1, h);
    FillBgTilemapBufferRect(1, 0x1AD, 1, 7, 1, 0x10, h);
    FillBgTilemapBufferRect(1, 0x1AF, 0x1C, 7, 1, 0x10, h);
    FillBgTilemapBufferRect(1, 0x1B0, 1, 0x13, 1, 1, h);
    FillBgTilemapBufferRect(1, 0x1B1, 2, 0x13, 0x1A, 1, h);
    FillBgTilemapBufferRect(1, 0x1B2, 0x1C, 0x13, 1, 1, h);
    CopyBgTilemapBufferToVram(1);
}

static void LoadOptionMenuItemNames(void)
{
    u8 i;
    
    FillWindowPixelBuffer(1, PIXEL_FILL(1));
    switch (sOptionMenuPtr->currentPage)
    {
    case 1:
        for (i = 0; i < MENUITEM_COUNT; i++)
            AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, sOptionMenuItemsNames[i], 8, (u8)((i * (GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT))) + 2) - i, TEXT_SKIP_DRAW, NULL);
        break;
    case 2:
        for (i = 0; i < MENUITEM_COUNT2; i++)
            AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, sOptionMenu2ItemsNames[i], 8, (u8)((i * (GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT))) + 2) - i, TEXT_SKIP_DRAW, NULL);
        break;
    case 3:
        for (i = 0; i < MENUITEM_COUNT3; i++)
            AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, sOptionMenu3ItemsNames[i], 8, (u8)((i * (GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT))) + 2) - i, TEXT_SKIP_DRAW, NULL);
        break;
    case 4:
        for (i = 0; i < MENUITEM_COUNT4; i++)
            AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, sOptionMenu4ItemsNames[i], 8, (u8)((i * (GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT))) + 2) - i, TEXT_SKIP_DRAW, NULL);
        break;
    }
}

static void UpdateSettingSelectionDisplay(u16 selection)
{
    u16 maxLetterHeight, y;
    
    maxLetterHeight = GetFontAttribute(FONT_NORMAL, FONTATTR_MAX_LETTER_HEIGHT);
    y = selection * (maxLetterHeight - 1) + 0x3A;
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(y, y + maxLetterHeight));
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0x10, 0xE0));
}
