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
	lock
	faceplayer
	specialvar VAR_RESULT, ArchipelagoSpecial_ArePassesSplit
	goto_if_eq VAR_RESULT, FALSE, SevenIsland_EventScript_OldWomanGavePass
	goto_if_set FLAG_GOT_SEVEN_PASS, SevenIsland_EventScript_OldWomanGavePass
	msgbox Text_TakeThisPass
Archipelago_Target_NPC_Gift_FLAG_GOT_SEVEN_PASS::
	giveitem ITEM_SEVEN_PASS, 1, FLAG_GOT_SEVEN_PASS
	release
	end

SevenIsland_EventScript_OldWomanGavePass::
	msgbox SevenIsland_Text_IslandsMadeInSevenDays
	release
	end

SevenIsland_EventScript_Scientist::
	msgbox SevenIsland_Text_IslandVirtuallyUntouched, MSGBOX_NPC
	end
