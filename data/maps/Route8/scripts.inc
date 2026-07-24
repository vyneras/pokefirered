Route8_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route8_OnTransition
	.byte 0

Route8_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_BlockUndergroundPaths
	call_if_eq VAR_RESULT, FALSE, Route8_EventScript_RemoveSmashRocks
	end

Route8_EventScript_RemoveSmashRocks::
	setflag FLAG_TEMP_14
	setflag FLAG_TEMP_15
	setflag FLAG_TEMP_16
	setflag FLAG_TEMP_17
	return

Route8_EventScript_UndergroundPathSign::
	msgbox Route8_Text_UndergroundPathSign, MSGBOX_SIGN
	end
