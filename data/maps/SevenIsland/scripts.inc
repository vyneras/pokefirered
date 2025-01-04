.equ GIVE_FLY_UNLOCK, VAR_TEMP_1

SevenIsland_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SevenIsland_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SevenIsland_OnFrame
	.byte 0

SevenIsland_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, SevenIsland_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, SevenIsland_EventScript_SetWorldMapFlag
	end

SevenIsland_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_SEVEN_ISLAND EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

SevenIsland_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_SEVEN_ISLAND
	return

SevenIsland_OnFrame::
	map_script_2 GIVE_FLY_UNLOCK, 1, SevenIsland_EventScript_GiveFlyUnlockItem
	.2byte 0

SevenIsland_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_SEVEN_ISLAND::
	giveitem ITEM_FLY_SEVEN_ISLAND, 1, FLAG_VISITED_SEVEN_ISLAND
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

SevenIsland_EventScript_IslandSign::
	msgbox SevenIsland_Text_IslandSign, MSGBOX_SIGN
	end

SevenIsland_EventScript_OldWoman::
	msgbox SevenIsland_Text_IslandsMadeInSevenDays, MSGBOX_NPC
	end

SevenIsland_EventScript_Scientist::
	lock
	faceplayer
	goto_if_set FLAG_GAVE_SCANNER, SevenIsland_EventScript_ScientistAlreadyGaveScanner
	checkitem ITEM_SCANNER
	goto_if_eq VAR_RESULT, TRUE, SevenIsland_EventScript_AskForScanner
	msgbox SevenIsland_Text_ColleagueLostScanner
	release
	end

SevenIsland_EventScript_AskForScanner::
	msgbox SevenIsland_Text_CanIHaveIt, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, SevenIsland_EventScript_AcceptTrade
	goto_if_eq VAR_RESULT, NO, SevenIsland_EventScript_DeclineTrade
	end

SevenIsland_EventScript_AcceptTrade::
    textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_TMHM
	message SevenIsland_Text_HandedScanner
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	closemessage
Archipelago_Target_NPC_Gift_FLAG_GOT_DEEP_SEA_SCALE::
	giveitem ITEM_DEEP_SEA_SCALE, 1, FLAG_GOT_DEEP_SEA_SCALE
Archipelago_Target_NPC_Gift_FLAG_GOT_DEEP_SEA_TOOTH::
	giveitem ITEM_DEEP_SEA_TOOTH, 1, FLAG_GOT_DEEP_SEA_TOOTH
	setflag FLAG_GAVE_SCANNER
	goto SevenIsland_EventScript_ScientistAlreadyGaveScanner
	end

SevenIsland_EventScript_DeclineTrade::
    msgbox SevenIsland_Text_AreYouSure
    release
    end

SevenIsland_EventScript_ScientistAlreadyGaveScanner::
	msgbox SevenIsland_Text_GetBackToResearch
	release
	end
