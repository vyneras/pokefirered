.set LOCALID_OLD_MAN,      3
.set LOCALID_TUTORIAL_MAN, 4
.set LOCALID_WOMAN,        5
.set REQUIRED_BADGES_GYMS, VAR_TEMP_1

.equ GIVE_FLY_UNLOCK, VAR_TEMP_2

ViridianCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianCity_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, ViridianCity_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, ViridianCity_OnFrame
	.byte 0

ViridianCity_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, ViridianCity_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, ViridianCity_EventScript_SetWorldMapFlag
	call_if_unset FLAG_UNLOCKED_VIRIDIAN_GYM_DOOR, ViridianCity_EventScript_TryUnlockGym
	call_if_ge VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 2, ViridianCity_EventScript_SetOldManNormal
	call_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 1, ViridianCity_EventScript_SetOldManStandingByRoad
	call_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 0, ViridianCity_EventScript_SetOldManBlockingRoad
	end

ViridianCity_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_VIRIDIAN_CITY EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

ViridianCity_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_VIRIDIAN_CITY
	return

ViridianCity_EventScript_TryUnlockGym::
	specialvar VAR_RESULT, ArchipelagoSpecial_GymKeys
	goto_if_eq VAR_RESULT, TRUE, EventScript_Return
	specialvar VAR_RESULT, ArchipelagoSpecial_CanChallengeGiovanni
	goto_if_eq VAR_RESULT, FALSE, EventScript_Return
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_GYM_DOOR, 1
	return

ViridianCity_EventScript_SetOldManNormal::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_OLD_MAN_1
	return

ViridianCity_EventScript_OldManNotBlockingRoad::
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 1
	call ViridianCity_EventScript_SetOldManStandingByRoad
	end

ViridianCity_EventScript_SetOldManStandingByRoad::
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_OLD_MAN_1
	setobjectxyperm LOCALID_TUTORIAL_MAN, 21, 8
	setobjectmovementtype LOCALID_TUTORIAL_MAN, MOVEMENT_TYPE_LOOK_AROUND
	return

ViridianCity_EventScript_SetOldManBlockingRoad::
	goto_if_set FLAG_DELIVERED_OAKS_PARCEL, ViridianCity_EventScript_OldManNotBlockingRoad
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_OLD_MAN_LYING_DOWN
	setobjectxyperm LOCALID_TUTORIAL_MAN, 21, 11
	setobjectmovementtype LOCALID_TUTORIAL_MAN, MOVEMENT_TYPE_FACE_DOWN
	return

ViridianCity_OnLoad::
	call_if_set FLAG_UNLOCKED_VIRIDIAN_GYM_DOOR, ViridianCity_EventScript_SetGymDoorUnlocked
	end

ViridianCity_OnFrame::
	map_script_2 GIVE_FLY_UNLOCK, 1, ViridianCity_EventScript_GiveFlyUnlockItem
	map_script_2 VAR_MAP_SCENE_VIRIDIAN_CITY_GYM_DOOR, 1, ViridianCity_EventScript_UnlockGymDoor
	.2byte 0

ViridianCity_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_VIRIDIAN_CITY::
	giveitem ITEM_FLY_VIRIDIAN, 1, FLAG_VISITED_VIRIDIAN_CITY
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

ViridianCity_EventScript_GymDoor::
	lockall
	goto_if_set FLAG_UNLOCKED_VIRIDIAN_GYM_DOOR, ViridianCity_EventScript_GymDoorAlreadyOpen
	specialvar VAR_RESULT, ArchipelagoSpecial_CanChallengeGiovanni
	goto_if_eq VAR_RESULT, FALSE, ViridianCity_EventScript_GymDoorNoLeader
	checkitem ITEM_VIRIDIAN_KEY
	goto_if_eq VAR_RESULT, TRUE, ViridianCity_EventScript_OpenGymDoor
	msgbox Text_DoorIsLocked
	releaseall
	end

ViridianCity_EventScript_GymDoorAlreadyOpen::
	msgbox Text_GymDoorAlreadyOpen
	releaseall
	end

ViridianCity_EventScript_GymDoorNoLeader::
	msgbox ViridianCity_Text_GymDoorsAreLocked
	releaseall
	end

ViridianCity_EventScript_OpenGymDoor::
	msgbox Text_InsertGymKey
	closemessage
	playse SE_UNLOCK
	waitse
	call ViridianCity_EventScript_UnlockGymDoor
	msgbox Text_OpenedGymDoor
	releaseall
	end

ViridianCity_EventScript_UnlockGymDoor::
	call ViridianCity_EventScript_SetGymDoorUnlocked
	special DrawWholeMapView
	setflag FLAG_UNLOCKED_VIRIDIAN_GYM_DOOR
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_GYM_DOOR, 2
	return

ViridianCity_EventScript_SetGymDoorUnlocked::
	setmetatile 36, 10, METATILE_General_SlidingDoubleDoor, 1
	return

ViridianCity_EventScript_CitySign::
	msgbox ViridianCity_Text_CitySign, MSGBOX_SIGN
	end

ViridianCity_EventScript_TrainerTips1::
	msgbox ViridianCity_Text_CatchMonsForEasierBattles, MSGBOX_SIGN
	end

ViridianCity_EventScript_TrainerTips2::
	msgbox ViridianCity_Text_MovesLimitedByPP, MSGBOX_SIGN
	end

ViridianCity_EventScript_GymSign::
	msgbox ViridianCity_Text_GymSign, MSGBOX_SIGN
	end

ViridianCity_EventScript_Boy::
	msgbox ViridianCity_Text_CanCarryMonsAnywhere, MSGBOX_NPC
	end

@ Other old man, not the tutorial old man
ViridianCity_EventScript_OldMan::
	lock
	faceplayer
	specialvar VAR_RESULT, ArchipelagoSpecial_CanChallengeGiovanni
	goto_if_eq VAR_RESULT, FALSE, ViridianCity_EventScript_OldManGymClosed
	msgbox ViridianCity_Text_ViridiansGymLeaderReturned
	specialvar VAR_RESULT, ArchipelagoSpecial_GymKeys
	call_if_eq VAR_RESULT, TRUE, ViridianCity_EventScript_OldManGiveViridianKey
	release
	end

ViridianCity_EventScript_OldManGiveViridianKey::
	goto_if_set FLAG_GOT_VIRIDIAN_KEY, EventScript_Return
	msgbox ViridianCity_Text_YoullNeedThis
Archipelago_Target_NPC_Gift_FLAG_GOT_VIRIDIAN_KEY::
	giveitem ITEM_VIRIDIAN_KEY, 1, FLAG_GOT_VIRIDIAN_KEY
	return

ViridianCity_EventScript_OldManGymClosed::
	specialvar REQUIRED_BADGES_GYMS, ArchipelagoSpecial_GetGiovanniRequiredCount
	buffernumberstring STR_VAR_1, REQUIRED_BADGES_GYMS
	specialvar VAR_RESULT, ArchipelagoSpecial_GiovanniRequiresGyms
	goto_if_eq VAR_RESULT, TRUE, ViridianCity_EventScript_OldManGymClosedGyms
	goto_if_eq REQUIRED_BADGES_GYMS, 1, ViridianCity_EventScript_OldManGymClosedNoBadge
	msgbox ViridianCity_Text_OnlyOpenWithXBadges
	closemessage
	applymovement LOCALID_OLD_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

ViridianCity_EventScript_OldManGymClosedNoBadge::
	msgbox ViridianCity_Text_OnlyOpenWithBadge
	closemessage
	applymovement LOCALID_OLD_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

ViridianCity_EventScript_OldManGymClosedGyms::
	goto_if_eq REQUIRED_BADGES_GYMS, 1, ViridianCity_EventScript_OldManGymClosedNoGym
	msgbox ViridianCity_Text_OnlyOpenWithXGyms
	closemessage
	applymovement LOCALID_OLD_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

ViridianCity_EventScript_OldManGymClosedNoGym::
	msgbox ViridianCity_Text_OnlyOpenWithGym
	closemessage
	applymovement LOCALID_OLD_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

ViridianCity_EventScript_TutorialOldMan::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 2, ViridianCity_EventScript_TutorialCompleted
	goto_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 1, ViridianCity_EventScript_TutorialStart
	goto_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 0, ViridianCity_EventScript_TutorialNotReady
	end

ViridianCity_EventScript_AskIfTeachyTVHelpful::
	msgbox ViridianCity_Text_HowsTeachyTVHelping, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianCity_EventScript_TeachyTVNotHelpful
	msgbox ViridianCity_Text_MyGrandsonOnTheShow
	release
	end

ViridianCity_EventScript_TeachyTVNotHelpful::
	msgbox ViridianCity_Text_TooBusyForTeachyTV
	release
	end

ViridianCity_EventScript_TutorialCompleted::
	goto_if_set FLAG_BADGE01_GET, ViridianCity_EventScript_AskIfTeachyTVHelpful
	msgbox ViridianCity_Text_WeakenMonsFirstToCatch
	release
	end

ViridianCity_EventScript_TutorialStart::
	call ViridianCity_EventScript_DoTutorialBattle
	release
	end

ViridianCity_EventScript_TutorialNotReady::
	msgbox ViridianCity_Text_ThisIsPrivateProperty
	closemessage
	release
	end

ViridianCity_EventScript_Youngster::
	lock
	faceplayer
	msgbox ViridianCity_Text_WantToKnowAboutCaterpillarMons, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, ViridianCity_EventScript_YoungsterExplainCaterpillars
	goto_if_eq VAR_RESULT, NO, ViridianCity_EventScript_YoungsterDeclineExplanation
	end

ViridianCity_EventScript_YoungsterExplainCaterpillars::
	msgbox ViridianCity_Text_ExplainCaterpieWeedle
	release
	end

ViridianCity_EventScript_YoungsterDeclineExplanation::
	msgbox ViridianCity_Text_OhOkayThen
	release
	end

ViridianCity_EventScript_Woman::
	lock
	faceplayer
	goto_if_eq VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 0, ViridianCity_EventScript_WomanRoadBlocked
	msgbox ViridianCity_Text_GoShoppingInPewterOccasionally
	release
	end

ViridianCity_EventScript_WomanRoadBlocked::
	msgbox ViridianCity_Text_GrandpaHasntHadCoffeeYet
	closemessage
	applymovement LOCALID_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

ViridianCity_EventScript_DreamEaterTutor::
	goto EventScript_DreamEaterTutor
	end

ViridianCity_EventScript_RoadBlocked::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	msgbox ViridianCity_Text_ThisIsPrivateProperty
	closemessage
	goto_if_eq VAR_FACING, DIR_SOUTH, ViridianCity_EventScript_RoadBlocked_WalkUp
	applymovement OBJ_EVENT_ID_PLAYER, ViridianCity_Movement_WalkDown
	waitmovement 0
	releaseall
	end

ViridianCity_EventScript_RoadBlocked_WalkUp::
	applymovement OBJ_EVENT_ID_PLAYER, ViridianCity_Movement_WalkUp
	waitmovement 0
	releaseall
	end

ViridianCity_Movement_WalkDown::
	walk_down
	step_end

ViridianCity_Movement_WalkUp::
	walk_up
	step_end

ViridianCity_EventScript_TutorialTriggerLeft::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_TUTORIAL_MAN, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	call ViridianCity_EventScript_DoTutorialBattle
	release
	end

ViridianCity_EventScript_TutorialTriggerRight::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_TUTORIAL_MAN, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	call ViridianCity_EventScript_DoTutorialBattle
	release
	end

ViridianCity_EventScript_DoTutorialBattle::
	msgbox ViridianCity_Text_TakeThis
	closemessage
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 2
Archipelago_Target_NPC_Gift_FLAG_GOT_TEACHY_TV::
	giveitem ITEM_TEACHY_TV, 1, FLAG_GOT_TEACHY_TV
	msgbox ViridianCity_Text_WatchThatToLearnBasics
	return
