CeruleanCity_Mart_MapScripts::
	.byte 0

CeruleanCity_Mart_EventScript_Youngster::
	msgbox CeruleanCity_Mart_Text_RepelWorksOnWeakMons, MSGBOX_NPC
	end

CeruleanCity_Mart_EventScript_Woman::
	msgbox CeruleanCity_Mart_Text_DoYouKnowAboutRareCandy, MSGBOX_NPC
	end

CeruleanCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	specialvar VAR_RESULT, ArchipelagoSpecial_UseBetterShops
	goto_if_eq VAR_RESULT, TRUE, Poke_Mart_EventScript_BetterItems
	pokemart SHOP_CERULEAN_CITY
	msgbox Text_PleaseComeAgain
	release
	end
