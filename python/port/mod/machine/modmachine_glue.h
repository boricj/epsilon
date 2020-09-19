#include <stddef.h>

typedef struct GPIO_PIN GPIO_PIN;
GPIO_PIN* gpio_get_pin(size_t pin);
void gpio_set_pull_none(GPIO_PIN* pin);
void gpio_set_pull_down(GPIO_PIN* pin);
void gpio_set_pull_up(GPIO_PIN* pin);
void gpio_set_input(GPIO_PIN* pin);
void gpio_set_output(GPIO_PIN* pin);
int gpio_get_value(GPIO_PIN* pin);
void gpio_set_value(GPIO_PIN* pin, int value);
