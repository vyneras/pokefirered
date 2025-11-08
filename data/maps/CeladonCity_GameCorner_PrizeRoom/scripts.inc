CeladonCity_GameCorner_PrizeRoom_MapScripts::
	.byte 0

CeladonCity_GameCorner_PrizeRoom_EventScript_BaldingMan::
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_FancyThatPorygon, MSGBOX_NPC
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_OldMan::
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_RakedItInToday, MSGBOX_NPC
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkMons::
	lock
	faceplayer
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase
	goto_if_questlog EventScript_ReleaseEnd
	showcoinsbox 0, 0
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_WeExchangeCoinsForPrizes
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeMon::
    setflag FLAG_VIEWED_PRIZE_POKEMON
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_POKEMON_PRIZES, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_Abra
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_Clefairy
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_DratiniPinsir
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_ScytherDratini
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_Porygon
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange::
	hidecoinsbox 0, 0
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Abra::
	.ifdef FIRERED
Archipelago_Target_Prize_Pokemon_1::
	setvar VAR_TEMP_1, SPECIES_ABRA
	setvar VAR_TEMP_2, 180
	setvar VAR_TEMP_3, 1
	.else
	.ifdef LEAFGREEN
Archipelago_Target_Prize_Pokemon_1::
	setvar VAR_TEMP_1, SPECIES_ABRA
	setvar VAR_TEMP_2, 120
	setvar VAR_TEMP_3, 1
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Clefairy::
	.ifdef FIRERED
Archipelago_Target_Prize_Pokemon_2::
	setvar VAR_TEMP_1, SPECIES_CLEFAIRY
	setvar VAR_TEMP_2, 500
	setvar VAR_TEMP_3, 2
	.else
	.ifdef LEAFGREEN
Archipelago_Target_Prize_Pokemon_2::
	setvar VAR_TEMP_1, SPECIES_CLEFAIRY
	setvar VAR_TEMP_2, 750
	setvar VAR_TEMP_3, 2
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_DratiniPinsir::
	.ifdef FIRERED
Archipelago_Target_Prize_Pokemon_3::
	setvar VAR_TEMP_1, SPECIES_DRATINI
	setvar VAR_TEMP_2, 2800
	setvar VAR_TEMP_3, 3
	.else
	.ifdef LEAFGREEN
Archipelago_Target_Prize_Pokemon_3::
	setvar VAR_TEMP_1, SPECIES_PINSIR
	setvar VAR_TEMP_2, 2500
	setvar VAR_TEMP_3, 3
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ScytherDratini::
	.ifdef FIRERED
Archipelago_Target_Prize_Pokemon_4::
	setvar VAR_TEMP_1, SPECIES_SCYTHER
	setvar VAR_TEMP_2, 5500
	setvar VAR_TEMP_3, 4
	.else
	.ifdef LEAFGREEN
Archipelago_Target_Prize_Pokemon_4::
	setvar VAR_TEMP_1, SPECIES_DRATINI
	setvar VAR_TEMP_2, 4600
	setvar VAR_TEMP_3, 4
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Porygon::
	.ifdef FIRERED
Archipelago_Target_Prize_Pokemon_5::
	setvar VAR_TEMP_1, SPECIES_PORYGON
	setvar VAR_TEMP_2, 9999
	setvar VAR_TEMP_3, 5
	.else
	.ifdef LEAFGREEN
Archipelago_Target_Prize_Pokemon_5::
	setvar VAR_TEMP_1, SPECIES_PORYGON
	setvar VAR_TEMP_2, 6500
	setvar VAR_TEMP_3, 5
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon::
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_YouWantPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	checkcoins VAR_RESULT
	goto_if_lt VAR_RESULT, VAR_TEMP_2, CeladonCity_GameCorner_PrizeRoom_EventScript_NotEnoughCoins
	textcolor NPC_TEXT_COLOR_NEUTRAL
	switch VAR_TEMP_3
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_GivePokemon1
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_GivePokemon2
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_GivePokemon3
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_GivePokemon4
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_GivePokemon5
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GivePokemon1::
	.ifdef FIRERED
Archipelago_Target_Level_Prize_Pokemon_1::
	givemon VAR_TEMP_1, 9
	.else
	.ifdef LEAFGREEN
Archipelago_Target_Level_Prize_Pokemon_1::
	givemon VAR_TEMP_1, 7
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GivePokemon2::
	.ifdef FIRERED
Archipelago_Target_Level_Prize_Pokemon_2::
	givemon VAR_TEMP_1, 8
	.else
	.ifdef LEAFGREEN
Archipelago_Target_Level_Prize_Pokemon_2::
	givemon VAR_TEMP_1, 12
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GivePokemon3::
	.ifdef FIRERED
Archipelago_Target_Level_Prize_Pokemon_3::
	givemon VAR_TEMP_1, 18
	.else
	.ifdef LEAFGREEN
Archipelago_Target_Level_Prize_Pokemon_3::
	givemon VAR_TEMP_1, 18
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GivePokemon4::
	.ifdef FIRERED
Archipelago_Target_Level_Prize_Pokemon_4::
	givemon VAR_TEMP_1, 25
	.else
	.ifdef LEAFGREEN
Archipelago_Target_Level_Prize_Pokemon_4::
	givemon VAR_TEMP_1, 24
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GivePokemon5::
	.ifdef FIRERED
Archipelago_Target_Level_Prize_Pokemon_5::
	givemon VAR_TEMP_1, 26
	.else
	.ifdef LEAFGREEN
Archipelago_Target_Level_Prize_Pokemon_5::
	givemon VAR_TEMP_1, 18
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon::
	goto_if_eq VAR_RESULT, 0, CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonParty
	goto_if_eq VAR_RESULT, 1, CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonPC
	goto_if_eq VAR_RESULT, 2, CeladonCity_GameCorner_PrizeRoom_EventScript_PartyFull
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PartyFull::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Text_NoMoreRoomForPokemon
	hidecoinsbox 0, 0
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_NicknamePartyMon::
	getpartysize
	subvar VAR_RESULT, 1
	copyvar VAR_0x8004, VAR_RESULT
	call EventScript_ChangePokemonNickname
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_CoinCaseRequired
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_NotEnoughCoins::
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_NeedMoreCoins
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonParty::
	removecoins VAR_TEMP_2
	updatecoinsbox 0, 5
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	playfanfare MUS_LEVEL_UP
	message Text_PlayerObtainedTheMon
	waitmessage
	waitfanfare
	specialvar VAR_RESULT, ArchipelagoSpecial_SkipNickname
	goto_if_eq VAR_RESULT, TRUE, CeladonCity_GameCorner_PrizeRoom_EventScript_NicknamePartyMon
	msgbox Text_GiveNicknameToThisMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CeladonCity_GameCorner_PrizeRoom_EventScript_NicknamePartyMon
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonPC::
	removecoins VAR_TEMP_2
	updatecoinsbox 0, 5
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	playfanfare MUS_LEVEL_UP
	message Text_PlayerObtainedTheMon
	waitmessage
	waitfanfare
	specialvar VAR_RESULT, ArchipelagoSpecial_SkipNickname
	goto_if_eq VAR_RESULT, TRUE, CeladonCity_GameCorner_PrizeRoom_EventScript_TransferredToPC
	msgbox Text_GiveNicknameToThisMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_GameCorner_PrizeRoom_EventScript_TransferredToPC
	call EventScript_NameReceivedBoxMon
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_TransferredToPC
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TransferredToPC::
	call EventScript_TransferredToPC
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkTMs::
	lock
	faceplayer
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase
	goto_if_questlog EventScript_ReleaseEnd
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_WeExchangeCoinsForPrizes
	waitmessage
	pokemartcoin SHOP_CELADON_CITY_GAME_CORNER_TM_PRIZE
	msgbox Text_PleaseComeAgain
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkItems::
	lock
	faceplayer
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase
	goto_if_questlog EventScript_ReleaseEnd
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_WeExchangeCoinsForPrizes
	waitmessage
	pokemartcoin SHOP_CELADON_CITY_GAME_CORNER_PRIZE
	msgbox Text_PleaseComeAgain
	release
	end
