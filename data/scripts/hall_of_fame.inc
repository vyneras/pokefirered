EventScript_SetDefeatedEliteFourFlagsVars::
	clearflag FLAG_HIDE_POSTGAME_GOSSIPERS
	call EventScript_ResetEliteFour
	special Script_UpdateTrainerFanClubGameClear
	call EventScript_ResetLegendaries
	return

EventScript_ResetLegendaries::
	clearflag FLAG_LUGIA_FLEW_AWAY
	clearflag FLAG_HO_OH_FLEW_AWAY
	clearflag FLAG_DEOXYS_FLEW_AWAY
	return

EventScript_ResetEliteFourEnd::
	call EventScript_ResetEliteFour
	end

EventScript_ResetEliteFour::
	clearflag FLAG_DEFEATED_LORELEI
	clearflag FLAG_DEFEATED_BRUNO
	clearflag FLAG_DEFEATED_AGATHA
	clearflag FLAG_DEFEATED_LANCE
	clearflag FLAG_DEFEATED_ELITE_FOUR
	setvar VAR_MAP_SCENE_POKEMON_LEAGUE, 0
	return
