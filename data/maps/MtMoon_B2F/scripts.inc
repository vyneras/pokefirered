MtMoon_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMoon_B2F_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, MtMoon_B2F_OnFrame
	.byte 0

MtMoon_B2F_OnTransition::
	call_if_unset FLAG_GOT_FOSSIL_FROM_MT_MOON, MtMoon_B2F_EventScript_ShowFossils
	call_if_lt VAR_MAP_SCENE_MT_MOON_B2F, 3, MtMoon_B2F_EventScript_SetMiguelPosition
	call_if_lt VAR_MAP_SCENE_MT_MOON_B2F, 2, MtMoon_B2F_EventScript_SetMiguelTrigger
	end

MtMoon_B2F_EventScript_ShowFossils::
	clearflag FLAG_HIDE_DOME_FOSSIL
	clearflag FLAG_HIDE_HELIX_FOSSIL
	return

MtMoon_B2F_EventScript_SetMiguelPosition::
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_le VAR_0x8004, 13, MtMoon_B2F_EventScript_SetMiguelPositionLeft
	setobjectxyperm LOCALID_MIGUEL, 13, 11
	setvar VAR_TEMP_1, 0

	return

MtMoon_B2F_EventScript_SetMiguelPositionLeft::
	setobjectxyperm LOCALID_MIGUEL, 10, 3
	setobjectmovementtype LOCALID_MIGUEL, MOVEMENT_TYPE_FACE_DOWN
	setvar VAR_TEMP_1, 1
	return

MtMoon_B2F_EventScript_SetMiguelTrigger::
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_le VAR_0x8004, 13, MtMoon_B2F_EventScript_SetMiguelTriggerLeft
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 0
	return

MtMoon_B2F_EventScript_SetMiguelTriggerLeft::
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 1
	return

MtMoon_B2F_OnFrame::
	map_script_2 VAR_MT_MOON_DARKNESS, 0, EventScript_NeedFlash
	.2byte 0

MtMoon_B2F_EventScript_MiguelRightTrigger::
	lockall
	applymovement LOCALID_MIGUEL, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	call MtMoon_B2F_EventScript_BattleMiguel
	releaseall
	end

MtMoon_B2F_EventScript_MiguelLeftTriggerTop::
	lockall
	applymovement LOCALID_MIGUEL, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	call MtMoon_B2F_EventScript_BattleMiguel
	releaseall
	end

MtMoon_B2F_EventScript_MiguelLeftTriggerMid::
	lockall
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_WalkDown
	waitmovement 0
	call MtMoon_B2F_EventScript_BattleMiguel
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_WalkUp
	waitmovement 0
	releaseall
	end

MtMoon_B2F_EventScript_MiguelLeftTriggerBottom::
	lockall
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_WalkDownTwice
	waitmovement 0
	call MtMoon_B2F_EventScript_BattleMiguel
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_WalkUpTwice
	waitmovement 0
	releaseall
	end

MtMoon_B2F_Movement_WalkDown::
	walk_down
	step_end

MtMoon_B2F_Movement_WalkUp::
	walk_up
	face_down
	step_end

MtMoon_B2F_Movement_WalkDownTwice::
	walk_down
	walk_down
	step_end

MtMoon_B2F_Movement_WalkUpTwice::
	walk_up
	walk_up
	face_down
	step_end

MtMoon_B2F_EventScript_Miguel::
	lock
	faceplayer
	goto_if_set FLAG_GOT_FOSSIL_FROM_MT_MOON, MtMoon_B2F_EventScript_MiguelFossilPicked
	goto_if_defeated TRAINER_SUPER_NERD_MIGUEL, MtMoon_B2F_EventScript_MiguelGoPickFossil
	call MtMoon_B2F_EventScript_BattleMiguel
	release
	end

MtMoon_B2F_EventScript_BattleMiguel::
	textcolor NPC_TEXT_COLOR_MALE
	message MtMoon_B2F_Text_MiguelIntro
	waitmessage
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setvar VAR_LAST_TALKED, LOCALID_MIGUEL
	trainerbattle_no_intro TRAINER_SUPER_NERD_MIGUEL, MtMoon_B2F_Text_MiguelDefeat
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 2
	msgbox MtMoon_B2F_Text_WellEachTakeAFossil
	return

MtMoon_B2F_EventScript_MiguelGoPickFossil::
	msgbox MtMoon_B2F_Text_WellEachTakeAFossil
	release
	end

MtMoon_B2F_EventScript_MiguelFossilPicked::
	msgbox MtMoon_B2F_Text_LabOnCinnabarRegeneratesFossils
	release
	end

MtMoon_B2F_EventScript_DomeFossil::
	lock
	faceplayer
	setvar VAR_TEMP_2, 0
	removeobject LOCALID_DOME_FOSSIL
	call MtMoon_B2F_EventScript_Fossil
	removeobject LOCALID_HELIX_FOSSIL
	release
	end

MtMoon_B2F_EventScript_HelixFossil::
	lock
	faceplayer
	setvar VAR_TEMP_2, 1
	removeobject LOCALID_HELIX_FOSSIL
	call MtMoon_B2F_EventScript_Fossil
	removeobject LOCALID_DOME_FOSSIL
	release
	end

MtMoon_B2F_EventScript_Fossil::
Archipelago_Target_NPC_Gift_FLAG_GOT_FOSSIL_FROM_MT_MOON::
	giveitem ITEM_DOME_FOSSIL, 1, FLAG_GOT_FOSSIL_FROM_MT_MOON
	delay 10
	call_if_eq VAR_TEMP_1, 0, MtMoon_B2F_EventScript_MiguelToFossilRight
	call_if_eq VAR_TEMP_1, 1, MtMoon_B2F_EventScript_MiguelToFossilLeft
	copyobjectxytoperm LOCALID_MIGUEL
	textcolor NPC_TEXT_COLOR_MALE
	playfanfare MUS_OBTAIN_KEY_ITEM
	message MtMoon_B2F_Text_ThenThisFossilIsMine
	waitmessage
	waitfanfare
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 3
	clearflag FLAG_HIDE_CINNABAR_FOSSIL
	return

MtMoon_B2F_EventScript_MiguelToFossilRight::
	goto_if_eq VAR_TEMP_2, 1, MtMoon_B2F_EventScript_MiguelToDomeFossilRight
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelToHelixFossilRight
	waitmovement 0
	return

MtMoon_B2F_EventScript_MiguelToDomeFossilRight::
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelToDomeFossilRight
	waitmovement 0
	return

MtMoon_B2F_EventScript_MiguelToFossilLeft::
	goto_if_eq VAR_TEMP_2, 1, MtMoon_B2F_EventScript_MiguelToDomeFossilLeft
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelToHelixFossilLeft
	waitmovement 0
	return

MtMoon_B2F_EventScript_MiguelToDomeFossilLeft::
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelToDomeFossilLeft
	waitmovement 0
	return

MtMoon_B2F_Movement_MiguelToHelixFossilRight::
	walk_right
	walk_up
	walk_up
	walk_up
	step_end

MtMoon_B2F_Movement_MiguelToDomeFossilRight::
	walk_up
	walk_up
	walk_up
	step_end

MtMoon_B2F_Movement_MiguelToHelixFossilLeft::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	step_end

MtMoon_B2F_Movement_MiguelToDomeFossilLeft::
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	step_end

MtMoon_B2F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT, MtMoon_B2F_Text_Grunt1Intro, MtMoon_B2F_Text_Grunt1Defeat
	msgbox MtMoon_B2F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_B2F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_2, MtMoon_B2F_Text_Grunt2Intro, MtMoon_B2F_Text_Grunt2Defeat
	msgbox MtMoon_B2F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_B2F_EventScript_Grunt3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_3, MtMoon_B2F_Text_Grunt3Intro, MtMoon_B2F_Text_Grunt3Defeat
	msgbox MtMoon_B2F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_B2F_EventScript_Grunt4::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_4, MtMoon_B2F_Text_Grunt4Intro, MtMoon_B2F_Text_Grunt4Defeat
	msgbox MtMoon_B2F_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end
