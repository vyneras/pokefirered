CinnabarIsland_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIsland_OnTransition
	.byte 0

CinnabarIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CINNABAR_ISLAND
	call CinnabarIsland_EventScript_CheckUnlockGym
	end

CinnabarIsland_EventScript_CheckUnlockGym::
	checkitem ITEM_SECRET_KEY
	goto_if_eq VAR_RESULT, TRUE, CinnabarIsland_EventScript_UnlockGym
	return

CinnabarIsland_EventScript_UnlockGym::
	setvar VAR_TEMP_1, 1
	return

@ Triggered when VAR_TEMP_1 is 0
CinnabarIsland_EventScript_GymDoorLockedTrigger::
	lockall
	checkitem ITEM_SECRET_KEY
	call_if_eq VAR_RESULT, FALSE, CinnabarIsland_EventScript_GymDoorLocked
	releaseall
	end

CinnabarIsland_EventScript_GymDoorLocked::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 20
	msgbox CinnabarIsland_Text_DoorIsLocked
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, CinnabarIsland_Movement_ForcePlayerFromDoor
	waitmovement 0
  return

CinnabarIsland_Movement_ForcePlayerFromDoor::
	walk_down
	step_end

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
