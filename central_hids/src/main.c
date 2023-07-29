/* main.c - Application main entry point */

/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <bluetooth/gatt_dm.h>
#include <zephyr/sys/byteorder.h>
#include <bluetooth/scan.h>
#include <bluetooth/services/hogp.h>
#include <dk_buttons_and_leds.h>
#include "gap_layer.h"
#include <zephyr/settings/settings.h>
#include "api.h"

#define ENABLE true
#define MANUFACTURER_ID_SIZE 5
extern struct bt_hogp hogp;
extern const struct bt_hogp_init_params hogp_init_params;
extern struct bt_conn_auth_cb conn_auth_callbacks;
extern struct bt_conn_auth_info_cb conn_auth_info_callbacks;

void main(void)
{
	int err;

	printk("Starting Bluetooth Central HIDS example\n");

	hogp_init();

	// err = bt_conn_auth_cb_register(&conn_auth_callbacks);
	// if (err) {
	// 	printk("failed to register authorization callbacks.\n");
	// 	return 0;
	// }

	// err = bt_conn_auth_info_cb_register(&conn_auth_info_callbacks);
	// if (err) {
	// 	printk("Failed to register authorization info callbacks.\n");
	// 	return 0;
	// }

	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	// err = dk_buttons_init(button_handler);
	// if (err) {
	// 	printk("Failed to initialize buttons (err %d)\n", err);
	// 	return 0;
	// }

    uint8_t manufacturer_id [MANUFACTURER_ID_SIZE]={0x58,0x02,0x0f,0x00,0xa5};
    set_filter(&manufacturer_id,MANUFACTURER_ID_SIZE);
    set_scan_timeout_min(1);
    scan_init();
    err = set_scan(ENABLE);
    if (err)
    {
        printk("set_scan failed\n (err %d)\n", err);
        return;
    }
	printk("Scanning successfully started\n");
}
