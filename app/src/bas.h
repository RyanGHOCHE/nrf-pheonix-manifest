#ifndef BAS_H
#define BAS_H
#include <bluetooth/services/bas_client.h>
#include <zephyr/bluetooth/addr.h>
#include <zephyr/sys/printk.h>
#include <dk_buttons_and_leds.h>
#define KEY_READVAL_MASK DK_BTN1_MSK
#define BAS_READ_VALUE_INTERVAL (10 * MSEC_PER_SEC)
void notify_battery_level_cb(struct bt_bas_client *bas,
				    uint8_t battery_level);
void button_readval(void);
void button_handler(uint32_t button_state, uint32_t has_changed);
#endif