.equ GIVE_FLY_UNLOCK, VAR_TEMP_1

FiveIsland_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FiveIsland_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, FiveIsland_OnFrame
	.byte 0

FiveIsland_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, FiveIsland_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, FiveIsland_EventScript_SetWorldMapFlag
	end

FiveIsland_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_FIVE_ISLAND EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

FiveIsland_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_FIVE_ISLAND
	return

FiveIsland_OnFrame::
	map_script_2 GIVE_FLY_UNLOCK, 1, FiveIsland_EventScript_GiveFlyUnlockItem
	.2byte 0

FiveIsland_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_FIVE_ISLAND::
	giveitem ITEM_FLY_FIVE_ISLAND, 1, FLAG_VISITED_FIVE_ISLAND
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

FiveIsland_EventScript_IslandSign::
	msgbox FiveIsland_Text_IslandSign, MSGBOX_SIGN
	end

FiveIsland_EventScript_BugCatcher::
	msgbox FiveIsland_Text_WeirdBuildingInMeadow, MSGBOX_NPC
	end

FiveIsland_EventScript_Fisher::
	lock
	faceplayer
	specialvar VAR_RESULT, ArchipelagoSpecial_ArePassesSplit
	goto_if_eq VAR_RESULT, FALSE, FiveIsland_EventScript_FisherGavePass
	goto_if_set FLAG_GOT_FIVE_PASS, FiveIsland_EventScript_FisherGavePass
	msgbox Text_TakeThisPass
Archipelago_Target_NPC_Gift_FLAG_GOT_FIVE_PASS::
	giveitem ITEM_FIVE_PASS, 1, FLAG_GOT_FIVE_PASS
	release
	end

FiveIsland_EventScript_FisherGavePass::
	msgbox FiveIsland_Text_BeenGettingBusierAroundHere
	release
	end
