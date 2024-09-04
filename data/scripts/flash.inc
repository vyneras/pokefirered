EventScript_FldEffFlash::
	animateflash 0
	setflashlevel 0
	end

EventScript_NeedFlash::
	lockall
	specialvar VAR_RESULT, ArchipelagoSpecial_FlashRequired
	goto_if_eq VAR_RESULT, FALSE, EventScript_CanUseFlash
	setvar VAR_0x8003, 0 @ FIELD_MOVE_FLASH
	specialvar VAR_RESULT, ArchipelagoSpecial_CanUseHmOutsideBattle
	goto_if_eq VAR_RESULT, FALSE, EventScript_CantUseFlash
	checkpartymove MOVE_FLASH
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_CantUseFlash
	goto EventScript_CanUseFlash
	end

EventScript_CanUseFlash::
	setvar VAR_MT_MOON_DARKNESS, 1
	setvar VAR_DIGLETTS_CAVE_DARKNESS, 1
	setvar VAR_ROCK_TUNNEL_DARKNESS, 1
	setvar VAR_VICTORY_ROAD_DARKNESS, 1
	releaseall
	end

EventScript_CantUseFlash::
	msgbox Text_NeedFlash
	closemessage
	special ArchipelagoSpecial_UseClosestWarp
	releaseall
	end
