#include <stdlib.h>
#include "st7789.h"
#include "master.h"

// --- 내부 보조 함수 ---
static void draw_pixel(int x, int y, uint16_t color) {
    st7789_drawRect(x, y, 1, 1, color);
}

static void draw_line(int x0, int y0, int x1, int y1, uint16_t color) {
    int dx = abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;

    while(1) {
        draw_pixel(x0, y0, color);
        if(x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if(e2 >= dy) { err += dy; x0 += sx; }
        if(e2 <= dx) { err += dx; y0 += sy; }
    }
}

static void draw_frame(int x, int y, int w, int h, uint16_t color) {
    st7789_drawRect(x, y, w, 2, color);
    st7789_drawRect(x, y + h - 2, w, 2, color);
    st7789_drawRect(x, y, 2, h, color);
    st7789_drawRect(x + w - 2, y, 2, h, color);
}

static void draw_icon_8dir_arrows(int x, int y, int w, int h, uint16_t color) {
    int cx = x + w / 2;
    int cy = y + h / 2;
    int len = (w < h ? w : h) / 2 - 6;
    int ah = 4;

    int dirs[8][2] = {
        { 0, -1}, { 0,  1}, {-1,  0}, { 1,  0},
        {-1, -1}, { 1, -1}, {-1,  1}, { 1,  1}
    };

    for(int i = 0; i < 8; i++) {
        int dx = dirs[i][0];
        int dy = dirs[i][1];
        int ex = cx + dx * len;
        int ey = cy + dy * len;
        draw_line(cx, cy, ex, ey, color);
        int px = -dy;
        int py = dx;
        draw_line(ex, ey, ex - dx * ah + px * ah, ey - dy * ah + py * ah, color);
        draw_line(ex, ey, ex - dx * ah - px * ah, ey - dy * ah - py * ah, color);
    }
    st7789_drawRect(cx - 2, cy - 2, 5, 5, color);
}

static void draw_icon_b_range_up(int x, int y, int w, int h, uint16_t color) {
    int cx = x + w / 2;
    int cy = y + h / 2;
    int outer = (w < h ? w : h) - 8;
    int inner = outer - 18;
    draw_frame(cx - outer/2, cy - outer/2, outer, outer, color);
    draw_frame(cx - inner/2, cy - inner/2, inner, inner, color);
    st7789_drawRect(cx - 4, cy - 4, 9, 9, color);
    draw_line(cx, cy - outer/2 + 6, cx, cy - outer/2 + 18, color);
    draw_line(cx, cy + outer/2 - 6, cx, cy + outer/2 - 18, color);
    draw_line(cx - outer/2 + 6, cy, cx - outer/2 + 18, cy, color);
    draw_line(cx + outer/2 - 6, cy, cx + outer/2 - 18, cy, color);
}

// --- 공개 렌더링 함수 ---
void draw_choice_screen(int selected_idx) {
    st7789_clearScreen(COLOR_BLACK);
    int card_w = 100;
    int card_h = 140;
    int gap = 20;
    int total_w = card_w * 2 + gap;
    int start_x = (ST7789_TFTWIDTH - total_w) / 2;
    int y = (ST7789_TFTHEIGHT - card_h) / 2;

    for(int i = 0; i < 2; i++) {
        int x = start_x + i * (card_w + gap);
        uint16_t border = (i == selected_idx) ? COLOR_YELLOW : 0x8410;
        uint16_t fill = 0x1082;
        st7789_drawRect(x, y, card_w, card_h, fill);
        draw_frame(x, y, card_w, card_h, border);
        int icon_pad = 14;
        int icon_x = x + icon_pad;
        int icon_y = y + icon_pad;
        int icon_w = card_w - icon_pad * 2;
        int icon_h = card_h - icon_pad * 2;
        if(i == 0) draw_icon_8dir_arrows(icon_x, icon_y, icon_w, icon_h, COLOR_WHITE);
        else draw_icon_b_range_up(icon_x, icon_y, icon_w, icon_h, COLOR_WHITE);
    }
    st7789_render();
}

void draw_game() {
    st7789_clearScreen(COLOR_BLACK);
    if(skill_b_timer == 0) {
        int r = skill_b_range;
        int rx = player_x + PLAYER_SIZE/2 - r/2;
        int ry = player_y + PLAYER_SIZE/2 - r/2;
        st7789_drawRect(rx, ry, r, 1, 0x0410);
        st7789_drawRect(rx, ry + r, r, 1, 0x0410);
        st7789_drawRect(rx, ry, 1, r, 0x0410);
        st7789_drawRect(rx + r, ry, 1, r, 0x0410);
    }
    st7789_drawRect(player_x, player_y, PLAYER_SIZE, PLAYER_SIZE, COLOR_GREEN);
    for(int i = 0; i < MAX_ENEMIES; i++) {
        if(enemies[i].active) st7789_drawRect(enemies[i].x, enemies[i].y, ENEMY_SIZE, ENEMY_SIZE, COLOR_RED);
    }
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(bullets[i].active) st7789_drawRect(bullets[i].x, bullets[i].y, BULLET_SIZE, BULLET_SIZE, COLOR_YELLOW);
    }
    st7789_render();
}

void draw_game_over() {
    st7789_clearScreen(COLOR_RED);
    st7789_render();
}
