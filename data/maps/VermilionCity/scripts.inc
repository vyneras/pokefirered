.equ GIVE_FLY_UNLOCK, VAR_TEMP_1

VermilionCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VermilionCity_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, VermilionCity_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VermilionCity_OnFrame
	.byte 0

VermilionCity_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, VermilionCity_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, VermilionCity_EventScript_SetWorldMapFlag
	call_if_set FLAG_TALKED_TO_OAKS_AIDE_IN_VERMILION, VermilionCity_EventScript_HideOaksAide
	call_if_set FLAG_DEFEATED_BLAINE, VermilionCity_ClearCinnabarIslandBillFlag
	end

VermilionCity_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_VERMILION_CITY EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

VermilionCity_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_VERMILION_CITY
	return

VermilionCity_EventScript_HideOaksAide::
	setflag FLAG_HIDE_VERMILION_CITY_OAKS_AIDE
	return

VermilionCity_ClearCinnabarIslandBillFlag::
	specialvar VAR_RESULT, ArchipelagoSpecial_KantoOnly
	goto_if_eq VAR_RESULT, FALSE, EventScript_Return
	clearflag FLAG_HIDE_CINNABAR_POKECENTER_BILL
	return

VermilionCity_OnLoad::
	call_if_set FLAG_UNLOCKED_VERMILION_GYM_DOOR, VermilionCity_EventScript_SetGymDoorUnlocked
	end

VermilionCity_OnFrame::
	map_script_2 GIVE_FLY_UNLOCK, 1, VermilionCity_EventScript_GiveFlyUnlockItem
	.2byte 0

VermilionCity_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_VERMILION_CITY::
	giveitem ITEM_FLY_VERMILION, 1, FLAG_VISITED_VERMILION_CITY
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

VermilionCity_EventScript_GymDoor::
	lockall
	goto_if_set FLAG_UNLOCKED_VERMILION_GYM_DOOR, VermilionCity_EventScript_GymDoorAlreadyOpen
	checkitem ITEM_VERMILION_KEY
	goto_if_eq VAR_RESULT, TRUE, VermilionCity_EventScript_OpenGymDoor
	msgbox Text_DoorIsLocked
	releaseall
	end

VermilionCity_EventScript_OpenGymDoor::
	msgbox Text_InsertGymKey
	closemessage
	playse SE_UNLOCK
	waitse
	call VermilionCity_EventScript_SetGymDoorUnlocked
	special DrawWholeMapView
	setflag FLAG_UNLOCKED_VERMILION_GYM_DOOR
	msgbox Text_OpenedGymDoor
	releaseall
	end

VermilionCity_EventScript_GymDoorAlreadyOpen::
	msgbox Text_GymDoorAlreadyOpen
	releaseall
	end

VermilionCity_EventScript_SetGymDoorUnlocked::
	setmetatile 14, 25, METATILE_General_SlidingDoubleDoor, 1
	return

VermilionCity_EventScript_Woman::
	msgbox VermilionCity_Text_GrimerMultipliesInSludge, MSGBOX_NPC
	end

VermilionCity_EventScript_OldMan1::
	lock
	faceplayer
	msgbox VermilionCity_Text_DidYouSeeSSAnneInHarbor
	release
	end

VermilionCity_EventScript_FerrySailor::
	lock
	faceplayer
	setvar VAR_0x8004, SEAGALLOP_VERMILION_CITY
	specialvar VAR_RESULT, HasSeagallopPass
	goto_if_eq VAR_RESULT, TRUE, VermilionCity_EventScript_ChooseSeagallopDestPass
	checkitem ITEM_MYSTIC_TICKET
	goto_if_eq VAR_RESULT, TRUE, VermilionCity_EventScript_ChooseSeagallopDestMysticTicket
	checkitem ITEM_AURORA_TICKET
	goto_if_eq VAR_RESULT, TRUE, VermilionCity_EventScript_ChooseSeagallopDestAuroraTicket
	msgbox VermilionCity_Text_WelcomeToTheSSAnne
	release
	end

VermilionCity_EventScript_ChooseSeagallopDestPass::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	checkitem ITEM_MYSTIC_TICKET
	goto_if_eq VAR_RESULT, TRUE, VermilionCity_EventScript_ChooseSeagallopDestPassAndMysticTicket
	checkitem ITEM_AURORA_TICKET
	goto_if_eq VAR_RESULT, TRUE, VermilionCity_EventScript_ChooseSeagallopDestPassAndAuroraTicket
	setvar VAR_0x8004, SEAGALLOP_VERMILION_CITY
	message VermilionCity_Text_BoardSeagallopFerry
	waitmessage
	goto EventScript_SeviiDestinationsPage1
	end

VermilionCity_EventScript_ChooseSeagallopDestPassAndMysticTicket::
	checkitem ITEM_AURORA_TICKET
	goto_if_eq VAR_RESULT, TRUE, VermilionCity_EventScript_ChooseSeagallopDestPassAndMysticAndAuroraTicket
	message VermilionCity_Text_BoardSeagallopFerry
	waitmessage
	multichoice 17, 6, MULTICHOICE_SEVII_NAVEL, FALSE
	switch VAR_RESULT
	case 0, EventScript_SeviiDestinationsPage1
	case 1, EventScript_SailToNavelRock
	case 2, EventScript_ReleaseEnd
	case 127, EventScript_ReleaseEnd
	end

VermilionCity_EventScript_ChooseSeagallopDestPassAndAuroraTicket::
	message VermilionCity_Text_BoardSeagallopFerry
	waitmessage
	multichoice 17, 6, MULTICHOICE_SEVII_BIRTH, FALSE
	switch VAR_RESULT
	case 0, EventScript_SeviiDestinationsPage1
	case 1, EventScript_SailToBirthIsland
	case 2, EventScript_ReleaseEnd
	case 127, EventScript_ReleaseEnd
	end

VermilionCity_EventScript_ChooseSeagallopDestPassAndMysticAndAuroraTicket::
	message VermilionCity_Text_BoardSeagallopFerry
	waitmessage
	multichoice 17, 5, MULTICHOICE_SEVII_NAVEL_BIRTH, FALSE
	switch VAR_RESULT
	case 0, EventScript_SeviiDestinationsPage1
	case 1, EventScript_SailToNavelRock
	case 2, EventScript_SailToBirthIsland
	case 3, EventScript_ReleaseEnd
	case 127, EventScript_ReleaseEnd
	end

VermilionCity_EventScript_ChooseSeagallopDestMysticTicket::
	checkitem ITEM_AURORA_TICKET
	goto_if_eq VAR_RESULT, TRUE, VermilionCity_EventScript_ChooseSeagallopDestMysticAndAuroraTicket
	message VermilionCity_Text_BoardSeagallopFerry
	waitmessage
	multichoice 19, 8 MULTICHOICE_NAVEL, FALSE
	switch VAR_RESULT
	case 0, EventScript_SailToNavelRock
	case 1, EventScript_ReleaseEnd
	case 127, EventScript_ReleaseEnd
	end

VermilionCity_EventScript_ChooseSeagallopDestAuroraTicket::
	message VermilionCity_Text_BoardSeagallopFerry
	waitmessage
	multichoice 17, 8 MULTICHOICE_BIRTH, FALSE
	switch VAR_RESULT
	case 0, EventScript_SailToBirthIsland
	case 1, EventScript_ReleaseEnd
	case 127, EventScript_ReleaseEnd
	end

VermilionCity_EventScript_ChooseSeagallopDestMysticAndAuroraTicket::
	message VermilionCity_Text_BoardSeagallopFerry
	waitmessage
	multichoice 17, 6 MULTICHOICE_NAVEL_BIRTH, FALSE
	switch VAR_RESULT
	case 0, EventScript_SailToNavelRock
	case 1, EventScript_SailToBirthIsland
	case 2, EventScript_ReleaseEnd
	case 127, EventScript_ReleaseEnd
	end

EventScript_SailToNavelRock::
	msgbox VermilionCity_Text_Seagallop10Departing
	setvar VAR_0x8004, SEAGALLOP_VERMILION_CITY
	setvar VAR_0x8006, SEAGALLOP_NAVEL_ROCK
	goto EventScript_SetSail
	end

EventScript_SailToBirthIsland::
	msgbox VermilionCity_Text_Seagallop12Departing
	setvar VAR_0x8004, SEAGALLOP_VERMILION_CITY
	setvar VAR_0x8006, SEAGALLOP_BIRTH_ISLAND
	goto EventScript_SetSail
	end

VermilionCity_EventScript_CheckTicketLeft::
	lockall
	goto VermilionCity_EventScript_CheckTicket
	end

VermilionCity_EventScript_CheckTicketRight::
	lockall
	goto VermilionCity_EventScript_CheckTicket
	end

VermilionCity_EventScript_CheckTicket::
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_VERMILION_FERRY_SAILOR, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox VermilionCity_Text_DoYouHaveATicket
	checkitem ITEM_SS_TICKET
	goto_if_eq VAR_RESULT, FALSE, VermilionCity_EventScript_DontHaveSSTicket
	msgbox VermilionCity_Text_FlashedSSTicket
	setvar VAR_VERMILION_CITY_TICKET_CHECK_TRIGGER, 1
	releaseall
	end

VermilionCity_EventScript_DontHaveSSTicket::
	msgbox VermilionCity_Text_DontHaveNeededSSTicket
	closemessage
	goto_if_eq VAR_FACING, DIR_SOUTH, VermilionCity_EventScript_WalkUpPier
	goto VermilionCity_EventScript_WalkDownPier
	end

VermilionCity_EventScript_WalkUpPier::
	applymovement LOCALID_VERMILION_FERRY_SAILOR, Common_Movement_FaceOriginalDirection
	applymovement LOCALID_PLAYER, VermilionCity_Movement_WalkUp
	waitmovement 0
	releaseall
	end

VermilionCity_EventScript_WalkDownPier::
	applymovement LOCALID_VERMILION_FERRY_SAILOR, Common_Movement_FaceOriginalDirection
	applymovement LOCALID_PLAYER, VermilionCity_Movement_WalkDown
	waitmovement 0
	releaseall
	end

VermilionCity_Movement_WalkUp::
	walk_up
	step_end

VermilionCity_Movement_WalkDown::
	walk_down
	step_end

VermilionCity_EventScript_Sailor::
	msgbox VermilionCity_Text_SSAnneVisitsOnceAYear, MSGBOX_NPC
	end

VermilionCity_EventScript_OaksAide::
	lock
	faceplayer
	setflag FLAG_TALKED_TO_OAKS_AIDE_IN_VERMILION
	msgbox VermilionCity_Text_Route2AideHasPackageForYou
	release
	end

VermilionCity_EventScript_OldMan2::
	msgbox VermilionCity_Text_BuildingOnThisLand, MSGBOX_NPC
	end

VermilionCity_EventScript_Machop::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_MACHOP, CRY_MODE_NORMAL
	msgbox VermilionCity_Text_Machop
	waitmoncry
	msgbox VermilionCity_Text_MachopStompingLandFlat
	release
	end

VermilionCity_EventScript_CitySign::
	msgbox VermilionCity_Text_CitySign, MSGBOX_SIGN
	end

VermilionCity_EventScript_SnorlaxNotice::
	msgbox VermilionCity_Text_SnorlaxBlockingRoute12, MSGBOX_SIGN
	end

VermilionCity_EventScript_PokemonFanClubSign::
	msgbox VermilionCity_Text_PokemonFanClubSign, MSGBOX_SIGN
	end

VermilionCity_EventScript_GymSign::
	lockall
	famechecker FAMECHECKER_LTSURGE, 0
	msgbox VermilionCity_Text_GymSign
	releaseall
	end

VermilionCity_EventScript_HarborSign::
	msgbox VermilionCity_Text_VermilionHarbor, MSGBOX_SIGN
	end
