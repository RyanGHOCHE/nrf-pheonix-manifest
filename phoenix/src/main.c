#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/settings/settings.h>
#include <zephyr/kernel.h>
#include "gpio_button.h"

#define SLEEP_TIME_MS   10*60*1000
#define SIZE 5

void main(void)
{
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}

	printk("Bluetooth initialized\n");

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

    err = button_init();
    if (err)
    {
        printk("Button init failed\n");
        return;
    }
    printk("Debug button init\n");

	while (1) {
        k_msleep(SLEEP_TIME_MS);
	}

}