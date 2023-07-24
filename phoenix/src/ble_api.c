#include "ble_api.h"

void set_filter (uint8_t *manufacturer_id, uint8_t size)
{
	struct bt_scan_manufacturer_data manufacturer_data;
	manufacturer_data.data=manufacturer_id;
    manufacturer_data.data_len = size;
	set_manufacturer_data(manufacturer_data);
}

int scan_start(void)
{
    int err;
    err = bt_scan_start(BT_SCAN_TYPE_SCAN_PASSIVE);
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return err;
	}
	printk("Scanning successfully started\n");
	return 0;
}

int scan_stop(void)
{
    int err;
    err = bt_scan_stop();
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return err;
	}
	return 0;
}

uint8_t get_remote_volume(void)
{
    return notification_cb(4);
}

uint8_t get_remote_status(void)
{
    return notification_cb(7);
}

