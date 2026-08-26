PokemonTower_6F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonTower_6F_OnTransition
	.byte 0

PokemonTower_6F_OnTransition::
	getplayerxy VAR_0x8004, VAR_0x8005
	call_if_ge VAR_0x8004, 18, PokemonTower_6F_SetNearRightStairs
	call_if_le VAR_0x8004, 11, PokemonTower_6F_SetNearLeftStairs
	setvar VAR_TEMP_1, 0
	end

PokemonTower_6F_SetNearRightStairs::
	setvar VAR_TEMP_2, 0
	return

PokemonTower_6F_SetNearLeftStairs::
	setvar VAR_TEMP_2, 1
	return

PokemonTower_6F_EventScript_MarowakGhost::
	lockall
	textcolor NPC_TEXT_COLOR_MON
	msgbox PokemonTower_6F_Text_BeGoneIntruders
	goto_if_questlog EventScript_ReleaseEnd
Archipelago_Target_Static_Encounter_MAROWAK::
	setwildbattle SPECIES_MAROWAK, 30
	special StartMarowakBattle
	waitstate
	special QuestLog_CutRecording
	goto_if_eq VAR_RESULT, FALSE, PokemonTower_6F_EventScript_DefeatedMarowakGhost  @ VAR_RESULT set by CB2_EndMarowakBattle
	call_if_eq VAR_TEMP_1, 0, PokemonTower_6F_EventScript_ForcePlayerMoveUp6F
	call_if_eq VAR_TEMP_1, 1, PokemonTower_6F_EventScript_ForcePlayerMoveLeft1F
	releaseall
	end

PokemonTower_6F_EventScript_DefeatedMarowakGhost::
	message PokemonTower_6F_Text_GhostWasCubonesMother
	waitmessage
	waitse
	playmoncry SPECIES_MAROWAK, CRY_MODE_NORMAL
	waitbuttonpress
	waitmoncry
	msgbox PokemonTower_6F_Text_MothersSpiritWasCalmed
	setvar VAR_MAP_SCENE_POKEMON_TOWER_6F, 1
	setvar VAR_MAP_SCENE_POKEMON_TOWER_1F, 1
	releaseall
	end

PokemonTower_6F_EventScript_ForcePlayerMoveUp6F::
	goto_if_eq VAR_TEMP_2, 1, PokemonTower_6F_EventScript_ForcePlayerMoveDown6F
	applymovement LOCALID_PLAYER, PokemonTower_6F_Movement_ForcePlayerUp
	waitmovement 0
	return

PokemonTower_6F_EventScript_ForcePlayerMoveDown6F::
	goto_if_eq VAR_FACING, DIR_EAST, PokemonTower_6F_EventScript_ForcePlayerMoveLeft6F
	applymovement LOCALID_PLAYER, PokemonTower_6F_Movement_ForcePlayerDown
	waitmovement 0
	return

PokemonTower_6F_EventScript_ForcePlayerMoveLeft6F::
	applymovement LOCALID_PLAYER, PokemonTower_6F_Movement_ForcePlayerLeft
	waitmovement 0
	return

PokemonTower_6F_EventScript_ForcePlayerMoveLeft1F::
	goto_if_eq VAR_TEMP_2, 1, PokemonTower_6F_EventScript_ForcePlayerMoveRight1F
	applymovement LOCALID_PLAYER, PokemonTower_6F_Movement_ForcePlayerLeft
	waitmovement 0
	return

PokemonTower_6F_EventScript_ForcePlayerMoveRight1F::
	goto_if_eq VAR_FACING, DIR_NORTH, PokemonTower_6F_EventScript_ForcePlayerMoveDown1F
	goto_if_eq VAR_FACING, DIR_SOUTH, PokemonTower_6F_EventScript_ForcePlayerMoveUp1F
	applymovement LOCALID_PLAYER, PokemonTower_6F_Movement_ForcePlayerRight
	waitmovement 0
	return

PokemonTower_6F_EventScript_ForcePlayerMoveDown1F::
	applymovement LOCALID_PLAYER, PokemonTower_6F_Movement_ForcePlayerDown
	waitmovement 0
	return

PokemonTower_6F_EventScript_ForcePlayerMoveUp1F::
	applymovement LOCALID_PLAYER, PokemonTower_6F_Movement_ForcePlayerUp
	waitmovement 0
	return

PokemonTower_6F_Movement_ForcePlayerUp::
	walk_up
	step_end

PokemonTower_6F_Movement_ForcePlayerDown::
	walk_down
	step_end

PokemonTower_6F_Movement_ForcePlayerLeft::
	walk_left
	step_end

PokemonTower_6F_Movement_ForcePlayerRight::
	walk_right
	step_end

PokemonTower_6F_EventScript_Angelica::
	trainerbattle_single TRAINER_CHANNELER_ANGELICA, PokemonTower_6F_Text_AngelicaIntro, PokemonTower_6F_Text_AngelicaDefeat
	msgbox PokemonTower_6F_Text_AngelicaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_6F_EventScript_Emilia::
	trainerbattle_single TRAINER_CHANNELER_EMILIA, PokemonTower_6F_Text_EmiliaIntro, PokemonTower_6F_Text_EmiliaDefeat
	msgbox PokemonTower_6F_Text_EmiliaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_6F_EventScript_Jennifer::
	trainerbattle_single TRAINER_CHANNELER_JENNIFER, PokemonTower_6F_Text_JenniferIntro, PokemonTower_6F_Text_JenniferDefeat
	msgbox PokemonTower_6F_Text_JenniferPostBattle, MSGBOX_AUTOCLOSE
	end
