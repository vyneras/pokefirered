.equ GIVE_FLY_UNLOCK, VAR_TEMP_1

LavenderTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LavenderTown_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, LavenderTown_OnFrame
	.byte 0

LavenderTown_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, LavenderTown_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, LavenderTown_EventScript_SetWorldMapFlag
	setvar VAR_TELEPORT_LOCATION, SPAWN_LAVENDER_TOWN
	end

LavenderTown_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_LAVENDER_TOWN EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

LavenderTown_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_LAVENDER_TOWN
	return

LavenderTown_OnFrame::
	map_script_2 GIVE_FLY_UNLOCK, 1, LavenderTown_EventScript_GiveFlyUnlockItem
	.2byte 0

LavenderTown_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_LAVENDER_TOWN::
	giveitem ITEM_FLY_LAVENDER, 1, FLAG_VISITED_LAVENDER_TOWN
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

LavenderTown_EventScript_LittleGirl::
	lock
	faceplayer
	msgbox LavenderTown_Text_DoYouBelieveInGhosts, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, LavenderTown_EventScript_LittleGirlBelieve
	msgbox LavenderTown_Text_JustImaginingWhiteHand
	release
	end

LavenderTown_EventScript_LittleGirlBelieve::
	msgbox LavenderTown_Text_SoThereAreBelievers
	release
	end

LavenderTown_EventScript_WorkerM::
	msgbox LavenderTown_Text_TownKnownAsMonGraveSite, MSGBOX_NPC
	end

LavenderTown_EventScript_Boy::
	msgbox LavenderTown_Text_GhostsAppearedInTower, MSGBOX_NPC
	end

LavenderTown_EventScript_TownSign::
	msgbox LavenderTown_Text_TownSign, MSGBOX_SIGN
	end

LavenderTown_EventScript_SilphScopeNotice::
	msgbox LavenderTown_Text_SilphScopeNotice, MSGBOX_SIGN
	end

LavenderTown_EventScript_VolunteerHouseSign::
	msgbox LavenderTown_Text_VolunteerPokemonHouse, MSGBOX_SIGN
	end

LavenderTown_EventScript_PokemonTowerSign::
	msgbox LavenderTown_Text_PokemonTowerSign, MSGBOX_SIGN
	end
