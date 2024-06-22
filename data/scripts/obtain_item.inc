.set ITEM_ID, VAR_0x8000
.set ITEM_ID_2, VAR_0x8014
.set AMOUNT, VAR_0x8001
.set LOCATION_ID, VAR_0x8003

EventScript_RestorePrevTextColor::
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	return

Std_ObtainItem::
	call_if_ne LOCATION_ID, 0, EventScript_TrySetGiveItemFlag
	copyvar ITEM_ID_2, ITEM_ID
	specialvar VAR_RESULT, ArchipelagoSpecial_ShouldHandle
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_ObtainItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	additem ITEM_ID, AMOUNT
	call EventScript_ObtainItemMessage
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	return

EventScript_TrySetGiveItemFlag::
	call_if_lt LOCATION_ID, 10000, EventScript_SetGiveItemFlag
	return

EventScript_SetGiveItemFlag::
	setflagvar LOCATION_ID
	return

EventScript_ObtainItemMessage::
	bufferitemnameplural STR_VAR_2, ITEM_ID, AMOUNT
	checkitemtype ITEM_ID
	call EventScript_BufferPocketNameTryFanfare
	copyvar ITEM_ID, ITEM_ID_2  @ The switch in EventScript_BufferPocketNameAndTryFanfare clobbers VAR_0x8000
	call EventScript_ObtainedItem
	return

EventScript_BufferPocketNameTryFanfare::
	switch VAR_RESULT
	case POCKET_ITEMS, EventScript_BufferItemsPocket
	case POCKET_KEY_ITEMS, EventScript_BufferKeyItemsPocket
	case POCKET_POKE_BALLS, EventScript_BufferPokeBallsPocket
	case POCKET_TM_CASE, EventScript_BufferTMCase
	case POCKET_BERRY_POUCH, EventScript_BufferBerryPouch
	end

EventScript_BufferItemsPocket::
	bufferstdstring STR_VAR_3, STDSTRING_ITEMS_POCKET
	call EventScript_PlayFanfareObtainedItem
	return

EventScript_BufferKeyItemsPocket::
	bufferstdstring STR_VAR_3, STDSTRING_KEY_ITEMS_POCKET
	call EventScript_PlayFanfareObtainedItem
	return

EventScript_BufferPokeBallsPocket::
	bufferstdstring STR_VAR_3, STDSTRING_POKEBALLS_POCKET
	call EventScript_PlayFanfareObtainedItem
	return

EventScript_BufferTMCase::
	bufferstdstring STR_VAR_3, STDSTRING_TM_CASE
	call EventScript_PlayFanfareObtainedItem
	return

EventScript_BufferBerryPouch::
	bufferstdstring STR_VAR_3, STDSTRING_BERRY_POUCH
	call EventScript_PlayFanfareObtainedItem
	return

EventScript_ObtainedItem::
	copyvar VAR_0x8004, ITEM_ID
	specialvar VAR_RESULT, BufferTMHMMoveName
	goto_if_eq VAR_RESULT, TRUE, EventScript_ObtainedTMHM
	msgbox Text_ObtainedTheX, MSGBOX_DEFAULT
	waitfanfare
	setvar VAR_RESULT, TRUE
	return

EventScript_ObtainedTMHM::
	msgbox Text_ObtainedTheTMHM, MSGBOX_DEFAULT
	waitfanfare
	setvar VAR_RESULT, TRUE
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	return

EventScript_PlayFanfareObtainedItem::
	playfanfare MUS_LEVEL_UP
	return

EventScript_PlayFanfareObtainedTMHM::
	playfanfare MUS_LEVEL_UP
	return

Std_ObtainDecoration::
	adddecoration VAR_0x8000
	call EventScript_ObtainDecorMessage
	return

EventScript_ObtainDecorMessage::
	bufferdecorationname STR_VAR_2, VAR_0x8000
	call EventScript_ObtainedDecor
	return

EventScript_ObtainedDecor::
	playfanfare MUS_OBTAIN_ITEM
	message Text_ObtainedTheDecor
	waitfanfare
	waitmessage
	msgbox Text_DecorTransferredToPC
	setvar VAR_RESULT, TRUE
	return

Std_FindItem::
	lock
	faceplayer
	waitse
	copyvar VAR_0x8004, ITEM_ID
	copyvar ITEM_ID_2, ITEM_ID
	copyvar VAR_0x8005, AMOUNT
	specialvar LOCATION_ID, ArchipelagoSpecial_GetObjectScriptFlag
	specialvar VAR_RESULT, ArchipelagoSpecial_ShouldHandle
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_FindItem
	checkitemspace ITEM_ID, AMOUNT
	bufferitemname STR_VAR_2, ITEM_ID
	@ Dangerously remove the object immediately before checking for space.
	@ The removeobject function sets the flag, so this means found items
	@ will be sent to the server sooner (in line with archipelago items and gifts).
	@ Bag capacity should be more than large enough to allow the player to carry
	@ all the items in the game. Player would need to intentionally overfill their
	@ bag to run out of space.
	removeobject VAR_LAST_TALKED
	checkitemtype VAR_0x8000
	call EventScript_BufferPocketNameTryFanfare
	call EventScript_PickUpItem
	release
	return

EventScript_PickUpItem::
	additem VAR_0x8004, VAR_0x8005
	specialvar VAR_RESULT, BufferTMHMMoveName
	copyvar VAR_0x8008, VAR_RESULT
	call_if_eq VAR_0x8008, TRUE, EventScript_FoundTMHM
	call_if_eq VAR_0x8008, FALSE, EventScript_FoundItem
	waitfanfare
	waitmessage
	return

EventScript_FoundTMHM::
	message Text_FoundTMHMContainsMove
	return

EventScript_FoundItem::
	message Text_FoundOneItem
	return

EventScript_HiddenItemScript::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	waitse
	goto_if_eq VAR_0x8005, 0, EventScript_TryPickUpHiddenCoins
	copyvar LOCATION_ID, VAR_0x8004
	copyvar ITEM_ID_2, VAR_0x8005
	specialvar VAR_RESULT, ArchipelagoSpecial_ShouldHandle
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_FindHiddenItem
	call EventScript_TryPickUpHiddenItem
	goto EventScript_PickedUpHiddenItem
	end

EventScript_TryPickUpHiddenItem::
	additem VAR_0x8005, VAR_0x8006
	bufferitemname STR_VAR_2, VAR_0x8005
	checkitemtype VAR_0x8005
	@ Dangerously set the item flag before checking for space.
	@ Items will be sent to the server sooner (in line with archipelago items and gifts).
	@ Bag capacity should be more than large enough to allow the player to carry
	@ all the items in the game. Player would need to intentionally overfill their
	@ bag to run out of space.
	special SetHiddenItemFlag
	call EventScript_BufferPocketNameTryFanfare
	return

EventScript_PickedUpHiddenItem::
	call_if_eq VAR_0x8006, 1, EventScript_FoundSingleItem
	call_if_ne VAR_0x8006, 1, EventScript_FoundMultipleItems
	waitfanfare
	waitmessage
	releaseall
	end

EventScript_FoundSingleItem::
	copyvar VAR_0x8008, VAR_0x8004
	copyvar VAR_0x8004, VAR_0x8005
	specialvar VAR_RESULT, BufferTMHMMoveName
	call_if_eq VAR_RESULT, TRUE, EventScript_FoundTMHM
	call_if_eq VAR_RESULT, FALSE, EventScript_FoundItem
	return

EventScript_FoundCoins::
	buffernumberstring STR_VAR_1, VAR_0x8006
	message Text_FoundXCoins
	return

EventScript_FoundMultipleItems::
	buffernumberstring STR_VAR_1, VAR_0x8006
	message Text_FoundXItems
	return

EventScript_TryPickUpHiddenCoins::
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, EventScript_NoCaseForHiddenCoins
	checkcoins VAR_RESULT
	specialvar VAR_RESULT, CheckAddCoins
	goto_if_eq VAR_RESULT, FALSE, EventScript_HiddenCoinsButCaseIsFull
	addcoins VAR_0x8006
	bufferstdstring STR_VAR_2, STDSTRING_COINS
	call EventScript_PlayFanfareObtainedItem
	call EventScript_FoundCoins
	waitfanfare
	waitmessage
	msgbox Text_PutCoinsAwayInCoinCase
	special SetHiddenItemFlag
	releaseall
	end

EventScript_HiddenCoinsButCaseIsFull::
	buffernumberstring STR_VAR_1, VAR_0x8006
	bufferstdstring STR_VAR_2, STDSTRING_COINS
	msgbox Text_FoundXCoins
	msgbox Text_CoinCaseIsFull
	setvar VAR_RESULT, 0
	releaseall
	end

EventScript_NoCaseForHiddenCoins::
	buffernumberstring STR_VAR_1, VAR_0x8006
	bufferstdstring STR_VAR_2, STDSTRING_COINS
	msgbox Text_FoundXCoins
	msgbox Text_NothingToPutThemIn
	setvar VAR_RESULT, 0
	releaseall
	end
