Route2_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route2_OnTransition
	.byte 0

Route2_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_DiglettsCaveRoadblock
	call_if_eq VAR_RESULT, 1, Route2_EventScript_RemoveCutTree
	call_if_eq VAR_RESULT, 0, Route2_EventScript_RemoveSmashRock
	end

Route2_EventScript_RemoveCutTree::
	setflag FLAG_TEMP_15
	return

Route2_EventScript_RemoveSmashRock:
	setflag FLAG_TEMP_17
	return

Route2_EventScript_RouteSign::
	msgbox Route2_Text_RouteSign, MSGBOX_SIGN
	end

Route2_EventScript_DiglettsCaveSign::
	msgbox Route2_Text_DiglettsCave, MSGBOX_SIGN
	end
