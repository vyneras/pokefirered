Route3_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route3_OnTransition
	.byte 0

Route3_OnTransition::
	call_if_eq VAR_MAP_SCENE_PEWTER_CITY_GYM_GUIDE, 0, Route3_EventScript_CheckIfCanLeavePewterCity
	call_if_set FLAG_DEFEATED_BROCK, Route3_EventScript_CheckRunningShoesGuyTrigger
	end

Route3_EventScript_CheckIfCanLeavePewterCity::
	specialvar VAR_RESULT, ArchipelagoSpecial_CanLeavePewterCity
	call_if_eq VAR_RESULT, TRUE, Route3_EventScript_SetCanLeavePewterCity
	return

Route3_EventScript_SetCanLeavePewterCity::
	setvar VAR_MAP_SCENE_PEWTER_CITY_GYM_GUIDE, 1
	setflag FLAG_HIDE_PEWTER_CITY_GYM_GUIDE
	return

Route3_EventScript_CheckRunningShoesGuyTrigger::
	call_if_lt VAR_MAP_SCENE_PEWTER_CITY_OAKS_AIDE, 1, Route3_EventScript_SetRunningShoesGuyTrigger
	return

Route3_EventScript_SetRunningShoesGuyTrigger::
	setvar VAR_MAP_SCENE_PEWTER_CITY_OAKS_AIDE, 1
	return

Route3_EventScript_Youngster::
	msgbox Route3_Text_TunnelFromCeruleanTiring, MSGBOX_NPC
	end

Route3_EventScript_RouteSign::
	msgbox Route3_Text_RouteSign, MSGBOX_SIGN
	end
