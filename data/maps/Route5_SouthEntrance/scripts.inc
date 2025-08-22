Route5_SouthEntrance_MapScripts::
	.byte 0

Route5_SouthEntrance_EventScript_Guard::
	msgbox Route5_SouthEntrance_Text_HiHowsItGoing, MSGBOX_NPC
	end

Route5_SouthEntrance_EventScript_GuardTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route5_SouthEntrance_EventScript_GuardTrigger
	end

Route5_SouthEntrance_EventScript_GuardTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	goto Route5_SouthEntrance_EventScript_GuardTrigger
	end

Route5_SouthEntrance_EventScript_GuardTriggerRight::
	lockall
	setvar VAR_TEMP_1, 2
	goto Route5_SouthEntrance_EventScript_GuardTrigger
	end

Route5_SouthEntrance_EventScript_GuardTrigger::
	textcolor NPC_TEXT_COLOR_MALE
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	checkitem ITEM_TEA
	goto_if_eq VAR_RESULT, TRUE, Route5_SouthEntrance_EventScript_GiveTea
	checkitem ITEM_BLUE_TEA
	goto_if_eq VAR_RESULT, TRUE, Route5_SouthEntrance_EventScript_GiveBlueTea
	msgbox Route5_SouthEntrance_Text_ThirstyOnGuardDuty
	closemessage
	goto_if_eq VAR_FACING, DIR_NORTH, Route5_SouthEntrance_EventScript_MovePlayerDown
	applymovement OBJ_EVENT_ID_PLAYER, Route5_SouthEntrance_Movement_BlockPlayerEntryTop
	waitmovement 0
	releaseall
	end

Route5_SouthEntrance_EventScript_GiveTea::
	removeitem ITEM_TEA
	call Route5_SouthEntrance_EventScript_GuardDrinkTea
	msgbox Route5_SouthEntrance_Text_ThanksIllShareTeaWithGuards
	setvar VAR_MAP_SCENE_ROUTE5_GATE, 1
	setvar VAR_MAP_SCENE_ROUTE6_GATE, 1
	setvar VAR_MAP_SCENE_ROUTE7_GATE, 1
	setvar VAR_MAP_SCENE_ROUTE8_GATE, 1
	releaseall
	end

Route5_SouthEntrance_EventScript_GuardDrinkTea::
	msgbox Route5_SouthEntrance_Text_ThatTeaLooksTasty
	closemessage
	call_if_eq VAR_TEMP_1, 0, Route5_SouthEntrance_EventScript_WalkToGuardLeft
	call_if_eq VAR_TEMP_1, 1, Route5_SouthEntrance_EventScript_WalkToGuardMid
	call_if_eq VAR_TEMP_1, 2, Route5_SouthEntrance_EventScript_WalkToGuardRight
	return

Route5_SouthEntrance_EventScript_GiveBlueTea::
	removeitem ITEM_BLUE_TEA
	call Route5_SouthEntrance_EventScript_GuardDrinkTea
	msgbox Route5_SouthEntrance_Text_ThanksForTea
	setvar VAR_MAP_SCENE_ROUTE5_GATE, 1
	releaseall
	end

Route5_SouthEntrance_EventScript_MovePlayerDown::
	applymovement OBJ_EVENT_ID_PLAYER, Route5_SouthEntrance_Movement_BlockPlayerEntryBottom
	waitmovement 0
	releaseall
	end

Route5_SouthEntrance_EventScript_WalkToGuardLeft::
	applymovement OBJ_EVENT_ID_PLAYER, Route5_SouthEntrance_Movement_WalkToGuardLeft
	waitmovement 0
	return

Route5_SouthEntrance_EventScript_WalkToGuardMid::
	applymovement OBJ_EVENT_ID_PLAYER, Route5_SouthEntrance_Movement_WalkToGuardMid
	waitmovement 0
	return

Route5_SouthEntrance_EventScript_WalkToGuardRight::
	applymovement OBJ_EVENT_ID_PLAYER, Route5_SouthEntrance_Movement_WalkToGuardRight
	waitmovement 0
	return

Route5_SouthEntrance_Movement_WalkToGuardMid::
	walk_left
	step_end

Route5_SouthEntrance_Movement_WalkToGuardRight::
	walk_left
	walk_left
	step_end

Route5_SouthEntrance_Movement_WalkToGuardLeft::
	walk_in_place_left
	step_end

Route5_SouthEntrance_Movement_BlockPlayerEntryTop::
	walk_up
	step_end

Route5_SouthEntrance_Movement_BlockPlayerEntryBottom::
	walk_down
	step_end
