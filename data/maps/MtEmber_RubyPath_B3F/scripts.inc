.set LOCALID_RUBY, 1

MtEmber_RubyPath_B3F_MapScripts::
	.byte 0

@ Odd that this is here and not in MtEmber_RubyPath_B5F/scripts.inc
MtEmber_RubyPath_B5F_EventScript_Ruby::
	lock
	faceplayer
	removeobject LOCALID_RUBY
Archipelago_Target_NPC_Gift_FLAG_GOT_RUBY::
	giveitem ITEM_RUBY, 1, FLAG_GOT_RUBY
	release
	end
