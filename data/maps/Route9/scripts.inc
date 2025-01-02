Route9_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route9_OnTransition
	.byte 0

Route9_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_Route9Modified
	call_if_eq VAR_RESULT, TRUE, Route9_EventScript_RemoveCutTree
	call_if_eq VAR_RESULT, FALSE, Route9_EventScript_RemoveSmashRock
	end

Route9_EventScript_RemoveCutTree::
	setflag FLAG_TEMP_12
	return

Route9_EventScript_RemoveSmashRock::
	setflag FLAG_TEMP_14
	return

Route9_EventScript_RouteSign::
	msgbox Route9_Text_RouteSign, MSGBOX_SIGN
	end
