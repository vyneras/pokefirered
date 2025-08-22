.set LOCALID_EARTH_BADGE_GUARD,   7
.set REQUIRED_BADGES_GYMS, VAR_TEMP_1

Route23_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route23_OnTransition
	.byte 0

Route23_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_Route23Modified
	call_if_eq VAR_RESULT, TRUE, Route23_EventScript_SetAltLayout
	specialvar VAR_RESULT, ArchipelagoSpecial_Route23Trees
	call_if_eq VAR_RESULT, FALSE, Route23_EventScript_RemoveTrees
	end

Route23_EventScript_SetAltLayout::
	setmaplayoutindex LAYOUT_ROUTE23_ALT
	return

Route23_EventScript_RemoveTrees::
	setflag FLAG_TEMP_12
	setflag FLAG_TEMP_13
	setflag FLAG_TEMP_14
	return

Route23_EventScript_LastGuard::
	lock
	faceplayer
	setvar VAR_TEMP_2, 0
	goto Route23_EventScript_LastGuardCheckBadgesGyms
	end

Route23_EventScript_LastGuardTrigger::
	lockall
	applymovement LOCALID_EARTH_BADGE_GUARD, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	setvar VAR_TEMP_2, 1
	goto Route23_EventScript_LastGuardCheckBadgesGyms
	end

Route23_EventScript_LastGuardCheckBadgesGyms::
	textcolor NPC_TEXT_COLOR_MALE
	specialvar VAR_RESULT, ArchipelagoSpecial_CanPassRoute23Guard
	goto_if_eq VAR_RESULT, FALSE, Route23_EventScript_LastGuardCantPass
	setvar VAR_MAP_SCENE_ROUTE23, 1
	specialvar VAR_RESULT, ArchipelagoSpecial_Route23RequiresGyms
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_LastGuardEnoughGyms
	msgbox Text_OhThatsEnoughBadgesGoRightAhead
	release
	end

Route23_EventScript_LastGuardEnoughGyms::
	msgbox Text_OhThatsEnoughGymsGoRightAhead
	release
	end

Route23_EventScript_LastGuardCantPass::
	specialvar REQUIRED_BADGES_GYMS, ArchipelagoSpecial_GetRoute23RequiredCount
	buffernumberstring STR_VAR_1, REQUIRED_BADGES_GYMS
	specialvar VAR_RESULT, ArchipelagoSpecial_Route23RequiresGyms
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_LastGuardCantPassGyms
	goto_if_eq REQUIRED_BADGES_GYMS, 1, Route23_EventScript_LastGuardCantPassNoBadge
	msgbox Text_OnlyPassWithXBadges
	goto_if_eq VAR_TEMP_2, 1, Route23_EventScript_ForcePlayerDown
	release
	end

Route23_EventScript_LastGuardCantPassNoBadge::
	msgbox Text_OnlyPassWithBadge
	goto_if_eq VAR_TEMP_2, 1, Route23_EventScript_ForcePlayerDown
	release
	end

Route23_EventScript_LastGuardCantPassGyms::
	goto_if_eq REQUIRED_BADGES_GYMS, 1, Route23_EventScript_LastGuardCantPassNoGym
	msgbox Text_OnlyPassWithXGyms
	goto_if_eq VAR_TEMP_2, 1, Route23_EventScript_ForcePlayerDown
	release
	end

Route23_EventScript_LastGuardCantPassNoGym::
	msgbox Text_OnlyPassWithGym
	goto_if_eq VAR_TEMP_2, 1, Route23_EventScript_ForcePlayerDown
	release
	end

Route23_EventScript_ForcePlayerDown::
	closemessage
	goto_if_eq VAR_FACING, DIR_SOUTH, Route23_EventScript_ForcePlayerUp
	applymovement OBJ_EVENT_ID_PLAYER, Route23_Movement_ForcePlayerDown
	waitmovement 0
	release
	end

Route23_EventScript_ForcePlayerUp::
	applymovement OBJ_EVENT_ID_PLAYER, Route23_Movement_ForcePlayerUp
	waitmovement 0
	release
	end

Route23_Movement_ForcePlayerDown::
	walk_down
	step_end

Route23_Movement_ForcePlayerUp::
	walk_up
	step_end

Route23_EventScript_Guard::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Text_GoodLuckWithEliteFour
	release
	end

Route23_EventScript_VictoryRoadGateSign::
	msgbox Route23_Text_VictoryRoadGateSign, MSGBOX_SIGN
	end
