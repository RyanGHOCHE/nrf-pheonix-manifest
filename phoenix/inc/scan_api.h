#ifndef SCAN_API_H
#define SCAN_API_H
#include <bluetooth/scan.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include "hogp_api.h"
#define ERR -1

int set_manufacturer_data (struct bt_scan_manufacturer_data usr_manufacturer_data);

struct bt_scan_manufacturer_data get_manufacturer_data ();

/**********************************************************/

void scan_filter_match(struct bt_scan_device_info *device_info,
			      struct bt_scan_filter_match *filter_match,
			      bool connectable);

void scan_connecting_error(struct bt_scan_device_info *device_info);

void scan_connecting(struct bt_scan_device_info *device_info,
			    struct bt_conn *conn);

void scan_filter_no_match(struct bt_scan_device_info *device_info,
				 bool connectable);

int scan_init(void);

/***********************/

void discovery_completed_cb(struct bt_gatt_dm *dm,
				   void *context);

void discovery_service_not_found_cb(struct bt_conn *conn,
					   void *context);

void discovery_error_found_cb(struct bt_conn *conn,
				     int err,
				     void *context);

void gatt_discover(struct bt_conn *conn);

static const struct bt_gatt_dm_cb discovery_cb = {
	.completed = discovery_completed_cb,
	.service_not_found = discovery_service_not_found_cb,
	.error_found = discovery_error_found_cb,
};

void connected(struct bt_conn *conn, uint8_t conn_err);

void disconnected(struct bt_conn *conn, uint8_t reason);

void security_changed(struct bt_conn *conn, bt_security_t level,
			     enum bt_security_err err);
#endif