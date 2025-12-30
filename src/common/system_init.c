#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "master.h"
#include "driver/st7789.h"

int init_system(void) {
    // 1. bcm2835 라이브러리 초기화
    if (!bcm2835_init()) {
        printf("bcm2835_init failed. Are you running as root?\n");
        return 0;
    }
    
    // 2. 난수 시드 설정
    srand(time(NULL));
    
    // 3. GPIO 설정 (TFT 제어 핀 및 백라이트)
    bcm2835_gpio_fsel(TFT_DC, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(TFT_RST, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(26, BCM2835_GPIO_FSEL_OUTP); // 백라이트 핀
    bcm2835_gpio_set(26);

    // 4. GPIO 설정 (입력 버튼들)
    uint8_t btns[] = {BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_A, BTN_B};
    for(int i = 0; i < 6; i++) {
        bcm2835_gpio_fsel(btns[i], BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pud(btns[i], BCM2835_GPIO_PUD_UP);
    }
    
    // 5. 디스플레이 드라이버 초기화
    st7789_init();
    
    return 1;
}
