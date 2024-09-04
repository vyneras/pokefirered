.set LOCALID_SCIENTIST, 7

Route10_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route10_OnTransition
	.byte 0

Route10_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_Route10Modified
	call_if_eq VAR_RESULT, TRUE, Route10_EventScript_SetAltLayout
	end

Route10_EventScript_SetAltLayout::
	setmaplayoutindex LAYOUT_ROUTE10_ALT
	return

Route10_EventScript_NorthRockTunnelSign::
	msgbox Route10_Text_RockTunnelDetourToLavender, MSGBOX_SIGN
	end

Route10_EventScript_SouthRockTunnelSign::
	msgbox Route10_Text_RockTunnel, MSGBOX_SIGN
	end

Route10_EventScript_PowerPlantSign::
	msgbox Route10_Text_PowerPlant, MSGBOX_SIGN
	end

Route10_EventScript_Scientist::
	lock
	faceplayer
	checkitem ITEM_MACHINE_PART
	goto_if_eq VAR_RESULT, TRUE, Route10_EventScript_ScientistGiveMachinePart
	msgbox Route10_Text_NoMachinePart
	release
	end

Route10_EventScript_ScientistGiveMachinePart::
	removeitem ITEM_MACHINE_PART
	msgbox Route10_Text_FoundMachinePart
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_TMHM
	message Route10_Text_GiveMachinePart
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	closemessage
	msgbox Route10_Text_GetBackToWork
	closemessage
	applymovement LOCALID_SCIENTIST, Route10_Movement_ScientistExit
	waitmovement 0
	playse SE_EXIT
	removeobject LOCALID_SCIENTIST
	release
	end

Route10_Movement_ScientistExit::
	walk_up
	delay_8
	set_invisible
	step_end
