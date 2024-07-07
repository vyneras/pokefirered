.include "data/text/archipelago.inc"

ArchipelagoScript_ReceiveReward::
	specialvar VAR_RESULT, ArchipelagoSpecial_PopReward
	giveitem VAR_RESULT, 1, VAR_0x8003
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
	additem VAR_RESULT, 1
	end

ArchipelagoScript_HandleTextBox::
	bufferapitemstrings STR_VAR_1, STR_VAR_2, VAR_0x8003
	goto_if_eq VAR_RESULT, 0, ArchipelagoScript_FoundArchipelagoItem
	goto_if_eq VAR_RESULT, 1, ArchipelagoScript_FoundPlayersItem
	return

ArchipelagoScript_FoundArchipelagoItem::
	call EventScript_PlayFanfareObtainedItem
  copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox gText_PlayerFoundArchipelagoItem, MSGBOX_DEFAULT
	waitfanfare
	waitmessage
  copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	return

ArchipelagoScript_FoundPlayersItem::
	call EventScript_PlayFanfareObtainedItem
  copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox gText_PlayerFoundPlayersItem, MSGBOX_DEFAULT
	waitfanfare
	waitmessage
  copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	return

ArchipelagoScript_ObtainItem::
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemBadge
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_HandleBadgeItem
	call ArchipelagoScript_HandleTextBox
	setvar VAR_RESULT, TRUE
	return

ArchipelagoScript_FindItem::
	removeobject VAR_LAST_TALKED
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemBadge
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_HandleBadgeItem
	call ArchipelagoScript_HandleTextBox
	release
	return

ArchipelagoScript_FindHiddenItem::
	special SetHiddenItemFlag
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemBadge
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_HandleBadgeItem
	call ArchipelagoScript_HandleTextBox
	releaseall
	end

ArchipelagoScript_HandleBadgeItem::
	switch VAR_0x8014
	case ITEM_BADGE_1, ArchipelagoScript_ReceiveBoulderBadge
	case ITEM_BADGE_2, ArchipelagoScript_ReceiveCascadeBadge
	case ITEM_BADGE_3, ArchipelagoScript_ReceiveThunderBadge
	case ITEM_BADGE_4, ArchipelagoScript_ReceiveRainbowBadge
	case ITEM_BADGE_5, ArchipelagoScript_ReceiveSoulBadge
	case ITEM_BADGE_6, ArchipelagoScript_ReceiveMarshBadge
	case ITEM_BADGE_7, ArchipelagoScript_ReceiveVolcanoBadge
	case ITEM_BADGE_8, ArchipelagoScript_ReceiveEarthBadge
	return

ArchipelagoScript_ReceiveBoulderBadge::
	call EventScript_PlayGymBadgeFanfare
  copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PewterCity_Gym_Text_ReceivedBoulderBadge, MSGBOX_DEFAULT
	waitfanfare
	setflag FLAG_BADGE01_GET
  copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_ReceiveCascadeBadge::
	call EventScript_PlayGymBadgeFanfare
  copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox CeruleanCity_Gym_Text_ReceivedCascadeBadge, MSGBOX_DEFAULT
	waitfanfare
	setflag FLAG_BADGE02_GET
  copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_ReceiveThunderBadge::
	call EventScript_PlayGymBadgeFanfare
  copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox VermilionCity_Gym_Text_ReceivedThunderBadge, MSGBOX_DEFAULT
	waitfanfare
	setflag FLAG_BADGE03_GET
  copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_ReceiveRainbowBadge::
	call EventScript_PlayGymBadgeFanfare
  copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox CeladonCity_Gym_Text_ReceivedRainbowBadge, MSGBOX_DEFAULT
	waitfanfare
	setflag FLAG_BADGE04_GET
  copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_ReceiveSoulBadge::
	call EventScript_PlayGymBadgeFanfare
  copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox FuchsiaCity_Gym_Text_ReceivedSoulBadge, MSGBOX_DEFAULT
	waitfanfare
	setflag FLAG_BADGE05_GET
  copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_ReceiveMarshBadge::
	call EventScript_PlayGymBadgeFanfare
  copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox SaffronCity_Gym_Text_ReceivedMarshBadge, MSGBOX_DEFAULT
	waitfanfare
	setflag FLAG_BADGE06_GET
  copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_ReceiveVolcanoBadge::
	call EventScript_PlayGymBadgeFanfare
  copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox CinnabarIsland_Gym_Text_ReceivedVolcanoBadge, MSGBOX_DEFAULT
	waitfanfare
	setflag FLAG_BADGE07_GET
  copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_ReceiveEarthBadge::
	call EventScript_PlayGymBadgeFanfare
  copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox ViridianCity_Gym_Text_ReceivedEarthBadge, MSGBOX_DEFAULT
	waitfanfare
	setflag FLAG_BADGE08_GET
  copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_GoHome::
	warpteleport MAP_PALLET_TOWN, 6, 8
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceDown
	goto EventScript_ResetEliteFourEnd
	end
