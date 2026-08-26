Route8_WestEntrance_MapScripts::
	.byte 0

Route8_WestEntrance_EventScript_Guard::
	msgbox Route8_WestEntrance_Text_HiHowsItGoing, MSGBOX_NPC
	end

Route8_WestEntrance_EventScript_GuardTriggerTop::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route8_WestEntrance_EventScript_GuardTrigger
	end

Route8_WestEntrance_EventScript_GuardTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	goto Route8_WestEntrance_EventScript_GuardTrigger
	end

Route8_WestEntrance_EventScript_GuardTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 2
	goto Route8_WestEntrance_EventScript_GuardTrigger
	end

Route8_WestEntrance_EventScript_GuardTrigger::
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	checkitem ITEM_TEA
	goto_if_eq VAR_RESULT, TRUE, Route8_WestEntrance_EventScript_GiveTea
	checkitem ITEM_PURPLE_TEA
	goto_if_eq VAR_RESULT, TRUE, Route8_WestEntrance_EventScript_GivePurpleTea
	msgbox Route8_WestEntrance_Text_ThirstyOnGuardDuty
	closemessage
	goto_if_eq VAR_FACING, DIR_EAST, Route8_WestEntrance_EventScript_MovePlayerLeft
	applymovement LOCALID_PLAYER, Route8_WestEntrance_Movement_BlockPlayerEntryRight
	waitmovement 0
	releaseall
	end

Route8_WestEntrance_EventScript_GiveTea::
	removeitem ITEM_TEA
	call Route8_WestEntrance_EventScript_GuardDrinkTea
	msgbox Route8_WestEntrance_Text_ThanksIllShareTeaWithGuards
	setvar VAR_MAP_SCENE_ROUTE5_GATE, 1
	setvar VAR_MAP_SCENE_ROUTE6_GATE, 1
	setvar VAR_MAP_SCENE_ROUTE7_GATE, 1
	setvar VAR_MAP_SCENE_ROUTE8_GATE, 1
	releaseall
	end

Route8_WestEntrance_EventScript_GuardDrinkTea::
	msgbox Route8_WestEntrance_Text_ThatTeaLooksTasty
	closemessage
	call_if_eq VAR_TEMP_1, 0, Route8_WestEntrance_EventScript_WalkToGuardTop
	call_if_eq VAR_TEMP_1, 1, Route8_WestEntrance_EventScript_WalkToGuardMid
	call_if_eq VAR_TEMP_1, 2, Route8_WestEntrance_EventScript_WalkToGuardBottom
	return

Route8_WestEntrance_EventScript_GivePurpleTea::
	removeitem ITEM_PURPLE_TEA
	call Route8_WestEntrance_EventScript_GuardDrinkTea
	msgbox Route8_WestEntrance_Text_ThanksForTea
	setvar VAR_MAP_SCENE_ROUTE8_GATE, 1
	releaseall
	end

Route8_WestEntrance_EventScript_MovePlayerLeft::
	applymovement LOCALID_PLAYER, Route8_WestEntrance_Movement_BlockPlayerEntryLeft
	waitmovement 0
	releaseall
	end

Route8_WestEntrance_EventScript_WalkToGuardTop::
	applymovement LOCALID_PLAYER, Route8_WestEntrance_Movement_WalkToGuardTop
	waitmovement 0
	return

Route8_WestEntrance_EventScript_WalkToGuardMid::
	applymovement LOCALID_PLAYER, Route8_WestEntrance_Movement_WalkToGuardMid
	waitmovement 0
	return

Route8_WestEntrance_EventScript_WalkToGuardBottom::
	applymovement LOCALID_PLAYER, Route8_WestEntrance_Movement_WalkToGuardBottom
	waitmovement 0
	return

Route8_WestEntrance_Movement_WalkToGuardMid::
	walk_up
	step_end

Route8_WestEntrance_Movement_WalkToGuardBottom::
	walk_up
	walk_up
	step_end

Route8_WestEntrance_Movement_WalkToGuardTop::
	walk_in_place_up
	step_end

Route8_WestEntrance_Movement_BlockPlayerEntryRight::
	walk_right
	step_end

Route8_WestEntrance_Movement_BlockPlayerEntryLeft::
	walk_left
	step_end
