#include "scan_api.h"

struct bt_scan_manufacturer_data manufacturer_data; //app api
struct bt_conn *auth_conn;
struct bt_conn *default_conn; //scan

void set_manufacturer_data (struct bt_scan_manufacturer_data usr_manufacturer_data)
{
	manufacturer_data = usr_manufacturer_data;
}

struct bt_scan_manufacturer_data get_manufacturer_data ()
{
	return manufacturer_data;
}

void scan_filter_match(struct bt_scan_device_info *device_info,
			      struct bt_scan_filter_match *filter_match,
			      bool connectable)
{
    static char addr[BT_ADDR_LE_STR_LEN];
	if (!filter_match->manufacturer_data.match) {

		printk("Invalid device connected\n");

		return;
	}
	// if (device_info->recv_info->rssi < rssi_tresh)
	// {
	// 	printk("RSSI signal low\n");
	// 	return;
	// }
	bt_addr_le_to_str(device_info->recv_info->addr, addr, sizeof(addr));
    printk("Address:%s\n",addr);
}

void scan_connecting_error(struct bt_scan_device_info *device_info)
{
	printk("Connecting failed\n");
}

void scan_connecting(struct bt_scan_device_info *device_info,
			    struct bt_conn *conn)
{
	default_conn = bt_conn_ref(conn);
}
/** .. include_startingpoint_scan_rst */
void scan_filter_no_match(struct bt_scan_device_info *device_info,
				 bool connectable)
{
	int err;
	struct bt_conn *conn;
	char addr[BT_ADDR_LE_STR_LEN];
	if (device_info->recv_info->adv_type == BT_GAP_ADV_TYPE_ADV_DIRECT_IND) {
		bt_addr_le_to_str(device_info->recv_info->addr, addr,
				  sizeof(addr));
		printk("Direct advertising received from %s\n", addr);
		bt_scan_stop();

		err = bt_conn_le_create(device_info->recv_info->addr,
					BT_CONN_LE_CREATE_CONN,
					device_info->conn_param, &conn);

		if (!err) {
			default_conn = bt_conn_ref(conn);
			bt_conn_unref(conn);
		}
	}
}

/** .. include_endpoint_scan_rst */
BT_SCAN_CB_INIT(scan_cb, scan_filter_match, scan_filter_no_match,
		scan_connecting_error, scan_connecting);

void scan_init(void)
{
	int err;

	struct bt_scan_init_param scan_init = {
		.connect_if_match = 1,
		.scan_param = NULL,
		.conn_param = BT_LE_CONN_PARAM_DEFAULT
	};

	bt_scan_init(&scan_init);
	bt_scan_cb_register(&scan_cb);

	err = bt_scan_filter_add(BT_SCAN_FILTER_TYPE_MANUFACTURER_DATA, &manufacturer_data);
	if (err) {
		printk("Scanning filters cannot be set (err %d)\n", err);
		return;
	}

	err = bt_scan_filter_enable(BT_SCAN_MANUFACTURER_DATA_FILTER, false);
	if (err) {
		printk("Filters cannot be turned on (err %d)\n", err);
	}
}

/**************************/

void discovery_completed_cb(struct bt_gatt_dm *dm,
				   void *context)
{
	int err;

	printk("The discovery procedure succeeded\n");

	bt_gatt_dm_data_print(dm);

	err = bt_hogp_handles_assign(dm, get_hogp());
	if (err) {
		printk("Could not init HIDS client object, error: %d\n", err);
	}

	err = bt_gatt_dm_data_release(dm);
	if (err) {
		printk("Could not release the discovery data, error "
		       "code: %d\n", err);
	}
}

void discovery_service_not_found_cb(struct bt_conn *conn,
					   void *context)
{
	printk("The service could not be found during the discovery\n");
}

void discovery_error_found_cb(struct bt_conn *conn,
				     int err,
				     void *context)
{
	printk("The discovery procedure failed with %d\n", err);
}

void gatt_discover(struct bt_conn *conn)
{
	int err;

	if (conn != default_conn) {
		return;
	}

	err = bt_gatt_dm_start(conn, BT_UUID_HIDS, &discovery_cb, NULL);
	if (err) {
		printk("could not start the discovery procedure, error "
			"code: %d\n", err);
	}
}

/***************/

void connected(struct bt_conn *conn, uint8_t conn_err)
{
	int err;
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (conn_err) {
		printk("Failed to connect to %s (%u)\n", addr, conn_err);
		if (conn == default_conn) {
			bt_conn_unref(default_conn);
			default_conn = NULL;

			/* This demo doesn't require active scan */
			err = bt_scan_start(BT_SCAN_TYPE_SCAN_PASSIVE);
			if (err) {
				printk("Scanning failed to start (err %d)\n",
				       err);
			}
		}

		return;
	}

	printk("Connected: %s\n", addr);

	err = bt_conn_set_security(conn, BT_SECURITY_L2);
	if (err) {
		printk("Failed to set security: %d\n", err);

		gatt_discover(conn);
	}
}

void disconnected(struct bt_conn *conn, uint8_t reason)
{
	char addr[BT_ADDR_LE_STR_LEN];
	int err;

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (auth_conn) {
		bt_conn_unref(auth_conn);
		auth_conn = NULL;
	}

	printk("Disconnected: %s (reason %u)\n", addr, reason);

	if (bt_hogp_assign_check(get_hogp())) {
		printk("HIDS client active - releasing");
		bt_hogp_release(get_hogp());
	}

	if (default_conn != conn) {
		return;
	}

	bt_conn_unref(default_conn);
	default_conn = NULL;

	/* This demo doesn't require active scan */
	err = bt_scan_start(BT_SCAN_TYPE_SCAN_PASSIVE);
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
	}
}

void security_changed(struct bt_conn *conn, bt_security_t level,
			     enum bt_security_err err)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	if (!err) {
		printk("Security changed: %s level %u\n", addr, level);
	} else {
		printk("Security failed: %s level %u err %d\n", addr, level,
			err);
	}

	gatt_discover(conn);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected        = connected,
	.disconnected     = disconnected,
	.security_changed = security_changed
};