FourIsland_Harbor_MapScripts::
	.byte 0

FourIsland_Harbor_EventScript_Sailor::
	lock
	faceplayer
	setvar VAR_0x8004, SEAGALLOP_FOUR_ISLAND
	specialvar VAR_RESULT, HasSeagallopPass
	goto_if_eq VAR_RESULT, FALSE, EventScript_CantSail
	message Text_WhereDoYouWantToSail
	waitmessage
	goto EventScript_SeviiDestinationsPage1
	end
