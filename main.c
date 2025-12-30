#include "master.h"

int main(int argc, char **argv) {
    // 시스템 및 하드웨어 초기화
    if (!init_system()) {
        return 1;
    }
    
    // 게임 데이터 초기화
    init_enemies();
    init_bullets();

    player_x = ST7789_TFTWIDTH / 2 - PLAYER_SIZE / 2;
    player_y = ST7789_TFTHEIGHT / 2 - PLAYER_SIZE / 2;
    
    // 선택 화면 입력 감지용 지역 변수 (엣지 트리거)
    int prev_left = 0, prev_right = 0, prev_a = 0;
    
    while(!game_over) {
        // Case 1: 게임 진행
        if(state == STATE_PLAY) {
            // 1. 입력 처리
            if (button_is_down(BTN_UP)) player_y -= PLAYER_SPEED;
            if (button_is_down(BTN_DOWN)) player_y += PLAYER_SPEED;
            if (button_is_down(BTN_LEFT)) player_x -= PLAYER_SPEED;
            if (button_is_down(BTN_RIGHT)) player_x += PLAYER_SPEED;
            
            player_x = clamp_int(player_x, 0, ST7789_TFTWIDTH - PLAYER_SIZE);
            player_y = clamp_int(player_y, 0, ST7789_TFTHEIGHT - PLAYER_SIZE);
            
            if (button_is_down(BTN_A)) skill_a_fire();
            if (button_is_down(BTN_B)) skill_b_aoe();
            
            // 2. 쿨타임 및 발사체 업데이트
            if(skill_a_timer > 0) skill_a_timer--;
            if(skill_b_timer > 0) skill_b_timer--;
            
            move_bullets();
            check_bullet_collision();
            
            // 3. 게임 진행 및 타이머 업데이트 (스폰, 이동 등)
            update_game_logic(&prev_left, &prev_right, &prev_a);
            
            // 4. 충돌 체크 및 사망 판정
            if(check_player_collision()) game_over = 1;
            
            // 5. 렌더링
            draw_game();
        } 

        // Case 2: 능력 선택
        else update_choice_logic(&prev_left, &prev_right, &prev_a);
        
        delay(50);
    }
    
    draw_game_over();

    // Print results to terminal
    printf("\n============================\n");
    printf("        GAME OVER\n");
    printf("============================\n");
    printf("Survival Time: %d seconds\n", game_time);
    printf("Total Kills: %d\n", kill_count);
    printf("============================\n\n");

    delay(3000);
    bcm2835_close();
    return 0;
}
