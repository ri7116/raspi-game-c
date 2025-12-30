#include <stdlib.h>
#include "st7789.h"
#include "master.h"

// 적 초기화
void init_enemies() {
    for(int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = 0;
    }
    enemy_count = 0;
}

// 총알 초기화
void init_bullets() {
    for(int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
}

// 랜덤 위치에서 적 스폰
void spawn_enemy() {
    int slot = -1;
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(!enemies[i].active) {
            slot = i;
            break;
        }
    }
    
    if(slot == -1) return;
    
    int side = rand() % 4;
    int x = 0, y = 0;
    
    switch(side) {
        case 0: x = rand() % ST7789_TFTWIDTH; y = 0; break;
        case 1: x = rand() % ST7789_TFTWIDTH; y = ST7789_TFTHEIGHT - ENEMY_SIZE; break;
        case 2: x = 0; y = rand() % ST7789_TFTHEIGHT; break;
        case 3: x = ST7789_TFTWIDTH - ENEMY_SIZE; y = rand() % ST7789_TFTHEIGHT; break;
    }
    
    enemies[slot].x = x;
    enemies[slot].y = y;
    enemies[slot].active = 1;
    enemy_count++;
}

// 적 이동
void move_enemies() {
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(!enemies[i].active) continue;
        
        int dx = player_x - enemies[i].x;
        int dy = player_y - enemies[i].y;
        
        if(dx > 0) enemies[i].x += ENEMY_SPEED;
        else if(dx < 0) enemies[i].x -= ENEMY_SPEED;
        
        if(dy > 0) enemies[i].y += ENEMY_SPEED;
        else if(dy < 0) enemies[i].y -= ENEMY_SPEED;
    }
}

// 스킬 A: 총알 발사
void skill_a_fire() {
    if(skill_a_timer > 0) return;
    
    int directions4[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    int directions8[8][2] = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0},
        {-1, -1}, {1, -1}, {-1, 1}, {1, 1}
    };
    int (*dirs)[2] = upgrade_a_8dir ? directions8 : directions4;
    int dir_count = upgrade_a_8dir ? 8 : 4;

    for(int d = 0; d < dir_count; d++) {
        for(int i = 0; i < MAX_BULLETS; i++) {
            if(!bullets[i].active) {
                bullets[i].x = player_x + PLAYER_SIZE/2 - BULLET_SIZE/2;
                bullets[i].y = player_y + PLAYER_SIZE/2 - BULLET_SIZE/2;
                bullets[i].dx = dirs[d][0];
                bullets[i].dy = dirs[d][1];
                bullets[i].active = 1;
                break;
            }
        }
    }
    
    skill_a_timer = SKILL_A_COOLDOWN;
}

// 총알 이동
void move_bullets() {
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(!bullets[i].active) continue;
        
        bullets[i].x += bullets[i].dx * BULLET_SPEED;
        bullets[i].y += bullets[i].dy * BULLET_SPEED;
        
        if(bullets[i].x < 0 || bullets[i].x >= ST7789_TFTWIDTH ||
           bullets[i].y < 0 || bullets[i].y >= ST7789_TFTHEIGHT) {
            bullets[i].active = 0;
        }
    }
}

// 총알-적 충돌 체크
void check_bullet_collision() {
    for(int b = 0; b < MAX_BULLETS; b++) {
        if(!bullets[b].active) continue;
        
        for(int e = 0; e < MAX_ENEMIES; e++) {
            if(!enemies[e].active) continue;
            
            if(bullets[b].x < enemies[e].x + ENEMY_SIZE &&
               bullets[b].x + BULLET_SIZE > enemies[e].x &&
               bullets[b].y < enemies[e].y + ENEMY_SIZE &&
               bullets[b].y + BULLET_SIZE > enemies[e].y) {
                enemies[e].active = 0;
                bullets[b].active = 0;
                enemy_count--;
                kill_count++;
                break;
            }
        }
    }
}

// 스킬 B: 범위 공격
void skill_b_aoe() {
    if(skill_b_timer > 0) return;
    
    int player_cx = player_x + PLAYER_SIZE / 2;
    int player_cy = player_y + PLAYER_SIZE / 2;
    
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(!enemies[i].active) continue;
        
        int enemy_cx = enemies[i].x + ENEMY_SIZE / 2;
        int enemy_cy = enemies[i].y + ENEMY_SIZE / 2;
        
        int dist = abs(player_cx - enemy_cx) + abs(player_cy - enemy_cy);
        
        if(dist <= skill_b_range) {
            enemies[i].active = 0;
            enemy_count--;
            kill_count++;
        }
    }
    
    skill_b_timer = SKILL_B_COOLDOWN;
}

// 플레이어-적 충돌 체크
int check_player_collision() {
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(!enemies[i].active) continue;
        
        if(player_x < enemies[i].x + ENEMY_SIZE &&
           player_x + PLAYER_SIZE > enemies[i].x &&
           player_y < enemies[i].y + ENEMY_SIZE &&
           player_y + PLAYER_SIZE > enemies[i].y) {
            return 1;
        }
    }
    return 0;
}

// 게임 진행 로직 처리 (타이머 기반)
void update_game_logic(int *prev_left, int *prev_right, int *prev_a) {
    // 1. 스폰 및 시간 타이머 처리 (1초 주기)
    spawn_timer++;
    if(spawn_timer >= 20) {
        spawn_timer = 0;
        game_time++;
        spawn_rate = clamp_int(3 + (game_time / 10), 3, 8);
        for(int i = 0; i < spawn_rate; i++) spawn_enemy();

        // 5초 시점: 업그레이드 선택 화면 진입
        if(!choice_shown && game_time >= 5) {
            choice_shown = 1;
            state = STATE_CHOICE;
            choice_selected = 0;
            if(prev_left) *prev_left = 0;
            if(prev_right) *prev_right = 0;
            if(prev_a) *prev_a = 0;
        }
    }
    
    // 2. 적 이동 타이머 처리 (0.5초 주기)
    move_timer++;
    if(move_timer >= 10) {
        move_timer = 0;
        move_enemies();
    }
}

// 업그레이드 선택 화면 로직 처리
void update_choice_logic(int *prev_left, int *prev_right, int *prev_a) {
    if(button_pressed_edge(BTN_LEFT, prev_left)) {
        choice_selected = 0;
    }
    if(button_pressed_edge(BTN_RIGHT, prev_right)) {
        choice_selected = 1;
    }
    if(button_pressed_edge(BTN_A, prev_a)) {
        if(choice_selected == 0) {
            upgrade_a_8dir = 1;
        } else {
            skill_b_range += SKILL_B_RANGE_UPGRADE;
            if(skill_b_range > 140) skill_b_range = 140;
        }
        state = STATE_PLAY;
    }
    
    // 선택 화면 그리기 호출
    draw_choice_screen(choice_selected);
}
