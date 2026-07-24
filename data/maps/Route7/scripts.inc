Route7_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route7_OnTransition
	.byte 0

Route7_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_BlockUndergroundPaths
	call_if_eq VAR_RESULT, FALSE, Route7_EventScript_RemoveSmashRocks
	end

Route7_EventScript_RemoveSmashRocks::
	setflag FLAG_TEMP_12
	setflag FLAG_TEMP_13
	setflag FLAG_TEMP_14
	setflag FLAG_TEMP_15
	setflag FLAG_TEMP_16
	return

Route7_EventScript_UndergroundPathSign::
	msgbox Route7_Text_UndergroundPathSign, MSGBOX_SIGN
	end
