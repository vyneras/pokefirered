.equ GIVE_FLY_UNLOCK, VAR_TEMP_1

TwoIsland_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, TwoIsland_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, TwoIsland_OnFrame
	.byte 0

TwoIsland_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, TwoIsland_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, TwoIsland_EventScript_SetWorldMapFlag
	end

TwoIsland_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_TWO_ISLAND EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

TwoIsland_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_TWO_ISLAND
	return

TwoIsland_OnFrame::
	map_script_2 GIVE_FLY_UNLOCK, 1, TwoIsland_EventScript_GiveFlyUnlockItem
	.2byte 0

TwoIsland_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_TWO_ISLAND::
	giveitem ITEM_FLY_TWO_ISLAND, 1, FLAG_VISITED_TWO_ISLAND
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

TwoIsland_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart SHOP_TWO_ISLAND
	msgbox Text_PleaseComeAgain
	release
	end

TwoIsland_EventScript_Sailor::
	msgbox TwoIsland_Text_FellowMovedFromCeladonCity, MSGBOX_NPC
	end

TwoIsland_EventScript_Woman::
	msgbox TwoIsland_Text_ShopkeepersBrotherWorksGameCorner, MSGBOX_NPC
	end

TwoIsland_EventScript_Beauty::
	lock
	faceplayer
	famechecker FAMECHECKER_BRUNO, 4
	msgbox TwoIsland_Text_BrunoCameToIslandWhileBack
	release
	end

TwoIsland_EventScript_SuperNerd::
	msgbox TwoIsland_Text_BuyRareItemsHere, MSGBOX_NPC
	end

TwoIsland_EventScript_Boy::
	msgbox TwoIsland_Text_OldWomanLivesOutOnCape, MSGBOX_NPC
	end

TwoIsland_EventScript_LittleBoy::
	msgbox TwoIsland_Text_HaveYouTriedJumpingGame, MSGBOX_NPC
	end

TwoIsland_EventScript_IslandSign::
	msgbox TwoIsland_Text_IslandSign, MSGBOX_SIGN
	end

TwoIsland_EventScript_JoyfulGameCornerSign::
	msgbox TwoIsland_Text_JoyfulGameCornerSign, MSGBOX_SIGN
	end

TwoIsland_EventScript_FastCurrentSign::
	msgbox TwoIsland_Text_DangerFastCurrent, MSGBOX_SIGN
	end
