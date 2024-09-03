.set LOCALID_TREE, 10
.set LOCALID_ROCK, 13

Route9_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route9_OnTransition
	.byte 0

Route9_OnTransition::
	call_if_set FLAG_ROUTE_9_MODIFIED, Route9_EventScript_RemoveCutTree
	call_if_unset FLAG_ROUTE_9_MODIFIED, Route9_EventScript_RemoveSmashRock
	end

Route9_EventScript_RemoveCutTree::
	setflag FLAG_TEMP_12
	return

Route9_EventScript_RemoveSmashRock::
	setflag FLAG_TEMP_13
	return

Route9_EventScript_RouteSign::
	msgbox Route9_Text_RouteSign, MSGBOX_SIGN
	end
