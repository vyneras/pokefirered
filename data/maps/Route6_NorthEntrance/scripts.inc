Route6_NorthEntrance_MapScripts::
	.byte 0

Route6_NorthEntrance_EventScript_Guard::
	msgbox Route6_NorthEntrance_Text_HiHowsItGoing, MSGBOX_NPC
	end

Route6_NorthEntrance_EventScript_GuardTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route6_NorthEntrance_EventScript_GuardTrigger
	end

Route6_NorthEntrance_EventScript_GuardTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	goto Route6_NorthEntrance_EventScript_GuardTrigger
	end

Route6_NorthEntrance_EventScript_GuardTriggerRight::
	lockall
	setvar VAR_TEMP_1, 2
	goto Route6_NorthEntrance_EventScript_GuardTrigger
	end

Route6_NorthEntrance_EventScript_GuardTrigger::
	textcolor NPC_TEXT_COLOR_MALE
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	checkitem ITEM_TEA
	goto_if_eq VAR_RESULT, TRUE, Route6_NorthEntrance_EventScript_GiveTea
	checkitem ITEM_RED_TEA
	goto_if_eq VAR_RESULT, TRUE, Route6_NorthEntrance_EventScript_GiveRedTea
	msgbox Route6_NorthEntrance_Text_ThirstyOnGuardDuty
	closemessage
	goto_if_eq VAR_FACING, DIR_SOUTH, Route6_NorthEntrance_EventScript_MovePlayerUp
	applymovement OBJ_EVENT_ID_PLAYER, Route6_NorthEntrance_Movement_BlockPlayerEntryBottom
	waitmovement 0
	releaseall
	end

Route6_NorthEntrance_EventScript_GiveTea::
	removeitem ITEM_TEA
	call Route6_NorthEntrance_EventScript_GuardDrinkTea
	msgbox Route6_NorthEntrance_Text_ThanksIllShareTeaWithGuards
	setvar VAR_MAP_SCENE_ROUTE5_GATE, 1
	setvar VAR_MAP_SCENE_ROUTE6_GATE, 1
	setvar VAR_MAP_SCENE_ROUTE7_GATE, 1
	setvar VAR_MAP_SCENE_ROUTE8_GATE, 1
	releaseall
	end

Route6_NorthEntrance_EventScript_GuardDrinkTea::
	msgbox Route6_NorthEntrance_Text_ThatTeaLooksTasty
	closemessage
	call_if_eq VAR_TEMP_1, 0, Route6_NorthEntrance_EventScript_WalkToGuardLeft
	call_if_eq VAR_TEMP_1, 1, Route6_NorthEntrance_EventScript_WalkToGuardMid
	call_if_eq VAR_TEMP_1, 2, Route6_NorthEntrance_EventScript_WalkToGuardRight
	return

Route6_NorthEntrance_EventScript_GiveRedTea::
	removeitem ITEM_RED_TEA
	call Route6_NorthEntrance_EventScript_GuardDrinkTea
	msgbox Route6_NorthEntrance_Text_ThanksForTea
	setvar VAR_MAP_SCENE_ROUTE6_GATE, 1
	releaseall
	end

Route6_NorthEntrance_EventScript_MovePlayerUp::
	applymovement OBJ_EVENT_ID_PLAYER, Route6_NorthEntrance_Movement_BlockPlayerEntryTop
	waitmovement 0
	releaseall
	end

Route6_NorthEntrance_EventScript_WalkToGuardLeft::
	applymovement OBJ_EVENT_ID_PLAYER, Route6_NorthEntrance_Movement_WalkToGuardLeft
	waitmovement 0
	return

Route6_NorthEntrance_EventScript_WalkToGuardMid::
	applymovement OBJ_EVENT_ID_PLAYER, Route6_NorthEntrance_Movement_WalkToGuardMid
	waitmovement 0
	return

Route6_NorthEntrance_EventScript_WalkToGuardRight::
	applymovement OBJ_EVENT_ID_PLAYER, Route6_NorthEntrance_Movement_WalkToGuardRight
	waitmovement 0
	return

Route6_NorthEntrance_Movement_WalkToGuardLeft::
	walk_right
	walk_right
	step_end

Route6_NorthEntrance_Movement_WalkToGuardMid::
	walk_right
	step_end

Route6_NorthEntrance_Movement_WalkToGuardRight::
	walk_in_place_right
	step_end

Route6_NorthEntrance_Movement_BlockPlayerEntryBottom::
	walk_down
	step_end

Route6_NorthEntrance_Movement_BlockPlayerEntryTop::
	walk_up
	step_end
