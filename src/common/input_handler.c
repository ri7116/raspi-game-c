#include "input_handler.h"

int button_is_down(uint8_t pin) {
    return (bcm2835_gpio_lev(pin) == 0); 
}

int button_pressed_edge(uint8_t pin, int *prev_down) {
    int down = button_is_down(pin);
    int pressed = (down && !(*prev_down));
    *prev_down = down;
    return pressed;
}
