Route5_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route5_OnTransition
	.byte 0

Route5_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_BlockTunnels
	call_if_eq VAR_RESULT, FALSE, Route5_EventScript_RemoveSmashRocks
	end

Route5_EventScript_RemoveSmashRocks::
	setflag FLAG_TEMP_12
	setflag FLAG_TEMP_13
	return

Route5_EventScript_UndergroundPathSign::
	msgbox Route5_Text_UndergroundPathSign, MSGBOX_SIGN
	end
