.include "data/text/archipelago.inc"

.set ITEM_ID, VAR_0x8000
.set LOCATION_ID, VAR_0x8003

ArchipelagoScript_ReceiveReward::
	specialvar VAR_RESULT, ArchipelagoSpecial_PopReward
	giveitem VAR_RESULT, 1, LOCATION_ID
	releaseall
	end

ArchipelagoScript_ReceiveRemoteItem::
	lockall
	specialvar VAR_RESULT, ArchipelagoSpecial_ConsumeReceivedItem
	giveitem VAR_RESULT
	releaseall
	end

ArchipelagoScript_ReceiveRemoteItemSilent::
	specialvar VAR_RESULT, ArchipelagoSpecial_ConsumeReceivedItem
	additem VAR_RESULT
	end

ArchipelagoScript_ObtainItem::
	call ArchipelagoScript_HandleTextBox
	setvar VAR_RESULT, TRUE
	return

ArchipelagoScript_FindItem::
	call ArchipelagoScript_HandleTextBox
	release
	return

ArchipelagoScript_FindHiddenItem::
	call ArchipelagoScript_HandleTextBox
	releaseall
	return

ArchipelagoScript_HandleTextBox::
	bufferapitemstrings STR_VAR_1, STR_VAR_2, LOCATION_ID, ITEM_ID
	goto_if_eq VAR_RESULT, 0, ArchipelagoScript_FoundArchipelagoItem
	goto_if_eq VAR_RESULT, 1, ArchipelagoScript_FoundPlayersItem
	return

ArchipelagoScript_FoundArchipelagoItem::
	call EventScript_PlayNormalItemFanfare
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_PlayerSentArchipelagoItem, MSGBOX_DEFAULT
	waitfanfare
	call EventScript_RestorePrevTextColor
	return

ArchipelagoScript_FoundPlayersItem::
	call EventScript_PlayNormalItemFanfare
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_PlayerSentPlayersItem, MSGBOX_DEFAULT
	waitfanfare
	call EventScript_RestorePrevTextColor
	return

ArchipelagoScript_GoHome::
	special ArchipelagoSpecial_WarpToStartingLocation
	goto EventScript_ResetEliteFourEnd
	end
