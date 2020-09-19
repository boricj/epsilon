extern "C" {
#include "modmachine_glue.h"
}

#include <ion/gpio.h>

GPIO_PIN* gpio_get_pin(size_t pin) {
    return (GPIO_PIN*)Ion::GPIO::getPin(pin);
}

void gpio_set_pull_none(GPIO_PIN* pin) {
    ((Ion::GPIO::Pin const*)pin)->setPullNone();
}

void gpio_set_pull_down(GPIO_PIN* pin) {
    ((Ion::GPIO::Pin const*)pin)->setPullDown();
}

void gpio_set_pull_up(GPIO_PIN* pin) {
    ((Ion::GPIO::Pin const*)pin)->setPullUp();
}

void gpio_set_input(GPIO_PIN* pin) {
    ((Ion::GPIO::Pin const*)pin)->setInput();
}

void gpio_set_output(GPIO_PIN* pin) {
    ((Ion::GPIO::Pin const*)pin)->setOutput();
}

int gpio_get_value(GPIO_PIN* pin) {
    return ((Ion::GPIO::Pin const*)pin)->value();
}

void gpio_set_value(GPIO_PIN* pin, int value) {
    ((Ion::GPIO::Pin const*)pin)->setValue(value);
}
