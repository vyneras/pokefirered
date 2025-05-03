Aide_EventScript_HaventCaughtEnough::
	setflagvar VAR_0x8003
	goto_if_eq VAR_TEMP_3, 0, Aide_EventScript_HaventCaughtEnoughItem
	goto_if_eq VAR_TEMP_3, 1, Aide_EventScript_HaventCaughtEnoughPlayerItem
	goto_if_eq VAR_TEMP_3, 2, Aide_EventScript_HaventCaughtEnoughItem
	end

Aide_EventScript_HaventCaughtEnoughItem::
	msgbox Aide_Text_HaventCaughtEnoughMonsForItem
	release
	end

Aide_EventScript_HaventCaughtEnoughPlayerItem::
	msgbox Aide_Text_HaventCaughtEnoughMonsForPlayerItem
	release
	end

Aide_EventScript_DeclineCheckMons::
	setflagvar VAR_0x8003
	goto_if_eq VAR_TEMP_3, 0, Aide_EventScript_DeclineCheckMonsItem
	goto_if_eq VAR_TEMP_3, 1, Aide_EventScript_DeclineCheckMonsPlayerItem
	goto_if_eq VAR_TEMP_3, 2, Aide_EventScript_DeclineCheckMonsItem
	end

Aide_EventScript_DeclineCheckMonsItem::
	msgbox Aide_Text_GetEnoughMonsComeBackForItem
	release
	end

Aide_EventScript_DeclineCheckMonsPlayerItem::
	msgbox Aide_Text_GetEnoughMonsComeBackForPlayerItem
	release
	end

Aide_EventScript_GiveIfSeenXMons::
	call_if_eq VAR_TEMP_3, 0, Aide_EventScript_GiveItemIfSeenXMons
	call_if_eq VAR_TEMP_3, 1, Aide_EventScript_GivePlayerItemIfSeenXMons
	call_if_eq VAR_TEMP_3, 2, Aide_EventScript_GiveItemIfSeenXMons
	return

Aide_EventScript_GiveItemIfSeenXMons::
	msgbox Aide_Text_GiveItemIfCaughtEnough, MSGBOX_YESNO
	return

Aide_EventScript_GivePlayerItemIfSeenXMons::
	msgbox Aide_Text_GivePlayerItemIfCaughtEnough, MSGBOX_YESNO
	return
