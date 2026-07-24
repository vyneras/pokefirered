Route9_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route9_OnTransition
	.byte 0

Route9_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_Route9Roadblock
	call_if_eq VAR_RESULT, 1, Route9_EventScript_RemoveCutTree
	call_if_eq VAR_RESULT, 0, Route9_EventScript_RemoveSmashRock
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
