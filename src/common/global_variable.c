#include "master.h"

int player_x, player_y;
Enemy enemies[MAX_ENEMIES];
Bullet bullets[MAX_BULLETS];
int enemy_count = 0;
int spawn_rate = 3;
int game_time = 0;
int game_over = 0;
int kill_count = 0;

int skill_a_timer = 0;
int skill_b_timer = 0;

int upgrade_a_8dir = 0;
int skill_b_range = SKILL_B_RANGE;

// 타이머 및 상태 관련 변수 정의
int spawn_timer = 0;
int move_timer = 0;
int choice_shown = 0;
int choice_selected = 0;
GameState state = STATE_PLAY;
