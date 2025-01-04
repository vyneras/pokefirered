.equ GIVE_FLY_UNLOCK, VAR_TEMP_1

SixIsland_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SixIsland_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SixIsland_OnFrame
	.byte 0

SixIsland_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, SixIsland_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, SixIsland_EventScript_SetWorldMapFlag
	end

SixIsland_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_SIX_ISLAND EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

SixIsland_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_SIX_ISLAND
	return

SixIsland_OnFrame::
	map_script_2 GIVE_FLY_UNLOCK, 1, SixIsland_EventScript_GiveFlyUnlockItem
	.2byte 0

SixIsland_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_SIX_ISLAND::
	giveitem ITEM_FLY_SIX_ISLAND, 1, FLAG_VISITED_SIX_ISLAND
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

SixIsland_EventScript_Hiker::
	msgbox SixIsland_Text_ThatWayToWaterPathRuinValley, MSGBOX_NPC
	end

SixIsland_EventScript_Boy::
	msgbox SixIsland_Text_SkyAtNightIsFantastic, MSGBOX_NPC
	end

SixIsland_EventScript_IslandSign::
	msgbox SixIsland_Text_IslandSign, MSGBOX_SIGN
	end
