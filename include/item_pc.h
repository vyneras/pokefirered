#ifndef GUARD_ITEM_PC_H
#define GUARD_ITEM_PC_H

#include "constants/items.h"

void ItemPc_Init(u8 kind, MainCallback callback);
void ItemPc_SetInitializedFlag(bool8 flag);

static const u16 sArchipelagoPCItemId = ITEM_POTION;

#endif //GUARD_ITEM_PC_H
