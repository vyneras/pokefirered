EventScript_ItemfinderDigUpUnderfootItem::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	waitse
	setflagvar VAR_0x8003
	specialvar VAR_RESULT, ArchipelagoSpecial_ShouldHandle
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_FindHiddenItem
	call EventScript_FindHiddenItem
	additem VAR_0x8000, VAR_0x8001
	releaseall
	return
