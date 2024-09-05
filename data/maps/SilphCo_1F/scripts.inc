SilphCo_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SilphCo_1F_OnTransition
	.byte 0

SilphCo_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SILPH_CO_1F
	end

SilphCo_1F_EventScript_Receptionist::
	lock
	specialvar VAR_RESULT, ArchipelagoSpecial_AreCardKeysSplit
	goto_if_eq VAR_RESULT, FALSE, SilphCo_1F_EventScript_ReceptionistGaveCardKey
	goto_if_set FLAG_GOT_2F_CARD_KEY, SilphCo_1F_EventScript_ReceptionistGaveCardKey
	msgbox SilphCo_1F_Text_TakeThisCardKey
Archipelago_Target_NPC_Gift_FLAG_GOT_2F_CARD_KEY::
	giveitem ITEM_CARD_KEY_2F, 1, FLAG_GOT_2F_CARD_KEY
	release
	end

SilphCo_1F_EventScript_ReceptionistGaveCardKey::
	msgbox SilphCo_1F_Text_WelcomePresidentInBoardroom
	release
	end

SilphCo_1F_EventScript_FloorSign::
	msgbox SilphCo_1F_Text_FloorSign, MSGBOX_SIGN
	end
