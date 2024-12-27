.equ LOCALID_DOME_FOSSIL,  1
.equ LOCALID_HELIX_FOSSIL, 2
.equ LOCALID_MIGUEL,       3

MtMoon_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMoon_B2F_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, MtMoon_B2F_OnFrame
	.byte 0

MtMoon_B2F_OnTransition::
	call_if_unset FLAG_GOT_DOME_FOSSIL, MtMoon_B2F_EventScript_ShowDomeFossil
	call_if_unset FLAG_GOT_HELIX_FOSSIL, MtMoon_B2F_EventScript_ShowHelixFossil
	goto_if_ne VAR_MAP_SCENE_MT_MOON_B2F, 2, MtMoon_B2F_EventScript_SetMiguelPosition
	end

MtMoon_B2F_EventScript_ShowDomeFossil::
	clearflag FLAG_HIDE_DOME_FOSSIL
	return

MtMoon_B2F_EventScript_ShowHelixFossil::
	clearflag FLAG_HIDE_HELIX_FOSSIL
	return

MtMoon_B2F_EventScript_SetMiguelPosition::
	getplayerxy VAR_0x8004, VAR_0x8005
	goto_if_lt VAR_0x8004, 7, MtMoon_B2F_EventScript_SetMiguelLeft
	setobjectxyperm LOCALID_MIGUEL, 13, 11
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 0
	end

MtMoon_B2F_EventScript_SetMiguelLeft::
	setobjectxyperm LOCALID_MIGUEL, 3, 6
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 1
	end

MtMoon_B2F_OnFrame::
	map_script_2 VAR_MT_MOON_DARKNESS, 0, EventScript_NeedFlash
	.2byte 0

MtMoon_B2F_EventScript_MiguelTrigger::
	lockall
	applymovement LOCALID_MIGUEL, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	call MtMoon_B2F_EventScript_BattleMiguel
	releaseall
	end

MtMoon_B2F_EventScript_Miguel::
	lock
	faceplayer
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
	goto_if_set FLAG_GOT_FOSSIL_FROM_MT_MOON, MtMoon_B2F_EventScript_MiguelFossilPicked
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
	removeobject LOCALID_DOME_FOSSIL
Archipelago_Target_NPC_Gift_FLAG_GOT_DOME_FOSSIL::
	giveitem ITEM_DOME_FOSSIL, 1, FLAG_GOT_DOME_FOSSIL
	setflag FLAG_GOT_FOSSIL_FROM_MT_MOON
	release
	end

MtMoon_B2F_EventScript_HelixFossil::
	lock
	faceplayer
	removeobject LOCALID_HELIX_FOSSIL
Archipelago_Target_NPC_Gift_FLAG_GOT_HELIX_FOSSIL::
	giveitem ITEM_HELIX_FOSSIL, 1, FLAG_GOT_HELIX_FOSSIL
	setflag FLAG_GOT_FOSSIL_FROM_MT_MOON
	release
	end

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
