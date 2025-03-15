Route25_MapScripts::
	.byte 0

Route25_EventScript_SeaCottageSign::
	msgbox Route25_Text_SeaCottageSign, MSGBOX_SIGN
	end

Route25_EventScript_Man::
	lock
	faceplayer
	msgbox Route25_Text_AreYouHereAlone
	specialvar VAR_RESULT, ArchipelagoSpecial_GymKeys
	call_if_eq VAR_RESULT, TRUE, Route25_EventScript_ManGiveCeruleanKey
	release
	end

Route25_EventScript_ManGiveCeruleanKey::
	goto_if_set FLAG_GOT_CERULEAN_KEY, EventScript_Return
	msgbox Route25_Text_MistyDroppedThis
Archipelago_Target_NPC_Gift_FLAG_GOT_CERULEAN_KEY::
	giveitem ITEM_CERULEAN_KEY, 1, FLAG_GOT_CERULEAN_KEY
	return
