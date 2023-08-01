#ifndef GPIO_BUTTON_H
#define GPIO_BUTTON_H

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#define GPIO_DEVICE_IS_NOT_READY 1U
#define GPIO_PIN_CONFIG_DT_FAILED 2U

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_NODELABEL(button0), gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led0), gpios);
static struct gpio_callback button_cb_data;

int button_init(void);
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

#endif