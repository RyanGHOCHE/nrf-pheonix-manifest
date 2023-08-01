#include "gpio_button.h"

extern uint32_t irq_status;
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    gpio_pin_toggle_dt(&led);
	printk("irq-status: %d\n",irq_status);
    irq_status = 0x00;
}

int button_init(void)
{
	int err;
	if (!device_is_ready(led.port)) {
		return GPIO_DEVICE_IS_NOT_READY;
	}
	if (!device_is_ready(button.port)) {
		return GPIO_DEVICE_IS_NOT_READY;
	}
	err = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (err < 0) {
		return GPIO_PIN_CONFIG_DT_FAILED;
	}
	err = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (err < 0) {
		return GPIO_PIN_CONFIG_DT_FAILED;
	}
	err = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE );
    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	return 0;
}