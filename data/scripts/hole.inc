EventScript_FallDownHole::
	lockall
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, Movement_SetInvisible
	waitmovement 0
	playse SE_FALL
	delay 60
	warphole MAP_UNDEFINED
	waitstate
	end

@ Differs from the script above by assuming the warp destination has already been set
EventScript_DoFallWarp::
	lockall
	delay 20
	applymovement OBJ_EVENT_ID_PLAYER, Movement_SetInvisible
	waitmovement 0
	playse SE_FALL
	delay 60
	special DoFallWarp
	waitstate
	end

Movement_SetInvisible:
	set_invisible
	step_end
