.set ITEM_ID, VAR_0x8000
.set AMOUNT, VAR_0x8001
.set LOCATION_ID, VAR_0x8003

EventScript_RestorePrevTextColor::
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	return

Std_ObtainItem::
	call_if_ne LOCATION_ID, 0, EventScript_TrySetGiveItemFlag
	specialvar VAR_RESULT, ArchipelagoSpecial_ShouldHandle
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_ObtainItem
	textcolor NPC_TEXT_COLOR_NEUTRAL
	call EventScript_ObtainItem
	additem ITEM_ID, AMOUNT
	call EventScript_RestorePrevTextColor
	return

EventScript_TrySetGiveItemFlag::
	call_if_lt LOCATION_ID, FLAGS_COUNT, EventScript_SetGiveItemFlag
	return

EventScript_SetGiveItemFlag::
	setflagvar LOCATION_ID
	return

EventScript_ObtainItem::
	bufferitemname STR_VAR_2, ITEM_ID
	call EventScript_PlayItemFanfare
	call EventScript_ObtainItemMessage
	waitfanfare
	setvar VAR_RESULT, TRUE
	return

EventScript_PlayItemFanfare::
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemBadge
	goto_if_eq VAR_RESULT, TRUE, EventScript_PlayGymBadgeFanfare
	goto EventScript_PlayNormalItemFanfare
	end

EventScript_PlayGymBadgeFanfare::
	playfanfare MUS_OBTAIN_BADGE
	return

EventScript_PlayNormalItemFanfare::
	playfanfare MUS_LEVEL_UP
	return

EventScript_ObtainItemMessage::
	specialvar VAR_RESULT, BufferTMHMMoveName
	goto_if_eq VAR_RESULT, TRUE, EventScript_ObtainedTMHM
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemBadge
	goto_if_eq VAR_RESULT, TRUE, EventScript_ObtainedBadge
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemFlyUnlock
	goto_if_eq VAR_RESULT, TRUE, EventScript_ObtainedFlyUnlock
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemCoins
	goto_if_eq VAR_RESULT, TRUE, EventScript_ObtainedCoins
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemLedgeJump
	goto_if_eq VAR_RESULT, TRUE, EventScript_ObtainedLedgeJump
	msgbox Text_ObtainedItem, MSGBOX_DEFAULT
	return

EventScript_ObtainedTMHM::
	msgbox Text_ObtainedTMHM, MSGBOX_DEFAULT
	return

EventScript_ObtainedBadge::
	msgbox Text_ObtainedBadge, MSGBOX_DEFAULT
	return

EventScript_ObtainedFlyUnlock::
	bufferuniqueitemname STR_VAR_2, ITEM_ID
	msgbox Text_ObtainedFlyUnlock, MSGBOX_DEFAULT
	return

EventScript_ObtainedCoins::
	msgbox Text_ObtainedCoins, MSGBOX_DEFAULT
	return

EventScript_ObtainedLedgeJump::
	msgbox Text_ObtainedLedgeJump, MSGBOX_DEFAULT
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
	specialvar LOCATION_ID, ArchipelagoSpecial_GetObjectScriptFlag
	removeobject VAR_LAST_TALKED
	specialvar VAR_RESULT, ArchipelagoSpecial_ShouldHandle
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_FindItem
	call EventScript_FindItem
	additem ITEM_ID, AMOUNT
	release
	return

EventScript_FindItem::
	bufferitemname STR_VAR_2, ITEM_ID
	call EventScript_PlayItemFanfare
	call EventScript_FindItemMessage
	waitfanfare
	return

EventScript_FindItemMessage::
	specialvar VAR_RESULT, BufferTMHMMoveName
	goto_if_eq VAR_RESULT, TRUE, EventScript_FoundTMHM
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemBadge
	goto_if_eq VAR_RESULT, TRUE, EventScript_FoundBadge
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemFlyUnlock
	goto_if_eq VAR_RESULT, TRUE, EventScript_ObtainedFlyUnlock
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemCoins
	goto_if_eq VAR_RESULT, TRUE, EventScript_FoundCoins
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemLedgeJump
	goto_if_eq VAR_RESULT, TRUE, EventScript_ObtainedLedgeJump
	msgbox Text_FoundItem, MSGBOX_DEFAULT
	return

EventScript_FoundTMHM::
	msgbox Text_FoundTMHM, MSGBOX_DEFAULT
	return

EventScript_FoundBadge::
	msgbox Text_FoundBadge, MSGBOX_DEFAULT
	return

EventScript_FoundCoins::
	msgbox Text_FoundCoins, MSGBOX_DEFAULT
	return

EventScript_HiddenItemScript::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	waitse
	setflagvar LOCATION_ID
	specialvar VAR_RESULT, ArchipelagoSpecial_ShouldHandle
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_FindHiddenItem
	call EventScript_FindHiddenItem
	additem ITEM_ID, AMOUNT
	releaseall
	return

EventScript_FindHiddenItem::
	bufferitemname STR_VAR_2, ITEM_ID
	call EventScript_PlayItemFanfare
	call EventScript_FindItemMessage
	waitfanfare
	return
