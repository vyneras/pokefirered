EventScript_SailToDest::
	specialvar VAR_RESULT, GetSeagallopNumber
	buffernumberstring STR_VAR_1, VAR_RESULT
	goto_if_eq VAR_0x8004, SEAGALLOP_VERMILION_CITY, EventScript_DepartingVermilion
	goto_if_ne VAR_0x8004, SEAGALLOP_VERMILION_CITY, EventScript_DepartingNotVermilion
	end

EventScript_DepartingVermilion::
	msgbox VermilionCity_Text_Seagallop7Departing
	goto EventScript_SetSail
	end

EventScript_DepartingNotVermilion::
	msgbox Text_AllAboardSeagallopNum
	goto EventScript_SetSail
	end

EventScript_SetSail::
	closemessage
	delay 20
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	fadescreen FADE_TO_BLACK
	special DoSeagallopFerryScene
	waitstate
	end

EventScript_SeviiDestinationsPage1::
	setvar VAR_0x8005, 0
	special DrawSeagallopDestinationMenu
	waitstate
	specialvar VAR_0x8006, GetSelectedSeagallopDestination
	switch VAR_0x8006
	case SEAGALLOP_VERMILION_CITY, EventScript_SailToVermilionCity
	case SEAGALLOP_ONE_ISLAND, EventScript_SailToOneIsland
	case SEAGALLOP_TWO_ISLAND, EventScript_SailToTwoIsland
	case SEAGALLOP_THREE_ISLAND, EventScript_SailToThreeIsland
	case SEAGALLOP_FOUR_ISLAND,	EventScript_SailToFourIsland
	case SEAGALLOP_FIVE_ISLAND, EventScript_SailToFiveIsland
	case SEAGALLOP_SIX_ISLAND, EventScript_SailToSixIsland
	case SEAGALLOP_SEVEN_ISLAND, EventScript_SailToSevenIsland
	case SEAGALLOP_MORE, EventScript_SeviiDestinationsPage2
	case SCR_MENU_CANCEL, EventScript_ReleaseEnd
	end

EventScript_SeviiDestinationsPage2::
	setvar VAR_0x8005, 1
	special DrawSeagallopDestinationMenu
	waitstate
	specialvar VAR_0x8006, GetSelectedSeagallopDestination
	switch VAR_0x8006
	case SEAGALLOP_VERMILION_CITY, EventScript_SailToVermilionCity
	case SEAGALLOP_ONE_ISLAND, EventScript_SailToOneIsland
	case SEAGALLOP_TWO_ISLAND, EventScript_SailToTwoIsland
	case SEAGALLOP_THREE_ISLAND, EventScript_SailToThreeIsland
	case SEAGALLOP_FOUR_ISLAND,	EventScript_SailToFourIsland
	case SEAGALLOP_FIVE_ISLAND, EventScript_SailToFiveIsland
	case SEAGALLOP_SIX_ISLAND, EventScript_SailToSixIsland
	case SEAGALLOP_SEVEN_ISLAND, EventScript_SailToSevenIsland
	case SEAGALLOP_MORE, EventScript_SeviiDestinationsPage1
	case SCR_MENU_CANCEL, EventScript_ReleaseEnd
	end

EventScript_SailToVermilionCity::
	setvar VAR_0x8006, SEAGALLOP_VERMILION_CITY
	goto EventScript_SailToDest
	end

EventScript_SailToOneIsland::
	setvar VAR_0x8006, SEAGALLOP_ONE_ISLAND
	goto EventScript_SailToDest
	end

EventScript_SailToTwoIsland::
	setvar VAR_0x8006, SEAGALLOP_TWO_ISLAND
	goto EventScript_SailToDest
	end

EventScript_SailToThreeIsland::
	setvar VAR_0x8006, SEAGALLOP_THREE_ISLAND
	goto EventScript_SailToDest
	end

EventScript_SailToFourIsland::
	setvar VAR_0x8006, SEAGALLOP_FOUR_ISLAND
	goto EventScript_SailToDest
	end

EventScript_SailToFiveIsland::
	setvar VAR_0x8006, SEAGALLOP_FIVE_ISLAND
	goto EventScript_SailToDest
	end

EventScript_SailToSixIsland::
	setvar VAR_0x8006, SEAGALLOP_SIX_ISLAND
	goto EventScript_SailToDest
	end

EventScript_SailToSevenIsland::
	setvar VAR_0x8006, SEAGALLOP_SEVEN_ISLAND
	goto EventScript_SailToDest
	end

EventScript_CantSail::
	msgbox Text_CantSail, MSGBOX_NPC
	release
	end
