#pragma once

// 로직 함수(원래 함수명/시그니처 유지)
void init_enemies(void);
void init_bullets(void);
void spawn_enemy(void);
void move_enemies(void);
void skill_a_fire(void);
void move_bullets(void);
void check_bullet_collision(void);
void skill_b_aoe(void);
int check_player_collision(void);
void update_game_logic(int *prev_left, int *prev_right, int *prev_a);
void update_choice_logic(int *prev_left, int *prev_right, int *prev_a);
