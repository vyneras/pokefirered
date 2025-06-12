.set REQUIRED_CAUGHT_MONS, VAR_TEMP_1

Route2_EastBuilding_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route2_EastBuilding_OnTransition
	.byte 0

Route2_EastBuilding_OnTransition::
	setvar VAR_0x8003, 0
	specialvar REQUIRED_CAUGHT_MONS, ArchipelagoSpecial_GetProfessorOaksAideCount
	end

Route2_EastBuilding_EventScript_Aide::
	lock
	faceplayer
Archipelago_Target_NPC_Gift_FLAG_GOT_HM05::
	setvar VAR_TEMP_2, ITEM_HM05
	goto_if_set FLAG_GOT_HM05, Route2_EastBuilding_EventScript_AlreadyGotHM05
	call Route2_EastBuilding_EventScript_GetAideRequestInfo
	call Aide_EventScript_GiveIfSeenXMons
	setvar VAR_0x8003, FLAG_HINT_ROUTE_2_OAKS_AIDE
	goto_if_eq VAR_RESULT, NO, Aide_EventScript_DeclineCheckMons
	setvar VAR_0x8004, 1
	specialvar VAR_RESULT, GetPokedexCount
	buffernumberstring STR_VAR_4, VAR_0x8006
	call Route2_EastBuilding_EventScript_GetAideRequestInfo
	setvar VAR_0x8003, FLAG_HINT_ROUTE_2_OAKS_AIDE
	goto_if_lt VAR_0x8006, REQUIRED_CAUGHT_MONS, Aide_EventScript_HaventCaughtEnough
	msgbox Route2_EastBuilding_Text_GreatHereYouGo
	giveitem VAR_TEMP_2, 1, FLAG_GOT_HM05
	msgbox Route2_EastBuilding_Text_ExplainHM05
	release
	end

Route2_EastBuilding_EventScript_AlreadyGotHM05::
	msgbox Route2_EastBuilding_Text_ExplainHM05
	release
	end

Route2_EastBuilding_EventScript_GetAideRequestInfo::
	setvar VAR_0x8003, FLAG_GOT_HM05
	buffernumberstring STR_VAR_1, REQUIRED_CAUGHT_MONS
	bufferapitemstrings STR_VAR_2, STR_VAR_3, VAR_0x8003, VAR_TEMP_2
	copyvar VAR_TEMP_3, VAR_RESULT
	return

Route2_EastBuilding_EventScript_Rocker::
	msgbox Route2_EastBuilding_Text_CanGetThroughRockTunnel, MSGBOX_NPC
	end
