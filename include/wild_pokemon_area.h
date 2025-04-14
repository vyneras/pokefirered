#ifndef GUARD_WILD_POKEMON_AREA_H
#define GUARD_WILD_POKEMON_AREA_H

#include "pokedex_screen.h"

s32 GetSpeciesPokedexAreaMarkers(u16 species, struct Subsprite * subsprites);
void GetSpeciesPokedexAreaInfo(u16 species, struct EncounterAreaInfo * areaInfo);

#endif //GUARD_WILD_POKEMON_AREA_H
