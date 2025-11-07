Route18_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route18_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Route18_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route18_OnFrame
	.byte 0

Route18_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_OnCyclingRoad
	call_if_eq VAR_RESULT, FALSE, Route18_OnTransitionNotCyclingRoad
	call_if_eq VAR_RESULT, TRUE, Route18_OnTransitionCyclingRoad
	end

Route18_OnTransitionNotCyclingRoad::
	setvar VAR_MAP_SCENE_ROUTE16, 0
	return

Route18_OnTransitionCyclingRoad::
	checkitem ITEM_BICYCLE
	goto_if_eq VAR_RESULT, FALSE, Route18_SetNeedBicycle
	setflag FLAG_SYS_ON_CYCLING_ROAD
	setvar VAR_MAP_SCENE_ROUTE16, 1
	return

Route18_SetNeedBicycle::
	setvar VAR_MAP_SCENE_ROUTE16, 2
	return

Route18_OnWarp::
	map_script_2 VAR_MAP_SCENE_ROUTE16, 1, Route18_OnWarpCyclingRoad
	.2byte 0

Route18_OnWarpCyclingRoad::
	special ForcePlayerOntoBike
	end

Route18_OnFrame::
	map_script_2 VAR_MAP_SCENE_ROUTE16, 2, Route16_NeedBicycle
	.2byte 0

Route18_NeedBicycle::
	lockall
	msgbox Text_NeedBicycle
	closemessage
	special ArchipelagoSpecial_UseLastWarp
	releaseall
	end

Route18_EventScript_RouteSign::
	msgbox Route18_Text_RouteSign, MSGBOX_SIGN
	end

Route18_EventScript_CyclingRoadSign::
	msgbox Route18_Text_CyclingRoadSign, MSGBOX_SIGN
	end
