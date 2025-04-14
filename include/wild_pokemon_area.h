#ifndef GUARD_WILD_POKEMON_AREA_H
#define GUARD_WILD_POKEMON_AREA_H

#define NUM_MAX_PAGES 20
#define INFO_BUFFER_LENGTH 300

struct EncounterAreaInfo
{
    u8 infoPages[NUM_MAX_PAGES][INFO_BUFFER_LENGTH];
    u8 currentPage;
    u8 totalPages;
};

s32 GetSpeciesPokedexAreaMarkers(u16 species, struct Subsprite * subsprites);
void GetSpeciesPokedexAreaInfo(u16 species, struct EncounterAreaInfo * areaInfo);

#endif //GUARD_WILD_POKEMON_AREA_H
