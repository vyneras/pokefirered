SaffronCity_Mart_MapScripts::
	.byte 0

SaffronCity_Mart_EventScript_Lass::
	msgbox SaffronCity_Mart_Text_ReviveIsCostly, MSGBOX_NPC
	end

SaffronCity_Mart_EventScript_Youngster::
	msgbox SaffronCity_Mart_Text_MaxRepelMoreEffectiveThanSuper, MSGBOX_NPC
	end

SaffronCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	specialvar VAR_RESULT, ArchipelagoSpecial_UseBetterShops
	goto_if_eq VAR_RESULT, TRUE, Poke_Mart_EventScript_BetterItems
	pokemart SHOP_SAFFRON_CITY
	msgbox Text_PleaseComeAgain
	release
	end
