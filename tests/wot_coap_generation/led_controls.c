#include <stdio.h>
#include <string.h>

#include "msg.h"
#include "led.h"
#include "periph/gpio.h"

#ifndef IS_NATIVE

bool led_on;

bool is_led_on(void) {
    return led_on;
}

char* get_led_status(void) {
    if (led_on) {
        return "On";
    }
    else {
        return "Off";
    }
}

void turn_on_led(void) {
    led_on = true;
    #ifdef LED_GPIO
    gpio_set(LED_GPIO);
    #endif
}

void turn_off_led(void) {
    led_on = false;
    #ifdef LED_GPIO
    gpio_clear(LED_GPIO);
    #endif
}

void toggle_led(void) {
    led_on = !led_on;
    #ifdef LED_GPIO
    gpio_toggle(LED_GPIO);
    #endif
}

// Toggle LED (default GPIO: 33, cf. Makefile)

int led_cmd(int argc, char **argv) {
    if ( argc != 2 ) {
        (void) puts("Usage: led [on|off|toggle|status]");
        return 1;
    }
    if ( strcmp( argv[1] , "on" ) == 0 ) {
        turn_on_led();
        return 0;
    }

    if ( strcmp( argv[1] , "off" ) == 0 ) {
        turn_off_led();
        return 0;
    }

    if ( strcmp( argv[1] , "toggle" ) == 0 ) {
        toggle_led();
        return 0;
    }

    if ( strcmp( argv[1] , "status" ) == 0 ) {
        puts(get_led_status());
        return 0;
    }

    puts("led: Invalid command");
    return 1;
}

void led_cmd_init(void) {
    #ifdef LED_GPIO
    gpio_init(LED_GPIO, GPIO_OUT);
    gpio_set(LED_GPIO);
    #endif
    led_on = true;
}

#endif
