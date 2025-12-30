#pragma once

#include <bcm2835.h>
#include <stdint.h>

// GPIO 핀 정의
#define BTN_UP    17
#define BTN_DOWN  22
#define BTN_LEFT  27
#define BTN_RIGHT 23
#define BTN_A     5
#define BTN_B     6

int button_is_down(uint8_t pin);
int button_pressed_edge(uint8_t pin, int *prev_down);
