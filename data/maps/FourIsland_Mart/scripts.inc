FourIsland_Mart_MapScripts::
	.byte 0

FourIsland_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	specialvar VAR_RESULT, ArchipelagoSpecial_UseBetterShops
	goto_if_eq VAR_RESULT, TRUE, Poke_Mart_EventScript_BetterItems
	pokemart SHOP_FOUR_ISLAND
	msgbox Text_PleaseComeAgain
	release
	end

FourIsland_Mart_EventScript_Camper::
	msgbox FourIsland_Mart_Text_IcefallCaveIsFrigid, MSGBOX_NPC
	end
