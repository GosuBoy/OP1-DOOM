/* Offline proposal only: finite sustained-session limits, not wall time. */
#pragma once
#define SESSION_GAME_TICS 126000u
#define SESSION_MAX_FRAMES 504000u
#define INPUT_TOTAL_TICKS 16777216u
#define INPUT_MAX_REQUESTS 1048576u
_Static_assert(INPUT_TOTAL_TICKS < 0x80000000u,"elapsed interval below half wrap");
_Static_assert(INPUT_MAX_REQUESTS < 0xffffffffu,"tags cannot wrap within session");
_Static_assert(SESSION_GAME_TICS < SESSION_MAX_FRAMES,"allow transition frames");

enum { STOP_NONE, STOP_TARGET, STOP_QUIT, STOP_INPUT_BUDGET, STOP_INPUT_FAULT, STOP_FRAME_LIMIT };
