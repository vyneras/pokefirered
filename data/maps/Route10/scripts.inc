.set LOCALID_SCIENTIST, 7

Route10_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route10_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route10_OnFrame
	.byte 0

Route10_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_Route10Modified
	call_if_eq VAR_RESULT, TRUE, Route10_EventScript_SetAltLayout
	specialvar VAR_RESULT, ArchipelagoSpecial_PlayerEnteredThroughBlockedDoor
	call_if_eq VAR_RESULT, TRUE, Route10_EventScript_DontBlockPowerPlant
	end

Route10_EventScript_SetAltLayout::
	setmaplayoutindex LAYOUT_ROUTE10_ALT
	return

Route10_EventScript_DontBlockPowerPlant::
	setobjectxyperm LOCALID_SCIENTIST, 8, 41
	setvar VAR_TEMP_1, 1
	return

Route10_OnFrame::
	map_script_2 VAR_TEMP_1, 1, Route10_EventScript_ScientistMoveLeft
	.2byte 0

Route10_EventScript_ScientistMoveLeft::
	lockall
	applymovement LOCALID_SCIENTIST, Route10_Movement_MoveLeft
	waitmovement 0
	copyobjectxytoperm LOCALID_SCIENTIST
	setvar VAR_TEMP_1, 0
	releaseall
	end

Route10_Movement_MoveLeft::
	walk_left
	face_down
	step_end

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
