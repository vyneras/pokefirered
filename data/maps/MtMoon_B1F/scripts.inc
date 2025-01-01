MtMoon_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, MtMoon_B1F_OnFrame
	.byte 0

MtMoon_B1F_OnFrame::
	map_script_2 VAR_MT_MOON_DARKNESS, 0, EventScript_NeedFlash
	.2byte 0
