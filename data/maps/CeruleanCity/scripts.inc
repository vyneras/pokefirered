.set LOCALID_POLICEMAN, 1
.set LOCALID_GRUNT,     2
.set LOCALID_SLOWBRO,   5
.set LOCALID_LASS,      6
.set LOCALID_RIVAL,     8
.set LOCALID_WOMAN,     11
.set LOCALID_CAVE_GUARD, 12
.set REQUIRED_BADGES_GYMS, VAR_TEMP_2

.equ GIVE_FLY_UNLOCK, VAR_TEMP_3

CeruleanCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCity_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, CeruleanCity_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CeruleanCity_OnFrame
	.byte 0

CeruleanCity_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, CeruleanCity_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, CeruleanCity_EventScript_SetWorldMapFlag
	specialvar VAR_RESULT, ArchipelagoSpecial_CanLeaveCeruleanCity
	call_if_eq VAR_RESULT, FALSE, CeruleanCity_EventScript_BlockExits
	specialvar VAR_RESULT, ArchipelagoSpecial_CanEnterCeruleanCave
	call_if_eq VAR_RESULT, TRUE, CeruleanCity_EventScript_RemoveGuard
	specialvar VAR_RESULT, ArchipelagoSpecial_Route9Modified
	call_if_eq VAR_RESULT, TRUE, CeruleanCity_EventScript_RemoveCutTree
	call_if_eq VAR_RESULT, FALSE, CeruleanCity_EventScript_RemoveSmashRock
	specialvar VAR_RESULT, ArchipelagoSpecial_IsFamesanity
	call_if_eq VAR_RESULT, TRUE, CeruleanCity_EventScript_CheckRivalSetPosition
	end

CeruleanCity_EventScript_RemoveCutTree::
	setflag FLAG_TEMP_12
	return

CeruleanCity_EventScript_RemoveSmashRock::
	setflag FLAG_TEMP_14
	return

CeruleanCity_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_CERULEAN_CITY EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

CeruleanCity_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_CERULEAN_CITY
	return

CeruleanCity_EventScript_BlockExits::
	specialvar VAR_RESULT, ArchipelagoSpecial_PlayerEnteredThroughBlockedDoor
	call_if_eq VAR_RESULT, TRUE, CeruleanCity_EventScript_GuardNotBlocking
	call_if_eq VAR_RESULT, FALSE, CeruleanCity_EventScript_GuardBlocking
	setobjectxyperm LOCALID_SLOWBRO, 26, 31
	setobjectxyperm LOCALID_LASS, 27, 31
	return

CeruleanCity_EventScript_GuardNotBlocking::
	setobjectxyperm LOCALID_POLICEMAN, 31, 12
	setvar VAR_TEMP_4, 1
	return

CeruleanCity_EventScript_GuardBlocking::
	setobjectxyperm LOCALID_POLICEMAN, 30, 12
	return

CeruleanCity_EventScript_RemoveGuard::
	setflag FLAG_HIDE_CERULEAN_CAVE_GUARD
	return

CeruleanCity_EventScript_CheckRivalSetPosition::
	call_if_set FLAG_GOT_FAME_CHECKER, CeruleanCity_EventScript_RivalSetPosition
	return

CeruleanCity_EventScript_RivalSetPosition::
	clearflag FLAG_HIDE_CERULEAN_RIVAL
	setobjectxyperm LOCALID_RIVAL, 23, 20
	return

CeruleanCity_OnLoad::
	call_if_set FLAG_UNLOCKED_CERULEAN_GYM_DOOR, CeruleanCity_EventScript_SetGymDoorUnlocked
	end

CeruleanCity_OnFrame::
	map_script_2 VAR_TEMP_4, 1, CeruleanCity_EventScript_PolicemanMoveLeft
	map_script_2 GIVE_FLY_UNLOCK, 1, CeruleanCity_EventScript_GiveFlyUnlockItem
	.2byte 0

CeruleanCity_EventScript_PolicemanMoveLeft::
	lockall
	applymovement LOCALID_POLICEMAN, CeruleanCity_Movement_PolicemanMoveLeft
	waitmovement 0
	setobjectxyperm LOCALID_POLICEMAN, 30, 12
	setvar VAR_TEMP_4, 0
	releaseall
	end

CeruleanCity_Movement_PolicemanMoveLeft::
	walk_left
	face_down
	step_end

CeruleanCity_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_CERULEAN_CITY::
	giveitem ITEM_FLY_CERULEAN, 1, FLAG_VISITED_CERULEAN_CITY
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

CeruleanCity_EventScript_GymDoor::
	lockall
	goto_if_set FLAG_UNLOCKED_CERULEAN_GYM_DOOR, CeruleanCity_EventScript_GymDoorAlreadyOpen
	checkitem ITEM_CERULEAN_KEY
	goto_if_eq VAR_RESULT, TRUE, CeruleanCity_EventScript_OpenGymDoor
	msgbox Text_DoorIsLocked
	releaseall
	end

CeruleanCity_EventScript_OpenGymDoor::
	msgbox Text_InsertGymKey
	closemessage
	playse SE_UNLOCK
	waitse
	call CeruleanCity_EventScript_SetGymDoorUnlocked
	special DrawWholeMapView
	setflag FLAG_UNLOCKED_CERULEAN_GYM_DOOR
	msgbox Text_OpenedGymDoor
	releaseall
	end

CeruleanCity_EventScript_GymDoorAlreadyOpen::
	msgbox Text_GymDoorAlreadyOpen
	releaseall
	end

CeruleanCity_EventScript_SetGymDoorUnlocked::
	setmetatile 31, 21, METATILE_General_SlidingDoubleDoor, 1
	return

CeruleanCity_EventScript_RivalTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto CeruleanCity_EventScript_Rival
	end

CeruleanCity_EventScript_RivalTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	setobjectxyperm LOCALID_RIVAL, 23, 0
	goto CeruleanCity_EventScript_Rival
	end

CeruleanCity_EventScript_RivalTriggerRight::
	lockall
	setvar VAR_TEMP_1, 2
	setobjectxyperm LOCALID_RIVAL, 24, 0
	goto CeruleanCity_EventScript_Rival
	end

CeruleanCity_EventScript_Rival::
	textcolor NPC_TEXT_COLOR_MALE
	playbgm MUS_ENCOUNTER_RIVAL, 0
	addobject LOCALID_RIVAL
	applymovement LOCALID_RIVAL, CeruleanCity_Movement_RivalEnter
	waitmovement 0
	msgbox CeruleanCity_Text_RivalIntro
	setvar VAR_LAST_TALKED, LOCALID_RIVAL
	call_if_eq VAR_STARTER_MON, 2, CeruleanCity_EventScript_RivalSquirtle
	call_if_eq VAR_STARTER_MON, 1, CeruleanCity_EventScript_RivalBulbasaur
	call_if_eq VAR_STARTER_MON, 0, CeruleanCity_EventScript_RivalCharmander
	settrainerflag TRAINER_RIVAL_CERULEAN_BULBASAUR
	settrainerflag TRAINER_RIVAL_CERULEAN_CHARMANDER
	settrainerflag TRAINER_RIVAL_CERULEAN_SQUIRTLE
	call CeruleanCity_EventScript_RivalFameChecker
	msgbox CeruleanCity_Text_RivalPostBattle
	closemessage
	playbgm MUS_RIVAL_EXIT, 0
	call_if_eq VAR_TEMP_1, 0, CeruleanCity_EventScript_RivalStartExit
	call_if_eq VAR_TEMP_1, 1, CeruleanCity_EventScript_RivalStartExit
	call_if_eq VAR_TEMP_1, 2, CeruleanCity_EventScript_RivalStartExitRight
	msgbox CeruleanCity_Text_OhRightLittlePresentAsFavor
	setvar VAR_MAP_SCENE_CERULEAN_CITY_RIVAL, 1
Archipelago_Target_NPC_Gift_FLAG_GOT_FAME_CHECKER::
	giveitem ITEM_FAME_CHECKER, 1, FLAG_GOT_FAME_CHECKER
	msgbox CeruleanCity_Text_ExplainFameCheckerSmellYa
	closemessage
	applymovement LOCALID_RIVAL, CeruleanCity_Movement_RivalExit
	waitmovement 0
	fadedefaultbgm
	removeobject LOCALID_RIVAL
	releaseall
	end

CeruleanCity_EventScript_RivalTalk::
	lock
	faceplayer
	call CeruleanCity_EventScript_RivalFameChecker
	msgbox CeruleanCity_Text_RivalTalkAboutBill
	release
	end

CeruleanCity_EventScript_RivalFameChecker::
	famechecker FAMECHECKER_BILL, 0
	return

CeruleanCity_EventScript_RivalSquirtle::
	trainerbattle_no_intro TRAINER_RIVAL_CERULEAN_SQUIRTLE, CeruleanCity_Text_RivalDefeat
	return

CeruleanCity_EventScript_RivalBulbasaur::
	trainerbattle_no_intro TRAINER_RIVAL_CERULEAN_BULBASAUR, CeruleanCity_Text_RivalDefeat
	return

CeruleanCity_EventScript_RivalCharmander::
	trainerbattle_no_intro TRAINER_RIVAL_CERULEAN_CHARMANDER, CeruleanCity_Text_RivalDefeat
	return

CeruleanCity_EventScript_RivalStartExit::
	applymovement OBJ_EVENT_ID_PLAYER, CeruleanCity_Movement_PlayerWatchRivalExit
	applymovement LOCALID_RIVAL, CeruleanCity_Movement_RivalStartExit
	waitmovement 0
	return

CeruleanCity_EventScript_RivalStartExitRight::
	applymovement OBJ_EVENT_ID_PLAYER, CeruleanCity_Movement_PlayerWatchRivalExitRight
	applymovement LOCALID_RIVAL, CeruleanCity_Movement_RivalStartExitRight
	waitmovement 0
	return

CeruleanCity_Movement_PlayerWatchRivalExit::
	delay_16
	delay_8
	walk_in_place_faster_right
	delay_16
	walk_in_place_faster_down
	step_end

CeruleanCity_Movement_PlayerWatchRivalExitRight::
	delay_16
	delay_8
	walk_in_place_faster_left
	delay_16
	walk_in_place_faster_down
	step_end

CeruleanCity_Movement_RivalEnter::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

CeruleanCity_Movement_RivalStartExit::
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	delay_16
	delay_16
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

CeruleanCity_Movement_RivalStartExitRight::
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	delay_16
	delay_16
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

CeruleanCity_Movement_RivalExit::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

CeruleanCity_EventScript_Grunt::
	lock
	faceplayer
	goto_if_defeated TRAINER_TEAM_ROCKET_GRUNT_5, CeruleanCity_EventScript_GruntDefeated
	message CeruleanCity_Text_GruntIntro
	waitmessage
	playbgm MUS_ENCOUNTER_ROCKET, 0
	waitbuttonpress
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_5, CeruleanCity_Text_GruntDefeat
	setvar VAR_MAP_SCENE_CERULEAN_CITY_ROCKET, 1
	goto CeruleanCity_EventScript_GruntDefeated
	end

CeruleanCity_EventScript_GruntDefeated::
	msgbox CeruleanCity_Text_OkayIllReturnStolenTM
Archipelago_Target_NPC_Gift_FLAG_GOT_TM28_FROM_ROCKET::
	giveitem ITEM_TM28, 1, FLAG_GOT_TM28_FROM_ROCKET
	msgbox CeruleanCity_Text_BetterGetMovingBye
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_GRUNT
	fadescreen FADE_FROM_BLACK
	release
	end

CeruleanCity_EventScript_GruntTriggerTop::
	lockall
	setvar VAR_TEMP_1, 0
	applymovement LOCALID_GRUNT, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	goto CeruleanCity_EventScript_GruntTrigger
	end

CeruleanCity_EventScript_GruntTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 1
	applymovement LOCALID_GRUNT, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	goto CeruleanCity_EventScript_GruntTrigger
	end

CeruleanCity_EventScript_GruntTrigger::
	textcolor NPC_TEXT_COLOR_MALE
	msgbox CeruleanCity_Text_GruntIntro
	setvar VAR_LAST_TALKED, LOCALID_GRUNT
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_5, CeruleanCity_Text_GruntDefeat
	setvar VAR_MAP_SCENE_CERULEAN_CITY_ROCKET, 1
	goto CeruleanCity_EventScript_GruntDefeated
	end

CeruleanCity_EventScript_Policeman::
	lock
	faceplayer
	msgbox CeruleanCity_Text_PeopleHereWereRobbed
	closemessage
	applymovement LOCALID_POLICEMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeruleanCity_EventScript_LittleBoy::
	lock
	faceplayer
	goto_if_set FLAG_GOT_SS_TICKET, CeruleanCity_EventScript_LittleBoySlowbroMoved
	msgbox CeruleanCity_Text_IfSlowbroWasntThereCouldCutTree
	release
	end

CeruleanCity_EventScript_LittleBoySlowbroMoved::
	msgbox CeruleanCity_Text_YouCanCutDownSmallTrees
	release
	end

CeruleanCity_EventScript_BaldingMan::
	msgbox CeruleanCity_Text_PokemonEncyclopediaAmusing, MSGBOX_NPC
	end

CeruleanCity_EventScript_Youngster::
	msgbox CeruleanCity_Text_TrainerLifeIsToughIsntIt, MSGBOX_NPC
	end

CeruleanCity_EventScript_CeruleanCaveGuard::
	lock
	faceplayer
	msgbox CeruleanCity_Text_ThisIsCeruleanCave
	specialvar VAR_RESULT, ArchipelagoSpecial_CanEnterCeruleanCave
	goto_if_eq VAR_RESULT, TRUE, CeruleanCity_EventScript_CeruleanCaveGuardLeave
	specialvar VAR_RESULT, ArchipelagoSpecial_GetCeruleanCaveRequirement
	switch VAR_RESULT
	case 0, CeruleanCity_EventScript_CeruleanCaveGuardChampAndNetwork
	case 1, CeruleanCity_EventScript_CeruleanCaveGuardChamp
	case 2, CeruleanCity_EventScript_CeruleanCaveGuardNetwork
	case 3, CeruleanCity_EventScript_CeruleanCaveGuardBadges
	case 4, CeruleanCity_EventScript_CeruleanCaveGuardGyms
	end

CeruleanCity_EventScript_CeruleanCaveGuardLeave::
	msgbox CeruleanCity_Text_HaveWhatItTakes
	fadescreen FADE_TO_BLACK
	closemessage
	removeobject LOCALID_CAVE_GUARD
	fadescreen FADE_FROM_BLACK
	release
	end

CeruleanCity_EventScript_CeruleanCaveGuardChampAndNetwork::
	msgbox CeruleanCity_Text_BecomeChampAndFixNetwork
	applymovement LOCALID_CAVE_GUARD, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeruleanCity_EventScript_CeruleanCaveGuardChamp::
	msgbox CeruleanCity_Text_BecomeChamp
	applymovement LOCALID_CAVE_GUARD, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeruleanCity_EventScript_CeruleanCaveGuardNetwork::
	msgbox CeruleanCity_Text_FixNetwork
	applymovement LOCALID_CAVE_GUARD, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeruleanCity_EventScript_CeruleanCaveGuardBadges::
	specialvar REQUIRED_BADGES_GYMS, ArchipelagoSpecial_GetCeruleanCaveRequiredCount
	buffernumberstring STR_VAR_1, REQUIRED_BADGES_GYMS
	goto_if_eq REQUIRED_BADGES_GYMS, 1, CeruleanCity_EventScript_CeruleanCaveGuardBadge
	msgbox CeruleanCity_Text_Badges
	applymovement LOCALID_CAVE_GUARD, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeruleanCity_EventScript_CeruleanCaveGuardBadge::
	msgbox CeruleanCity_Text_Badge
	applymovement LOCALID_CAVE_GUARD, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeruleanCity_EventScript_CeruleanCaveGuardGyms::
	specialvar REQUIRED_BADGES_GYMS, ArchipelagoSpecial_GetCeruleanCaveRequiredCount
	buffernumberstring STR_VAR_1, REQUIRED_BADGES_GYMS
	goto_if_eq REQUIRED_BADGES_GYMS, 1, CeruleanCity_EventScript_CeruleanCaveGuardGym
	msgbox CeruleanCity_Text_Gyms
	applymovement LOCALID_CAVE_GUARD, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeruleanCity_EventScript_CeruleanCaveGuardGym::
	msgbox CeruleanCity_Text_Gym
	applymovement LOCALID_CAVE_GUARD, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeruleanCity_EventScript_Woman::
	lock
	faceplayer
	msgbox CeruleanCity_Text_WantBrightRedBicycle
	closemessage
	applymovement LOCALID_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeruleanCity_EventScript_Lass::
	lock
	random 3
	copyvar VAR_0x8008, VAR_RESULT
	call_if_eq VAR_0x8008, 0, CeruleanCity_EventScript_SlowbroCommand1
	call_if_eq VAR_0x8008, 1, CeruleanCity_EventScript_SlowbroCommand2
	call_if_eq VAR_0x8008, 2, CeruleanCity_EventScript_SlowbroCommand3
	waitmessage
	delay 40
	playse SE_PIN
	applymovement LOCALID_SLOWBRO, Common_Movement_QuestionMark
	waitmovement 0
	delay 30
	call_if_eq VAR_0x8008, 0, CeruleanCity_EventScript_SlowbroFailed1
	call_if_eq VAR_0x8008, 1, CeruleanCity_EventScript_SlowbroFailed2
	call_if_eq VAR_0x8008, 2, CeruleanCity_EventScript_SlowbroFailed3
	release
	end

CeruleanCity_EventScript_SlowbroCommand1::
	message CeruleanCity_Text_SlowbroUseSonicboom
	return

CeruleanCity_EventScript_SlowbroCommand2::
	message CeruleanCity_Text_SlowbroPunch
	return

CeruleanCity_EventScript_SlowbroCommand3::
	message CeruleanCity_Text_SlowbroWithdraw
	return

CeruleanCity_EventScript_SlowbroFailed1::
	msgbox CeruleanCity_Text_SlowbroPayAttention
	return

CeruleanCity_EventScript_SlowbroFailed2::
	msgbox CeruleanCity_Text_NoYouBlewItAgain
	return

CeruleanCity_EventScript_SlowbroFailed3::
	msgbox CeruleanCity_Text_HardToControlMonsObedience
	return

CeruleanCity_EventScript_Slowbro::
	lock
	random 4
	copyvar VAR_0x8008, VAR_RESULT
	call_if_eq VAR_0x8008, 0, CeruleanCity_EventScript_SlowbroText1
	call_if_eq VAR_0x8008, 1, CeruleanCity_EventScript_SlowbroText2
	call_if_eq VAR_0x8008, 2, CeruleanCity_EventScript_SlowbroText3
	call_if_eq VAR_0x8008, 3, CeruleanCity_EventScript_SlowbroText4
	release
	end

CeruleanCity_EventScript_SlowbroText1::
	msgbox CeruleanCity_Text_SlowbroTookSnooze
	return

CeruleanCity_EventScript_SlowbroText2::
	msgbox CeruleanCity_Text_SlowbroLoafingAround
	return

CeruleanCity_EventScript_SlowbroText3::
	msgbox CeruleanCity_Text_SlowbroTurnedAway
	return

CeruleanCity_EventScript_SlowbroText4::
	msgbox CeruleanCity_Text_SlowbroIgnoredOrders
	return

CeruleanCity_EventScript_CitySign::
	msgbox CeruleanCity_Text_CitySign, MSGBOX_SIGN
	end

CeruleanCity_EventScript_TrainerTips::
	msgbox CeruleanCity_Text_TrainerTipsHeldItems, MSGBOX_SIGN
	end

CeruleanCity_EventScript_BikeShopSign::
	msgbox CeruleanCity_Text_BikeShopSign, MSGBOX_SIGN
	end

CeruleanCity_EventScript_GymSign::
	lockall
	famechecker FAMECHECKER_MISTY, 0
	msgbox CeruleanCity_Text_GymSign
	releaseall
	end
