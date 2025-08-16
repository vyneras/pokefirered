#ifndef GUARD_POKEDEX_SCREEN_H
#define GUARD_POKEDEX_SCREEN_H

#define DEX_CATEGORY_GRASSLAND      0
#define DEX_CATEGORY_FOREST         1
#define DEX_CATEGORY_WATERS_EDGE    2
#define DEX_CATEGORY_SEA            3
#define DEX_CATEGORY_CAVE           4
#define DEX_CATEGORY_MOUNTAIN       5
#define DEX_CATEGORY_ROUGH_TERRAIN  6
#define DEX_CATEGORY_URBAN          7
#define DEX_CATEGORY_RARE           8
#define DEX_CATEGORY_COUNT          9

#define DEX_ORDER_NUMERICAL_KANTO     0
#define DEX_ORDER_ATOZ                1
#define DEX_ORDER_TYPE                2
#define DEX_ORDER_LIGHTEST            3
#define DEX_ORDER_SMALLEST            4
#define DEX_ORDER_NUMERICAL_NATIONAL  5
#define DEX_ORDER_NUMERICAL_DEXSANITY 6

#define DEX_MODE(name) (DEX_CATEGORY_COUNT + DEX_ORDER_##name)

#define NUM_MAX_AREA_PAGES 15
#define NUM_MAX_EVOLUTION_PAGES 3
#define INFO_BUFFER_LENGTH 200


#include "pokedex.h"

struct EncounterAreaInfo
{
    u8 infoPages[NUM_MAX_AREA_PAGES][INFO_BUFFER_LENGTH];
    u8 currentPage;
    u8 totalPages;
};

struct EvolutionInfo
{
   u8 infoPages[NUM_MAX_EVOLUTION_PAGES][INFO_BUFFER_LENGTH];
    u8 currentPage;
    u8 totalPages;
};

extern const struct PokedexEntry gPokedexEntries[];

void CB2_OpenPokedexFromStartMenu(void);
s8 DexScreen_GetSetPokedexFlag(u16 nationalDexNo, u8 caseId, bool8 indexIsSpecies);
bool8 HasDexsanityItem(u16 nationalDexNo);
bool8 GiveDexsanityItems(void);

#endif //GUARD_POKEDEX_SCREEN_H
