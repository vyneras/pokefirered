#ifndef GUARD_COINS_H
#define GUARD_COINS_H

#include "global.h"

void PrintCoinsString(u32 coinAmount);
void ShowCoinsWindow(u32 coinAmount, u8 x, u8 y);
void HideCoinsWindow(void);
u16 GetCoins(void);
void SetCoins(u16 coinAmount);
bool8 IsEnoughCoins(u16 cost);
bool8 AddCoins(u16 toAdd);
bool8 RemoveCoins(u16 toSub);
void PrintCoinsString_Parameterized(u8 windowId, u32 coinAmount, u8 x, u8 y, u8 speed);
void ShowCoinsWindow_Parameterized(u8 windowId, u16 tileStart, u8 palette, u32 coinAmount);
void PrintCoinAmount(u8 windowId, u8 x, u8 y, int amount, u8 speed);

#endif // GUARD_COINS_H
