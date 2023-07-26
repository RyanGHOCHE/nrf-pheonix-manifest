#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/settings/settings.h>
#include <zephyr/kernel.h>
#include "api.h"
#include "gpio_button.h"

#define SLEEP_TIME_MS   10*60*1000
#define SIZE 5

void main(void)
{
	int err;
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
        printk("Button init failed (err %d)\n", err);
        return;
    }
    printk("Debug button init\n");

	printk ("FW_VERSION=%x\n",get_fw_version());
	printk ("API_VERSION=%x\n",get_api_version());

	uint8_t * manufacturer_id [5] = {0x58,0x02,0x0f,0x00,0xa5};
	err = set_filter(5,manufacturer_id);
	if (err)
	{
		printk("Set filter failed (err %d)\n", err);
	}

	while (1) {
        k_msleep(SLEEP_TIME_MS);
	}

}