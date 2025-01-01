.equ LOCALID_SCANNER, 5

SevenIsland_TanobyRuins_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SevenIsland_TanobyRuins_OnTransition
	.byte 0

SevenIsland_TanobyRuins_OnTransition::
	call_if_set FLAG_SYS_UNLOCKED_TANOBY_RUINS, SevenIsland_TanobyRuins_SetUnlockedLayout
	end

SevenIsland_TanobyRuins_SetUnlockedLayout::
	setmaplayoutindex LAYOUT_SEVEN_ISLAND_TANOBY_RUINS_UNLOCKED
	return

SevenIsland_TanobyRuins_EventScript_Scanner::
	lock
	faceplayer
	removeobject LOCALID_SCANNER
Archipelago_Target_NPC_Gift_FLAG_GOT_SCANNER::
	giveitem ITEM_SCANNER, 1, FLAG_GOT_SCANNER
	release
	end
