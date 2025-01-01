Route12_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Route12_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, Route12_OnTransition
	.byte 0

Route12_OnResume::
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, Route12_EventScript_TryRemoveSnorlax
	end

Route12_EventScript_TryRemoveSnorlax::
	removeobject VAR_LAST_TALKED
	return

Route12_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_Route12Modified
	call_if_eq VAR_RESULT, TRUE, Route12_EventScript_SetAltLayout
	call_if_unset FLAG_ROUTE_12_SNORLAX_IS_RECOVERING, Route12_EventScript_TryShowSnorlax
	end

Route12_EventScript_SetAltLayout::
	setmaplayoutindex LAYOUT_ROUTE12_ALT
	return

Route12_EventScript_TryShowSnorlax::
	goto_if_set FLAG_CAUGHT_ROUTE_12_SNORLAX, EventScript_Return
	clearflag FLAG_HIDE_ROUTE_12_SNORLAX
	return

Route12_EventScript_Snorlax::
	lock
	faceplayer
	checkitem ITEM_POKE_FLUTE
	goto_if_eq VAR_RESULT, FALSE, Route12_EventScript_SnorlaxNoPokeFlute
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	msgbox Text_WantToUsePokeFlute, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route12_EventScript_DontUsePokeFlute
	call EventScript_AwakenSnorlax
Archipelago_Target_Static_Encounter_ROUTE12_SNORLAX::
	setwildbattle SPECIES_SNORLAX, 30
	waitse
	playmoncry SPECIES_SNORLAX, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_HIDE_ROUTE_12_SNORLAX
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	setflag FLAG_WOKE_UP_ROUTE_12_SNORLAX
	dowildbattle
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	msgbox Text_SnorlaxReturnedToMountains
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, Route12_EventScript_CaughtSnorlax
	setflag FLAG_ROUTE_12_SNORLAX_IS_RECOVERING
	release
	end

Route12_EventScript_DontUsePokeFlute::
	release
	end

Route12_EventScript_CaughtSnorlax::
	setflag FLAG_CAUGHT_ROUTE_12_SNORLAX
	release
	end

Route12_EventScript_SnorlaxNoPokeFlute::
	msgbox Route12_Text_MonSprawledOutInSlumber
	release
	end

Route12_EventScript_RouteSign::
	msgbox Route12_Text_RouteSign, MSGBOX_SIGN
	end

Route12_EventScript_FishingSign::
	msgbox Route12_Text_SportfishingArea, MSGBOX_SIGN
	end
