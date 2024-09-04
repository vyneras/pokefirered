Route6_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route6_OnTransition
	.byte 0

Route6_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_BlockTunnels
	call_if_eq VAR_RESULT, FALSE, Route6_EventScript_RemoveSmashRocks
	end

Route6_EventScript_RemoveSmashRocks::
	setflag FLAG_TEMP_12
	setflag FLAG_TEMP_13
	setflag FLAG_TEMP_14
	return

Route6_EventScript_UndergroundPathSign::
	msgbox Route6_Text_UndergroundPathSign, MSGBOX_SIGN
	end
