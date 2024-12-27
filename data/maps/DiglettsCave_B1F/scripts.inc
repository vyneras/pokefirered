DiglettsCave_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, DiglettsCave_B1F_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, DiglettsCave_B1F_OnFrame
	.byte 0

DiglettsCave_B1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_DIGLETTS_CAVE_B1F
	end

DiglettsCave_B1F_OnFrame::
	map_script_2 VAR_DIGLETTS_CAVE_DARKNESS, 0, EventScript_NeedFlash
	.2byte 0
