Aide_EventScript_HaventCaughtEnough::
	call_if_eq VAR_TEMP_3, 0, Aide_EventScript_HaventCaughtEnoughItem
	call_if_eq VAR_TEMP_3, 1, Aide_EventScript_HaventCaughtEnoughPlayerItem
	call_if_eq VAR_TEMP_3, 2, Aide_EventScript_HaventCaughtEnoughItem
	setflagvar VAR_0x8003
	release
	end

Aide_EventScript_HaventCaughtEnoughItem::
	msgbox Aide_Text_HaventCaughtEnoughMonsForItem
	return

Aide_EventScript_HaventCaughtEnoughPlayerItem::
	msgbox Aide_Text_HaventCaughtEnoughMonsForPlayerItem
	return

Aide_EventScript_DeclineCheckMons::
	call_if_eq VAR_TEMP_3, 0, Aide_EventScript_DeclineCheckMonsItem
	call_if_eq VAR_TEMP_3, 1, Aide_EventScript_DeclineCheckMonsPlayerItem
	call_if_eq VAR_TEMP_3, 2, Aide_EventScript_DeclineCheckMonsItem
	setflagvar VAR_0x8003
	release
	end

Aide_EventScript_DeclineCheckMonsItem::
	msgbox Aide_Text_GetEnoughMonsComeBackForItem
	return

Aide_EventScript_DeclineCheckMonsPlayerItem::
	msgbox Aide_Text_GetEnoughMonsComeBackForPlayerItem
	return

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
