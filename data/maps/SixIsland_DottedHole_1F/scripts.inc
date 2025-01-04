.equ LOCALID_FIVE_PASS, 1

SixIsland_DottedHole_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SixIsland_DottedHole_1F_OnTransition
	.byte 0

SixIsland_DottedHole_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SIX_ISLAND_DOTTED_HOLE_1F
	end

SixIsland_DottedHole_B1F_EventScript_FivePass::
	lock
	faceplayer
	removeobject LOCALID_FIVE_PASS
	msgbox SixIsland_DottedHole_1F_Text_MustHaveDroppedThis
Archipelago_Target_NPC_Gift_FLAG_GOT_FIVE_PASS::
	giveitem ITEM_FIVE_PASS, 1, FLAG_GOT_FIVE_PASS
	release
	end

SixIsland_DottedHole_B1F_EventScript_BrailleUp::
	lockall
	braillemessage Braille_Text_Up
	waitbuttonpress
	releaseall
	end

SixIsland_DottedHole_B4F_EventScript_BrailleDown::
	lockall
	braillemessage Braille_Text_Down
	waitbuttonpress
	releaseall
	end

SixIsland_DottedHole_B3F_EventScript_BrailleRight::
	lockall
	braillemessage Braille_Text_Right
	waitbuttonpress
	releaseall
	end

SixIsland_DottedHole_B2F_EventScript_BrailleLeft::
	lockall
	braillemessage Braille_Text_Left
	waitbuttonpress
	releaseall
	end
