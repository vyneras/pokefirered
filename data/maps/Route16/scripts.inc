Route16_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Route16_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, Route16_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Route16_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route16_OnFrame
	.byte 0

Route16_OnResume::
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, Route16_EventScript_RemoveSnorlax
	end

Route16_EventScript_RemoveSnorlax::
	removeobject VAR_LAST_TALKED
	return

Route16_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_OnCyclingRoad
	call_if_eq VAR_RESULT, FALSE, Route16_OnTransitionNotCyclingRoad
	call_if_eq VAR_RESULT, TRUE, Route16_OnTransitionCyclingRoad
	specialvar VAR_RESULT, ArchipelagoSpecial_Route16Modified
	call_if_eq VAR_RESULT, TRUE, Route16_EventScript_SetAltLayout
	call_if_eq VAR_RESULT, FALSE, Route16_EventScript_RemoveRock
	call_if_unset FLAG_ROUTE_16_SNORLAX_IS_RECOVERING, Route16_EventScript_TryShowSnorlax
	end

Route16_OnTransitionNotCyclingRoad::
	setvar VAR_MAP_SCENE_ROUTE16, 0
	return

Route16_OnTransitionCyclingRoad::
	checkitem ITEM_BICYCLE
	goto_if_eq VAR_RESULT, FALSE, Route16_SetNeedBicycle
	setflag FLAG_SYS_ON_CYCLING_ROAD
	setvar VAR_MAP_SCENE_ROUTE16, 1
	return

Route16_SetNeedBicycle::
	setvar VAR_MAP_SCENE_ROUTE16, 2
	return

Route16_EventScript_SetAltLayout::
	setmaplayoutindex LAYOUT_ROUTE16_ALT
	return

Route16_EventScript_RemoveRock::
	setflag FLAG_TEMP_13
	return

Route16_EventScript_TryShowSnorlax::
	goto_if_set FLAG_CAUGHT_ROUTE_16_SNORLAX, EventScript_Return
	clearflag FLAG_HIDE_ROUTE_16_SNORLAX
	return

Route16_OnWarp::
	map_script_2 VAR_MAP_SCENE_ROUTE16, 1, Route16_OnWarpCyclingRoad
	.2byte 0

Route16_OnWarpCyclingRoad::
	special ForcePlayerOntoBike
	end

Route16_OnFrame::
	map_script_2 VAR_MAP_SCENE_ROUTE16, 2, Route16_NeedBicycle
	.2byte 0

Route16_NeedBicycle::
	lockall
	msgbox Text_NeedBicycle
	closemessage
	special ArchipelagoSpecial_UseLastWarp
	releaseall
	end

Route16_EventScript_Snorlax::
	lock
	faceplayer
	checkitem ITEM_POKE_FLUTE
	goto_if_eq VAR_RESULT, FALSE, Route16_EventScript_SnorlaxNoPokeFlute
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	msgbox Text_WantToUsePokeFlute, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route16_EventScript_DontUsePokeFlute
	call EventScript_AwakenSnorlax
Archipelago_Target_Static_Encounter_ROUTE16_SNORLAX::
	setwildbattle SPECIES_SNORLAX, 30
	waitse
	playmoncry SPECIES_SNORLAX, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_HIDE_ROUTE_16_SNORLAX
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	dowildbattle
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	setflag FLAG_FOUGHT_ROUTE_16_SNORLAX
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route16_EventScript_CaughtSnorlax
	setflag FLAG_ROUTE_16_SNORLAX_IS_RECOVERING
	msgbox Text_SnorlaxReturnedToMountains
	release
	end

Route16_EventScript_DontUsePokeFlute::
	release
	end

Route16_EventScript_CaughtSnorlax::
	setflag FLAG_CAUGHT_ROUTE_16_SNORLAX
	release
	end

Route16_EventScript_SnorlaxNoPokeFlute::
	msgbox Route16_Text_MonSprawledOutInSlumber
	release
	end

Route16_EventScript_CyclingRoadSign::
	msgbox Route16_Text_CyclingRoadSign, MSGBOX_SIGN
	end

Route16_EventScript_RouteSign::
	msgbox Route16_Text_RouteSign, MSGBOX_SIGN
	end
