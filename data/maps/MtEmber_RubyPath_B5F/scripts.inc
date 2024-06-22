.set LOCALID_RUBY, 1

MtEmber_RubyPath_B5F_MapScripts::
	.byte 0

MtEmber_RubyPath_B5F_EventScript_BrailleMessage::
	lockall
	setvar VAR_0x8005, 130
	braillemessage_wait Braille_Text_Everything
	braillemessage_wait Braille_Text_HasMeaning1
	braillemessage_wait Braille_Text_Existence
	braillemessage_wait Braille_Text_HasMeaning2
	braillemessage_wait Braille_Text_BeingAlive
	braillemessage_wait Braille_Text_HasMeaning3
	braillemessage_wait Braille_Text_HaveDreams
	braillemessage Braille_Text_UsePower
	waitbuttonpress
	releaseall
	end

MtEmber_RubyPath_B5F_EventScript_Ruby::
	lock
	faceplayer
	removeobject LOCALID_RUBY
Archipelago_Target_NPC_Gift_FLAG_GOT_RUBY::
	giveitem ITEM_RUBY, 1, FLAG_GOT_RUBY
	release
	end
