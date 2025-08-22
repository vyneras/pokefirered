PokemonTower_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonTower_1F_OnTransition
	.byte 0

PokemonTower_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_POKEMON_TOWER_1F
	getplayerxy VAR_0x8004, VAR_0x8005
	call_if_le VAR_0x8004, 11, PokemonTower_1F_SetNearEntrance
	call_if_ge VAR_0x8004, 18, PokemonTower_1F_SetNearStairs
	setvar VAR_TEMP_1, 1
	end

PokemonTower_1F_SetNearEntrance::
	setvar VAR_TEMP_2, 0
	return

PokemonTower_1F_SetNearStairs::
	setvar VAR_TEMP_2, 1
	return

PokemonTower_1F_EventScript_Channeler::
	msgbox PokemonTower_1F_Text_SenseSpiritsUpToMischief, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_Woman1::
	msgbox PokemonTower_1F_Text_GrowlitheWhyDidYouDie, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_BaldingMan::
	msgbox PokemonTower_1F_Text_CameToPrayForDepartedClefairy, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_Woman2::
	lock
	faceplayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, PokemonTower_1F_EventScript_Woman2MalePlayer
	msgbox PokemonTower_1F_Text_ComeToPayRespectsGirl
	release
	end

PokemonTower_1F_EventScript_Woman2MalePlayer::
	msgbox PokemonTower_1F_Text_ComeToPayRespectsSon
	release
	end

PokemonTower_1F_EventScript_WorkerF::
	msgbox PokemonTower_1F_Text_ErectedInMemoryOfDeadMons, MSGBOX_NPC
	end
