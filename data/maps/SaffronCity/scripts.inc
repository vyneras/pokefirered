.equ GIVE_FLY_UNLOCK, VAR_TEMP_1

SaffronCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SaffronCity_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, SaffronCity_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SaffronCity_OnFrame
	.byte 0

SaffronCity_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, SaffronCity_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, SaffronCity_EventScript_SetWorldMapFlag
	specialvar VAR_RESULT, ArchipelagoSpecial_CanEnterSilphCo
	call_if_eq VAR_RESULT, FALSE, SaffronCity_EventScript_MoveDoorGuardGrunt
	call_if_unset FLAG_HIDE_SAFFRON_ROCKETS, SaffronCity_EventScript_MoveRocketGrunts
	end

SaffronCity_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_SAFFRON_CITY EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

SaffronCity_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_SAFFRON_CITY
	return

SaffronCity_EventScript_MoveDoorGuardGrunt::
	specialvar VAR_RESULT, ArchipelagoSpecial_PlayerEnteredThroughBlockedDoor
	goto_if_eq VAR_RESULT, TRUE, SaffronCity_EventScript_DontMoveDoorGuardGrunt
	setobjectxyperm LOCALID_SAFFRON_DOOR_GUARD_GRUNT, 33, 31
	return

SaffronCity_EventScript_DontMoveDoorGuardGrunt::
	setvar VAR_TEMP_2, 1
	return

SaffronCity_EventScript_MoveRocketGrunts::
	specialvar VAR_RESULT, ArchipelagoSpecial_PlayerEnteredThroughBlockedDoor
	goto_if_eq VAR_RESULT, FALSE, EventScript_Return
	setobjectxyperm LOCALID_SAFFRON_GRUNT1, 23, 15
	setobjectxyperm LOCALID_SAFFRON_GRUNT2, 47, 13
	setobjectxyperm LOCALID_SAFFRON_GRUNT3, 26, 22
	setvar VAR_TEMP_3, 1
	return

SaffronCity_OnLoad::
	call_if_set FLAG_UNLOCKED_SAFFRON_GYM_DOOR, SaffronCity_EventScript_SetGymDoorUnlocked
	end

SaffronCity_OnFrame::
	map_script_2 VAR_TEMP_2, 1, SaffronCity_EventScript_DoorGuardGruntMoveLeft
	map_script_2 VAR_TEMP_3, 1, SaffronCity_EventScript_RocketGruntsMoveLeft
	map_script_2 GIVE_FLY_UNLOCK, 1, SaffronCity_EventScript_GiveFlyUnlockItem
	.2byte 0

SaffronCity_EventScript_DoorGuardGruntMoveLeft::
	lockall
	applymovement LOCALID_SAFFRON_DOOR_GUARD_GRUNT, SaffronCity_Movement_MoveLeft
	waitmovement 0
	setobjectxyperm LOCALID_SAFFRON_DOOR_GUARD_GRUNT, 33, 31
	setvar VAR_TEMP_2, 0
	releaseall
	end

SaffronCity_EventScript_RocketGruntsMoveLeft::
	lockall
	applymovement LOCALID_SAFFRON_GRUNT1, SaffronCity_Movement_MoveLeft
	applymovement LOCALID_SAFFRON_GRUNT2, SaffronCity_Movement_MoveLeft
	applymovement LOCALID_SAFFRON_GRUNT3, SaffronCity_Movement_MoveRight
	waitmovement 0
	setobjectxyperm LOCALID_SAFFRON_GRUNT1, 22, 15
	setobjectxyperm LOCALID_SAFFRON_GRUNT2, 46, 13
	setobjectxyperm LOCALID_SAFFRON_GRUNT3, 27, 22
	setvar VAR_TEMP_3, 0
	releaseall
	end

SaffronCity_Movement_MoveLeft::
	walk_left
	face_down
	step_end

SaffronCity_Movement_MoveRight::
	walk_right
	face_down
	step_end

SaffronCity_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_SAFFRON_CITY::
	giveitem ITEM_FLY_SAFFRON, 1, FLAG_VISITED_SAFFRON_CITY
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

SaffronCity_EventScript_GymDoor::
	lockall
	goto_if_set FLAG_UNLOCKED_SAFFRON_GYM_DOOR, SaffronCity_EventScript_GymDoorAlreadyOpen
	checkitem ITEM_SAFFRON_KEY
	goto_if_eq VAR_RESULT, TRUE, SaffronCity_EventScript_OpenGymDoor
	msgbox Text_DoorIsLocked
	releaseall
	end

SaffronCity_EventScript_OpenGymDoor::
	msgbox Text_InsertGymKey
	closemessage
	playse SE_UNLOCK
	waitse
	call SaffronCity_EventScript_SetGymDoorUnlocked
	special DrawWholeMapView
	setflag FLAG_UNLOCKED_SAFFRON_GYM_DOOR
	msgbox Text_OpenedGymDoor
	releaseall
	end

SaffronCity_EventScript_GymDoorAlreadyOpen::
	msgbox Text_GymDoorAlreadyOpen
	releaseall
	end

SaffronCity_EventScript_SetGymDoorUnlocked::
	setmetatile 46, 12, METATILE_General_SlidingDoubleDoor, 1
	return

SaffronCity_EventScript_RocketGrunt1::
	msgbox SaffronCity_Text_WhatDoYouWantGetLost, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt2::
	msgbox SaffronCity_Text_BossTakeTownForTeamRocket, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt3::
	msgbox SaffronCity_Text_DontGetDefiantOrIllHurtYou, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt4::
	msgbox SaffronCity_Text_SaffronBelongsToTeamRocket, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt5::
	msgbox SaffronCity_Text_CriminalLifeMakesMeFeelAlive, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt6::
	msgbox SaffronCity_Text_WatchWhereYoureWalking, MSGBOX_NPC
	end

SaffronCity_EventScript_RocketGrunt7::
	msgbox SaffronCity_Text_WeCanExploitMonsAroundWorld, MSGBOX_NPC
	end

SaffronCity_EventScript_DoorGuardGrunt::
	lock
	goto_if_set FLAG_RESCUED_MR_FUJI, SaffronCity_EventScript_DoorGuardAsleep
	applymovement LOCALID_SAFFRON_DOOR_GUARD_GRUNT, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SaffronCity_Text_ImASecurityGuard
	release
	end

SaffronCity_EventScript_DoorGuardAsleep::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox SaffronCity_Text_HesTakingASnooze
	release
	end

SaffronCity_EventScript_WorkerM::
	msgbox SaffronCity_Text_YouBeatTeamRocket, MSGBOX_NPC
	end

SaffronCity_EventScript_Youngster::
	msgbox SaffronCity_Text_SafeToGoOutAgain, MSGBOX_NPC
	end

SaffronCity_EventScript_Lass::
	msgbox SaffronCity_Text_PeopleComingBackToSaffron, MSGBOX_NPC
	end

SaffronCity_EventScript_Boy::
	msgbox SaffronCity_Text_FlewHereOnPidgeot, MSGBOX_NPC
	end

SaffronCity_EventScript_Pidgeot::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIDGEOT, CRY_MODE_NORMAL
	msgbox SaffronCity_Text_Pidgeot
	waitmoncry
	release
	end

SaffronCity_EventScript_Man::
	msgbox SaffronCity_Text_SawRocketBossEscaping, MSGBOX_NPC
	end

SaffronCity_EventScript_CitySign::
	msgbox SaffronCity_Text_CitySign, MSGBOX_SIGN
	end

SaffronCity_EventScript_DojoSign::
	msgbox SaffronCity_Text_FightingDojo, MSGBOX_SIGN
	end

SaffronCity_EventScript_GymSign::
	lockall
	famechecker FAMECHECKER_SABRINA, 0
	msgbox SaffronCity_Text_GymSign
	releaseall
	end

SaffronCity_EventScript_TrainerTips1::
	msgbox SaffronCity_Text_FullHealCuresStatus, MSGBOX_SIGN
	end

SaffronCity_EventScript_TrainerTips2::
	msgbox SaffronCity_Text_GreatBallImprovedCatchRate, MSGBOX_SIGN
	end

SaffronCity_EventScript_SilphCoSign::
	msgbox SaffronCity_Text_SilphCoSign, MSGBOX_SIGN
	end

SaffronCity_EventScript_MrPsychicsHouseSign::
	msgbox SaffronCity_Text_MrPsychicsHouse, MSGBOX_SIGN
	end

SaffronCity_EventScript_SilphProductSign::
	msgbox SaffronCity_Text_SilphsLatestProduct, MSGBOX_SIGN
	end

SaffronCity_EventScript_TrainerFanClubSign::
	msgbox SaffronCity_Text_TrainerFanClubSign, MSGBOX_SIGN
	end
