#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/settings/settings.h>
#include <dk_buttons_and_leds.h>

#include "hogp_api.h"
#include "ble_api.h"

#define KEY_BTN1_MASK DK_BTN1_MSK
#define SIZE 5

static uint8_t volume_state;

static void button_test(void)
{
	int err;
	struct bt_hogp_rep_info *rep = NULL;
	uint8_t data [2];

	if (!bt_hogp_ready_check(get_hogp())) {
		printk("HID device not ready\n");
		return;
	}

	volume_state = volume_state ? 0 : 1;
	data[0] = volume_state ? 0x32 : 0x25;
	data[1]=0x01;

	while (NULL != (rep = bt_hogp_rep_next(get_hogp(), rep)))
	{
		if (bt_hogp_rep_type(rep) ==
		    BT_HIDS_REPORT_TYPE_OUTPUT) {
			printk("Subscribe to output report id: %u\n",
			       bt_hogp_rep_id(rep));
			err = bt_hogp_rep_write_wo_rsp(get_hogp(),rep,
				       &data, sizeof(data),
				       hidc_write_cb);

			if (err) {
				printk("peripheral device data write error (err: %d)\n", err);
				return;
			}
	}
	}
	printk("Volume send (val: 0x%x)\n", data[0]);
}

static void button_handler(uint32_t button_state, uint32_t has_changed)
{
	uint32_t button = button_state & has_changed;

	if (button & KEY_BTN1_MASK) {
		button_test();
	}
}

int main(void)
{
	uint8_t data[SIZE]={0x58,0x02,0x0F,0x00,0xA5};
	set_filter(data,SIZE);
	int err;

	hogp_init();

	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return 0;
	}

	printk("Bluetooth initialized\n");

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	err = dk_buttons_init(button_handler);
	if (err)
	{
		printk("Failed to initialize buttons (err %d)\n", err);
		return 0;
	}

	scan_init();
	err = scan_start();
	if (err)
	{
		printk("Failed to start bluetooth scanning (err %d)\n", err);
		return 0;
	}
	while (true)
	{
		printk("APP REMOTE VOLUME = %u\n",get_remote_volume());
		printk("APP REMOTE STATUS = %x\n",get_remote_status());
		k_msleep(1000);
	}
	return 0;
}