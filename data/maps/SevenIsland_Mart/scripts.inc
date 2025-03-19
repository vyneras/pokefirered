SevenIsland_Mart_MapScripts::
	.byte 0

SevenIsland_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	specialvar VAR_RESULT, ArchipelagoSpecial_UseBetterShops
	goto_if_eq VAR_RESULT, TRUE, Poke_Mart_EventScript_BetterItems
	pokemart SHOP_SEVEN_ISLAND
	msgbox Text_PleaseComeAgain
	release
	end

SevenIsland_Mart_EventScript_Lass::
	msgbox SevenIsland_Mart_Text_MonHavePersonalitiesOfTheirOwn, MSGBOX_NPC
	end

SevenIsland_Mart_EventScript_Hiker::
	msgbox SevenIsland_Mart_Text_PreparationsCompleteForRuins, MSGBOX_NPC
	end

SevenIsland_Mart_EventScript_Fisher::
	msgbox SevenIsland_Mart_Text_NeedToFishOnSevenIsland, MSGBOX_NPC
	end
