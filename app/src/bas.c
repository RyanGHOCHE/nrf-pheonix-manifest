#include "bas.h"

struct bt_bas_client bas;

void bas_client_init()
{
    bt_bas_client_init(&bas);
}

void notify_battery_level_cb(struct bt_bas_client *bas,
				    uint8_t battery_level)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(bt_bas_conn(bas)),
			  addr, sizeof(addr));
	if (battery_level == BT_BAS_VAL_INVALID) {
		printk("[%s] Battery notification aborted\n", addr);
	} else {
		printk("[%s] Battery notification: %"PRIu8"%%\n",
		       addr, battery_level);
	}
}

void read_battery_level_cb(struct bt_bas_client *bas,
				  uint8_t battery_level,
				  int err)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(bt_bas_conn(bas)),
			  addr, sizeof(addr));
	if (err) {
		printk("[%s] Battery read ERROR: %d\n", addr, err);
		return;
	}

	printk("[%s] Battery read: %"PRIu8"%%\n", addr, battery_level);
}

void button_readval(void)
{
	int err;

	printk("Reading BAS value:\n");
	err = bt_bas_read_battery_level(&bas, read_battery_level_cb);
	if (err) {
		printk("BAS read call error: %d\n", err);
	}
}

// void button_handler(uint32_t button_state, uint32_t has_changed)
// {
// 	uint32_t button = button_state & has_changed;

// 	if (button & KEY_READVAL_MASK) {
// 		button_readval();
// 	}
// }