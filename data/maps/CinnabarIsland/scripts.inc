.set LOCALID_POLICEMAN, 3

.equ GIVE_FLY_UNLOCK, VAR_TEMP_1

CinnabarIsland_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIsland_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, CinnabarIsland_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CinnabarIsland_OnFrame
	.byte 0

CinnabarIsland_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, CinnabarIsland_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, CinnabarIsland_EventScript_SetWorldMapFlag
	specialvar VAR_RESULT ArchipelagoSpecial_ExtraKeyItems
	call_if_eq VAR_RESULT, TRUE, CinnabarIsland_EventScript_CheckBlockMansion
	setvar VAR_TELEPORT_LOCATION, SPAWN_CINNABAR_ISLAND
	end

CinnabarIsland_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_CINNABAR_ISLAND EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

CinnabarIsland_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_CINNABAR_ISLAND
	return

CinnabarIsland_EventScript_CheckBlockMansion::
	call_if_unset FLAG_SHOWED_LETTER, CinnabarIsland_EventScript_BlockMansion
	return

CinnabarIsland_EventScript_BlockMansion::
	specialvar VAR_RESULT, ArchipelagoSpecial_PlayerEnteredThroughBlockedDoor
	goto_if_eq VAR_RESULT, TRUE, CinnabarIsland_EventScript_DontBlockMansion
	setobjectxyperm LOCALID_POLICEMAN, 8, 4
	return

CinnabarIsland_EventScript_DontBlockMansion::
	setobjectxyperm LOCALID_POLICEMAN, 7, 4
	setvar VAR_TEMP_2, 1
	return

CinnabarIsland_OnLoad::
	setvar VAR_TEMP_3, 1
	end

CinnabarIsland_OnFrame::
	map_script_2 VAR_TEMP_2, 1, CinnabarIsland_EventScript_PolicemanMoveRight
	map_script_2 VAR_TEMP_3, 1, CinnabarIsland_EventScript_CheckGymDoorUnlocked
	map_script_2 GIVE_FLY_UNLOCK, 1, CinnabarIsland_EventScript_GiveFlyUnlockItem
	.2byte 0

CinnabarIsland_EventScript_PolicemanMoveRight::
	lockall
	applymovement LOCALID_POLICEMAN, CinnabarIsland_Movement_MoveRight
	waitmovement 0
	copyobjectxytoperm LOCALID_POLICEMAN
	setvar VAR_TEMP_2, 0
	releaseall
	end

CinnabarIsland_Movement_MoveRight::
	walk_right
	face_down
	step_end

CinnabarIsland_EventScript_CheckGymDoorUnlocked::
	call_if_set FLAG_UNLOCKED_CINNABAR_GYM_DOOR, CinnabarIsland_EventScript_SetGymDoorUnlocked
	setvar VAR_TEMP_3, 0
	end

CinnabarIsland_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_CINNABAR_ISLAND::
	giveitem ITEM_FLY_CINNABAR, 1, FLAG_VISITED_CINNABAR_ISLAND
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

CinnabarIsland_EventScript_GymDoor::
	lockall
	goto_if_set FLAG_UNLOCKED_CINNABAR_GYM_DOOR, CinnabarIsland_EventScript_GymDoorAlreadyOpen
	checkitem ITEM_SECRET_KEY
	goto_if_eq VAR_RESULT, TRUE, CinnabarIsland_EventScript_OpenGymDoor
	checkitem ITEM_CINNABAR_KEY
	goto_if_eq VAR_RESULT, TRUE, CinnabarIsland_EventScript_OpenGymDoor
	msgbox Text_DoorIsLocked
	releaseall
	end

CinnabarIsland_EventScript_OpenGymDoor::
	msgbox Text_InsertGymKey
	closemessage
	playse SE_UNLOCK
	waitse
	call CinnabarIsland_EventScript_SetGymDoorUnlocked
	setflag FLAG_UNLOCKED_CINNABAR_GYM_DOOR
	msgbox Text_OpenedGymDoor
	releaseall
	end

CinnabarIsland_EventScript_GymDoorAlreadyOpen::
	msgbox Text_GymDoorAlreadyOpen
	releaseall
	end

CinnabarIsland_EventScript_SetGymDoorUnlocked::
	setmetatile 20, 4, METATILE_General_SlidingDoubleDoor, 1
	special DrawWholeMapView
	return

CinnabarIsland_EventScript_Woman::
	lock
	faceplayer
	famechecker FAMECHECKER_BLAINE, 3
	msgbox CinnabarIsland_Text_BlaineLivedHereSinceBeforeLab
	release
	end

CinnabarIsland_EventScript_OldMan::
	msgbox CinnabarIsland_Text_ScientistsExperimentInMansion, MSGBOX_NPC
	end

CinnabarIsland_EventScript_IslandSign::
	msgbox CinnabarIsland_Text_IslandSign, MSGBOX_SIGN
	end

CinnabarIsland_EventScript_PokemonLabSign::
	msgbox CinnabarIsland_Text_PokemonLab, MSGBOX_SIGN
	end

CinnabarIsland_EventScript_GymSign::
	lockall
	famechecker FAMECHECKER_BLAINE, 0
	msgbox CinnabarIsland_Text_GymSign
	releaseall
	end

CinnabarIsland_EventScript_Policeman::
	lock
	faceplayer
	goto_if_set FLAG_SHOWED_LETTER, CinnabarIsland_EventScript_PolicemanBeCareful
	checkitem ITEM_LETTER
	goto_if_eq VAR_RESULT, TRUE, CinnabarIsland_EventScript_PolicemanFoundLetter
	msgbox CinnabarIsland_Text_NoLetter
	release
	end

CinnabarIsland_EventScript_PolicemanFoundLetter::
	removeitem ITEM_LETTER
	setflag FLAG_SHOWED_LETTER
	msgbox CinnabarIsland_Text_FoundLetter
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_TMHM
	message CinnabarIsland_Text_GiveLetter
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	closemessage
	msgbox CinnabarIsland_Text_YouCanHeadIn
	closemessage
	goto_if_eq VAR_FACING, DIR_EAST, CinnabarIsland_EventScript_PolicemanMoveEast
	applymovement LOCALID_POLICEMAN, CinnabarIsland_Movement_PolicemanMoveWest
	waitmovement 0
	copyobjectxytoperm LOCALID_POLICEMAN
	release
	end

CinnabarIsland_EventScript_PolicemanMoveEast::
	applymovement LOCALID_POLICEMAN, CinnabarIsland_Movement_PolicemanMoveEast
	waitmovement 0
	copyobjectxytoperm LOCALID_POLICEMAN
	release
	end

CinnabarIsland_EventScript_PolicemanBeCareful::
	msgbox CinnabarIsland_Text_BeCareful
	release
	end

CinnabarIsland_Movement_PolicemanMoveWest::
	walk_left
	face_down
	step_end

CinnabarIsland_Movement_PolicemanMoveEast::
	walk_right
	face_down
	step_end
