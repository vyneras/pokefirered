.set LOCALID_FAT_MAN,   2
.set LOCALID_POLIWRATH, 3

.equ GIVE_FLY_UNLOCK, VAR_TEMP_1

CeladonCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeladonCity_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CeladonCity_OnFrame
	.byte 0

CeladonCity_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, CeladonCity_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, CeladonCity_EventScript_SetWorldMapFlag
	end

CeladonCity_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_CELADON_CITY EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

CeladonCity_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_CELADON_CITY
	return

CeladonCity_OnFrame::
	map_script_2 GIVE_FLY_UNLOCK, 1, CeladonCity_EventScript_GiveFlyUnlockItem
	.2byte 0

CeladonCity_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_CELADON_CITY::
	giveitem ITEM_FLY_CELADON, 1, FLAG_VISITED_CELADON_CITY
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

CeladonCity_EventScript_LittleGirl::
	msgbox CeladonCity_Text_GotMyKoffingInCinnabar, MSGBOX_NPC
	end

CeladonCity_EventScript_OldMan1::
	lock
	msgbox CeladonCity_Text_GymIsGreatFullOfWomen
	release
	end

CeladonCity_EventScript_Woman::
	msgbox CeladonCity_Text_GameCornerIsBadForCitysImage, MSGBOX_NPC
	end

CeladonCity_EventScript_OldMan2::
	msgbox CeladonCity_Text_BlewItAllAtSlots, MSGBOX_NPC
	end

CeladonCity_EventScript_SoftboiledTutor::
	goto EventScript_SoftboiledTutor
	end

CeladonCity_EventScript_FatMan::
	lock
	faceplayer
	msgbox CeladonCity_Text_MyTrustedPalPoliwrath
	closemessage
	applymovement LOCALID_FAT_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeladonCity_EventScript_Poliwrath::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_POLIWRATH, CRY_MODE_NORMAL
	msgbox CeladonCity_Text_Poliwrath
	closemessage
	waitmoncry
	applymovement LOCALID_POLIWRATH, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeladonCity_EventScript_RocketGrunt1::
	msgbox CeladonCity_Text_KeepOutOfTeamRocketsWay, MSGBOX_NPC
	end

CeladonCity_EventScript_RocketGrunt2::
	msgbox CeladonCity_Text_GetLostOrIllPunchYou, MSGBOX_NPC
	end

CeladonCity_EventScript_Boy::
	msgbox CeladonCity_Text_ScaldedTongueOnTea, MSGBOX_NPC
	end

CeladonCity_EventScript_SilphCoScientist::
	msgbox CeladonCity_Text_SomeoneStoleSilphScope, MSGBOX_NPC
	end

CeladonCity_EventScript_TrainerTips1::
	msgbox CeladonCity_Text_ExplainXAccuracyDireHit, MSGBOX_SIGN
	end

CeladonCity_EventScript_CitySign::
	msgbox CeladonCity_Text_CitySign, MSGBOX_SIGN
	end

CeladonCity_EventScript_GymSign::
	lockall
	famechecker FAMECHECKER_ERIKA, 0
	msgbox CeladonCity_Text_GymSign
	releaseall
	end

CeladonCity_EventScript_MansionSign::
	msgbox CeladonCity_Text_MansionSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_DeptStoreSign::
	msgbox CeladonCity_Text_DeptStoreSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_TrainerTips2::
	msgbox CeladonCity_Text_GuardSpecProtectsFromStatus, MSGBOX_SIGN
	end

CeladonCity_EventScript_PrizeExchangeSign::
	msgbox CeladonCity_Text_PrizeExchangeSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_GameCornerSign::
	msgbox CeladonCity_Text_GameCornerSign, MSGBOX_SIGN
	end
