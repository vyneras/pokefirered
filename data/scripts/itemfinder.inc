EventScript_ItemfinderDigUpUnderfootItem::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	waitse
	copyvar VAR_0x8003, VAR_0x8004
	copyvar VAR_0x8014, VAR_0x8005
	specialvar VAR_RESULT, ArchipelagoSpecial_ShouldHandle
	goto_if_eq VAR_RESULT, TRUE, ArchipelagoScript_FindHiddenItem
	call EventScript_TryPickUpHiddenItem
	goto EventScript_PickedUpHiddenItem
	end
