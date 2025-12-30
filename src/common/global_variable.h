#pragma once

#include "game_set.h"
#include "skill_set.h"
#include "character_struct.h"

// 전역 변수 외부 선언
extern int player_x;
extern int player_y;
extern Enemy enemies[MAX_ENEMIES];
extern Bullet bullets[MAX_BULLETS];
extern int enemy_count;
extern int spawn_rate;
extern int game_time;
extern int game_over;
extern int kill_count;
extern int skill_a_timer;
extern int skill_b_timer;
extern int upgrade_a_8dir;
extern int skill_b_range;

// 타이머 및 상태 관련 변수 추가
extern int spawn_timer;
extern int move_timer;
extern int choice_shown;
extern int choice_selected;
extern GameState state;
