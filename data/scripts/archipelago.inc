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
	msgbox Archipelago_Text_PlayerFoundArchipelagoItem, MSGBOX_DEFAULT
	waitfanfare
	waitmessage
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	return

ArchipelagoScript_FoundPlayersItem::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_PlayerFoundPlayersItem, MSGBOX_DEFAULT
	waitfanfare
	waitmessage
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	return

ArchipelagoScript_ObtainItem::
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemUnique
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_HandleUniqueItem
	call ArchipelagoScript_HandleTextBox
	setvar VAR_RESULT, TRUE
	return

ArchipelagoScript_FindItem::
	removeobject VAR_LAST_TALKED
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemUnique
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_HandleUniqueItem
	call ArchipelagoScript_HandleTextBox
	release
	return

ArchipelagoScript_FindHiddenItem::
	special SetHiddenItemFlag
	specialvar VAR_RESULT, ArchipelagoSpecial_IsItemUnique
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_HandleUniqueItem
	call ArchipelagoScript_HandleTextBox
	releaseall
	end

ArchipelagoScript_HandleUniqueItem::
	switch VAR_0x8014
	case ITEM_BADGE_1, ArchipelagoScript_ReceiveBoulderBadge
	case ITEM_BADGE_2, ArchipelagoScript_ReceiveCascadeBadge
	case ITEM_BADGE_3, ArchipelagoScript_ReceiveThunderBadge
	case ITEM_BADGE_4, ArchipelagoScript_ReceiveRainbowBadge
	case ITEM_BADGE_5, ArchipelagoScript_ReceiveSoulBadge
	case ITEM_BADGE_6, ArchipelagoScript_ReceiveMarshBadge
	case ITEM_BADGE_7, ArchipelagoScript_ReceiveVolcanoBadge
	case ITEM_BADGE_8, ArchipelagoScript_ReceiveEarthBadge
	case ITEM_FLY_PALLET, ArchipelagoScript_UnlockPalletFly
	case ITEM_FLY_VIRIDIAN, ArchipelagoScript_UnlockViridianFly
	case ITEM_FLY_PEWTER, ArchipelagoScript_UnlockPewterFly
	case ITEM_FLY_CERULEAN, ArchipelagoScript_UnlockCeruleanFly
	case ITEM_FLY_LAVENDER, ArchipelagoScript_UnlockLavenderFly
	case ITEM_FLY_VERMILION, ArchipelagoScript_UnlockVermilionFly
	case ITEM_FLY_CELADON, ArchipelagoScript_UnlockCeladonFly
	case ITEM_FLY_FUCHSIA, ArchipelagoScript_UnlockFuchsiaFly
	case ITEM_FLY_CINNABAR, ArchipelagoScript_UnlockCinnabarFly
	case ITEM_FLY_INDIGO, ArchipelagoScript_UnlockIndigoFly
	case ITEM_FLY_SAFFRON, ArchipelagoScript_UnlockSaffronFly
	case ITEM_FLY_ONE_ISLAND, ArchipelagoScript_UnlockOneIslandFly
	case ITEM_FLY_TWO_ISLAND, ArchipelagoScript_UnlockTwoIslandFly
	case ITEM_FLY_THREE_ISLAND, ArchipelagoScript_UnlockThreeIslandFly
	case ITEM_FLY_FOUR_ISLAND, ArchipelagoScript_UnlockFourIslandFly
	case ITEM_FLY_FIVE_ISLAND, ArchipelagoScript_UnlockFiveIslandFly
	case ITEM_FLY_SIX_ISLAND, ArchipelagoScript_UnlockSixIslandFly
	case ITEM_FLY_SEVEN_ISLAND, ArchipelagoScript_UnlockSevenIslandFly
	case ITEM_FLY_ROUTE4, ArchipelagoScript_UnlockRoute4Fly
	case ITEM_FLY_ROUTE10, ArchipelagoScript_UnlockRoute10Fly
	case ITEM_COINS_10, ArchipelagoScript_Receive10Coins
	case ITEM_COINS_20, ArchipelagoScript_Receive20Coins
	case ITEM_COINS_40, ArchipelagoScript_Receive40Coins
	case ITEM_COINS_100, ArchipelagoScript_Receive100Coins
	case ITEM_PROG_PASS, ArchipelagoScript_ReceiveProgressivePass
	case ITEM_PROG_CARD_KEY, ArchipelagoScript_ReceiveProgressiveCardKey
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

ArchipelagoScript_UnlockPalletFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToPalletTown, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_PALLET_TOWN
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockViridianFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToViridianCity, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_VIRIDIAN_CITY
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockPewterFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToPewterCity, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_PEWTER_CITY
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockCeruleanFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToCeruleanCity, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_CERULEAN_CITY
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockLavenderFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToLavenderTown, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_LAVENDER_TOWN
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockVermilionFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToVermilionCity, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_VERMILION_CITY
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockCeladonFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToCeladonCity, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_CELADON_CITY
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockFuchsiaFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToFuchsiaCity, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_FUCHSIA_CITY
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockCinnabarFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToCinnabarIsland, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_CINNABAR_ISLAND
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockIndigoFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToIndigoPlateau, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_INDIGO_PLATEAU_EXTERIOR
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockSaffronFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToSaffronCity, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_SAFFRON_CITY
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockOneIslandFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToOneIsland, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_ONE_ISLAND
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockTwoIslandFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToTwoIsland, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_TWO_ISLAND
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockThreeIslandFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToThreeIsland, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_THREE_ISLAND
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockFourIslandFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToFourIsland, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_FOUR_ISLAND
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockFiveIslandFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToFiveIsland, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_FIVE_ISLAND
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockSixIslandFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToSixIsland, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_SIX_ISLAND
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockSevenIslandFly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToSevenIsland, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_SEVEN_ISLAND
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockRoute4Fly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToRoute4, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_ROUTE4_POKEMON_CENTER_1F
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_UnlockRoute10Fly::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_FlyToRoute10, MSGBOX_DEFAULT
	waitfanfare
	setworldmapflag FLAG_WORLD_MAP_ROUTE10_POKEMON_CENTER_1F
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_Receive10Coins::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_Received10Coins, MSGBOX_DEFAULT
	waitfanfare
	addcoins 10
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_Receive20Coins::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_Received20Coins, MSGBOX_DEFAULT
	waitfanfare
	addcoins 20
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_Receive40Coins::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_Received40Coins, MSGBOX_DEFAULT
	waitfanfare
	addcoins 40
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_Receive100Coins::
	call EventScript_PlayFanfareObtainedItem
	copyvar VAR_PREV_TEXT_COLOR, VAR_TEXT_COLOR
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Archipelago_Text_Received100Coins, MSGBOX_DEFAULT
	waitfanfare
	addcoins 100
	copyvar VAR_TEXT_COLOR, VAR_PREV_TEXT_COLOR
	release
	return

ArchipelagoScript_ReceiveProgressivePass::
	specialvar VAR_RESULT, ArchipelagoSpecial_ArePassesSplit
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_ReceiveProgressivePassSplit
	checkitem ITEM_TRI_PASS
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveTriPass
	checkitem ITEM_RAINBOW_PASS
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveRainbowPass
	release
	return

ArchipelagoScript_ReceiveTriPass::
	giveitem ITEM_TRI_PASS
	release
	return

ArchipelagoScript_ReceiveRainbowPass::
	giveitem ITEM_RAINBOW_PASS
	release
	return

ArchipelagoScript_ReceiveProgressivePassSplit::
	checkitem ITEM_ONE_PASS
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveOnePass
	checkitem ITEM_TWO_PASS
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveTwoPass
	checkitem ITEM_THREE_PASS
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveThreePass
	checkitem ITEM_FOUR_PASS
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveFourPass
	checkitem ITEM_FIVE_PASS
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveFivePass
	checkitem ITEM_SIX_PASS
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveSixPass
	checkitem ITEM_SEVEN_PASS
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveSevenPass
	release
	return

ArchipelagoScript_ReceiveOnePass::
	giveitem ITEM_ONE_PASS
	release
	return

ArchipelagoScript_ReceiveTwoPass::
	giveitem ITEM_TWO_PASS
	release
	return

ArchipelagoScript_ReceiveThreePass::
	giveitem ITEM_THREE_PASS
	release
	return

ArchipelagoScript_ReceiveFourPass::
	giveitem ITEM_FOUR_PASS
	release
	return

ArchipelagoScript_ReceiveFivePass::
	giveitem ITEM_FIVE_PASS
	release
	return

ArchipelagoScript_ReceiveSixPass::
	giveitem ITEM_SIX_PASS
	release
	return

ArchipelagoScript_ReceiveSevenPass::
	giveitem ITEM_SEVEN_PASS
	release
	return

ArchipelagoScript_ReceiveProgressiveCardKey::
	checkitem ITEM_CARD_KEY_2F
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveCardKey2F
	checkitem ITEM_CARD_KEY_3F
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveCardKey3F
	checkitem ITEM_CARD_KEY_4F
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveCardKey4F
	checkitem ITEM_CARD_KEY_5F
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveCardKey5F
	checkitem ITEM_CARD_KEY_6F
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveCardKey6F
	checkitem ITEM_CARD_KEY_7F
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveCardKey7F
	checkitem ITEM_CARD_KEY_8F
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveCardKey8F
	checkitem ITEM_CARD_KEY_9F
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveCardKey9F
	checkitem ITEM_CARD_KEY_10F
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveCardKey10F
	checkitem ITEM_CARD_KEY_11F
	goto_if_eq VAR_RESULT, FALSE, ArchipelagoScript_ReceiveCardKey11F
	return

ArchipelagoScript_ReceiveCardKey2F::
	giveitem ITEM_CARD_KEY_2F
	release
	return

ArchipelagoScript_ReceiveCardKey3F::
	giveitem ITEM_CARD_KEY_3F
	release
	return

ArchipelagoScript_ReceiveCardKey4F::
	giveitem ITEM_CARD_KEY_4F
	release
	return

ArchipelagoScript_ReceiveCardKey5F::
	giveitem ITEM_CARD_KEY_5F
	release
	return

ArchipelagoScript_ReceiveCardKey6F::
	giveitem ITEM_CARD_KEY_6F
	release
	return

ArchipelagoScript_ReceiveCardKey7F::
	giveitem ITEM_CARD_KEY_7F
	release
	return

ArchipelagoScript_ReceiveCardKey8F::
	giveitem ITEM_CARD_KEY_8F
	release
	return

ArchipelagoScript_ReceiveCardKey9F::
	giveitem ITEM_CARD_KEY_9F
	release
	return

ArchipelagoScript_ReceiveCardKey10F::
	giveitem ITEM_CARD_KEY_10F
	release
	return

ArchipelagoScript_ReceiveCardKey11F::
	giveitem ITEM_CARD_KEY_11F
	release
	return

ArchipelagoScript_GoHome::
	setvar VAR_0x8003, 0
	special ArchipelagoSpecial_WarpToStartingLocation
	goto EventScript_ResetEliteFourEnd
	end
