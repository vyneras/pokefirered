.equ GIVE_FLY_UNLOCK, VAR_TEMP_1

OneIsland_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OneIsland_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, OneIsland_OnFrame
	.byte 0

OneIsland_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, OneIsland_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, OneIsland_EventScript_SetWorldMapFlag
	end

OneIsland_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_ONE_ISLAND EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

OneIsland_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_ONE_ISLAND
	return

OneIsland_OnFrame::
	map_script_2 GIVE_FLY_UNLOCK, 1, OneIsland_EventScript_GiveFlyUnlockItem
	.2byte 0

OneIsland_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_ONE_ISLAND::
	giveitem ITEM_FLY_ONE_ISLAND, 1, FLAG_VISITED_ONE_ISLAND
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

OneIsland_EventScript_OldMan::
	lock
	faceplayer
	goto_if_set FLAG_SYS_CAN_LINK_WITH_RS, OneIsland_EventScript_OldManLinkHoenn
	goto_if_set FLAG_SEVII_DETOUR_FINISHED, OneIsland_EventScript_OldManLinkKanto
	msgbox OneIsland_Text_LuckyToHaveCelioHere
	release
	end

OneIsland_EventScript_OldManLinkKanto::
	msgbox OneIsland_Text_HavePCLinkageWithKanto
	release
	end

OneIsland_EventScript_OldManLinkHoenn::
	msgbox OneIsland_Text_HavePCLinkageWithHoenn
	release
	end

OneIsland_EventScript_BaldingMan::
	msgbox OneIsland_Text_IsntWarmClimateHereGreat, MSGBOX_NPC
	end

OneIsland_EventScript_IslandSign::
	msgbox OneIsland_Text_IslandSign, MSGBOX_SIGN
	end

OneIsland_EventScript_PokemonNetCenterSign::
	msgbox OneIsland_Text_PokemonNetCenterSign, MSGBOX_SIGN
	end
