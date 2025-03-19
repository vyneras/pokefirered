VermilionCity_Mart_MapScripts::
	.byte 0

VermilionCity_Mart_EventScript_CooltrainerF::
	msgbox VermilionCity_Mart_Text_MonsGoodOrBadDependingOnTrainer, MSGBOX_NPC
	end

VermilionCity_Mart_EventScript_BaldingMan::
	msgbox VermilionCity_Mart_Text_TeamRocketAreWickedPeople, MSGBOX_NPC
	end

VermilionCity_Mart_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	specialvar VAR_RESULT, ArchipelagoSpecial_UseBetterShops
	goto_if_eq VAR_RESULT, TRUE, Poke_Mart_EventScript_BetterItems
	pokemart SHOP_VERMILION_CITY
	msgbox Text_PleaseComeAgain
	release
	end
