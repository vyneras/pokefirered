BirthIsland_Harbor_MapScripts::
	.byte 0

BirthIsland_Harbor_EventScript_Sailor::
	lock
	faceplayer
	setvar VAR_0x8004, SEAGALLOP_BIRTH_ISLAND
	specialvar VAR_RESULT, HasSeagallopPass
	goto_if_eq VAR_RESULT, FALSE, EventScript_CantSail
	message Text_WhereDoYouWantToSail
	waitmessage
	multichoice 20, 8, MULTICHOICE_SEAGALLOP_VERMILION, FALSE
	switch VAR_RESULT
	case 0, EventScript_SailToVermilionCity
	case 1, EventScript_ReleaseEnd
	case 127, EventScript_ReleaseEnd
	end
