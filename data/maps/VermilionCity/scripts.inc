.equ LOCALID_FERRY_SAILOR, 6

VermilionCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VermilionCity_OnTransition
	.byte 0

VermilionCity_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_VERMILION_CITY
	call_if_set FLAG_TALKED_TO_OAKS_AIDE_IN_VERMILION, VermilionCity_EventScript_HideOaksAide
	end

VermilionCity_EventScript_HideOaksAide::
	setflag FLAG_HIDE_VERMILION_CITY_OAKS_AIDE
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
	applymovement LOCALID_FERRY_SAILOR, Common_Movement_WalkInPlaceFasterLeft
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
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
	goto VermilionCity_EventScript_WalkUpPier
	end

VermilionCity_EventScript_WalkUpPier::
	closemessage
	applymovement LOCALID_FERRY_SAILOR, Common_Movement_FaceOriginalDirection
	applymovement OBJ_EVENT_ID_PLAYER, VermilionCity_Movement_WalkUp
	waitmovement 0
	releaseall
	end

VermilionCity_Movement_WalkUp::
	walk_up
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
