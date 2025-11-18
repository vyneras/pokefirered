.equ LOCALID_SIGN_LADY, 1

.equ SIGN_LADY_READY, VAR_TEMP_2
.equ GIVE_FLY_UNLOCK, VAR_TEMP_3

PalletTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PalletTown_OnFrame
	.byte 0

PalletTown_OnTransition::
	specialvar VAR_RESULT, ArchipelagoSpecial_FlyUnlocks
	call_if_eq VAR_RESULT, TRUE, PalletTown_EventScript_SetGiveFlyUnlockItem
	call_if_eq VAR_RESULT, FALSE, PalletTown_EventScript_SetWorldMapFlag
	setvar VAR_TELEPORT_LOCATION, SPAWN_PALLET_TOWN
	end

PalletTown_EventScript_SetGiveFlyUnlockItem::
	goto_if_set FLAG_VISITED_PALLET_TOWN EventScript_Return
	setvar GIVE_FLY_UNLOCK, 1
	return

PalletTown_EventScript_SetWorldMapFlag::
	setworldmapflag FLAG_WORLD_MAP_PALLET_TOWN
	return

PalletTown_OnFrame::
	map_script_2 GIVE_FLY_UNLOCK, 1, PalletTown_EventScript_GiveFlyUnlockItem
	.2byte 0

PalletTown_EventScript_GiveFlyUnlockItem::
	lockall
Archipelago_Target_Fly_Unlock_FLAG_VISITED_PALLET_TOWN::
	giveitem ITEM_FLY_PALLET, 1, FLAG_VISITED_PALLET_TOWN
	setvar GIVE_FLY_UNLOCK, 0
	releaseall
	end

PalletTown_EventScript_SignLady::
	lock
	applymovement LOCALID_SIGN_LADY, Common_Movement_FacePlayer
	waitmovement 0
	msgbox PalletTown_Text_RaisingMonsToo
	release
	end

PalletTown_EventScript_FatMan::
	msgbox PalletTown_Text_CanStoreItemsAndMonsInPC, MSGBOX_NPC
	end

PalletTown_EventScript_OaksLabSign::
	lockall
	famechecker FAMECHECKER_OAK, 0
	msgbox PalletTown_Text_OakPokemonResearchLab
	releaseall
	end

PalletTown_EventScript_PlayersHouseSign::
	msgbox PalletTown_Text_PlayersHouse, MSGBOX_SIGN
	end

PalletTown_EventScript_RivalsHouseSign::
	msgbox PalletTown_Text_RivalsHouse, MSGBOX_SIGN
	end

PalletTown_EventScript_TownSign::
	msgbox PalletTown_Text_TownSign, MSGBOX_SIGN
	end

PalletTown_EventScript_TrainerTips::
	lockall
	msgbox PalletTown_Text_PressStartToOpenMenu
	setvar VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 1
	releaseall
	end
