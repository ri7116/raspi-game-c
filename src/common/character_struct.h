#pragma once

// 적 구조체
typedef struct {
    int x;
    int y;
    int active;
} Enemy;

// 총알 구조체
typedef struct {
    int x;
    int y;
    int dx;      // 이동 방향 X (-1, 0, 1)
    int dy;      // 이동 방향 Y (-1, 0, 1)
    int active;
} Bullet;

typedef enum {
    STATE_PLAY = 0,
    STATE_CHOICE = 1,
} GameState;
