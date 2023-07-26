

// /**
//  * Switch between boot protocol and report protocol mode.
//  */
// #define KEY_BOOTMODE_MASK DK_BTN2_MSK
// /**
//  * Switch CAPSLOCK state.
//  *
//  * @note
//  * For simplicity of the code it works only in boot mode.
//  */
// #define KEY_CAPSLOCK_MASK DK_BTN1_MSK
// /**
//  * Switch CAPSLOCK state with response
//  *
//  * Write CAPSLOCK with response.
//  * Just for testing purposes.
//  * The result should be the same like usine @ref KEY_CAPSLOCK_MASK
//  */
// #define KEY_CAPSLOCK_RSP_MASK DK_BTN3_MSK

// /* Key used to accept or reject passkey value */
// #define KEY_PAIRING_ACCEPT DK_BTN1_MSK
// #define KEY_PAIRING_REJECT DK_BTN2_MSK


// static uint8_t capslock_state;




// static void button_bootmode(void)
// {
// 	if (!bt_hogp_ready_check(&hogp)) {
// 		printk("HID device not ready\n");
// 		return;
// 	}
// 	int err;
// 	enum bt_hids_pm pm = bt_hogp_pm_get(&hogp);
// 	enum bt_hids_pm new_pm = ((pm == BT_HIDS_PM_BOOT) ? BT_HIDS_PM_REPORT : BT_HIDS_PM_BOOT);

// 	printk("Setting protocol mode: %s\n", (new_pm == BT_HIDS_PM_BOOT) ? "BOOT" : "REPORT");
// 	err = bt_hogp_pm_write(&hogp, new_pm);
// 	if (err) {
// 		printk("Cannot change protocol mode (err %d)\n", err);
// 	}
// }

// static void button_capslock(void)
// {
// 	int err;
// 	uint8_t data;

// 	if (!bt_hogp_ready_check(&hogp)) {
// 		printk("HID device not ready\n");
// 		return;
// 	}
// 	if (!hogp.rep_boot.kbd_out) {
// 		printk("HID device does not have Keyboard OUT report\n");
// 		return;
// 	}
// 	if (bt_hogp_pm_get(&hogp) != BT_HIDS_PM_BOOT) {
// 		printk("This function works only in BOOT Report mode\n");
// 		return;
// 	}
// 	capslock_state = capslock_state ? 0 : 1;
// 	data = capslock_state ? 0x02 : 0;
// 	err = bt_hogp_rep_write_wo_rsp(&hogp, hogp.rep_boot.kbd_out,
// 				       &data, sizeof(data),
// 				       hidc_write_cb);

// 	if (err) {
// 		printk("Keyboard data write error (err: %d)\n", err);
// 		return;
// 	}
// 	printk("Caps lock send (val: 0x%x)\n", data);
// }


// static uint8_t capslock_read_cb(struct bt_hogp *hogp,
// 			     struct bt_hogp_rep_info *rep,
// 			     uint8_t err,
// 			     const uint8_t *data)
// {
// 	if (err) {
// 		printk("Capslock read error (err: %u)\n", err);
// 		return BT_GATT_ITER_STOP;
// 	}
// 	if (!data) {
// 		printk("Capslock read - no data\n");
// 		return BT_GATT_ITER_STOP;
// 	}
// 	printk("Received data (size: %u, data[0]: 0x%x)\n",
// 	       bt_hogp_rep_size(rep), data[0]);

// 	return BT_GATT_ITER_STOP;
// }


// static void capslock_write_cb(struct bt_hogp *hogp,
// 			      struct bt_hogp_rep_info *rep,
// 			      uint8_t err)
// {
// 	int ret;

// 	printk("Capslock write result: %u\n", err);

// 	ret = bt_hogp_rep_read(hogp, rep, capslock_read_cb);
// 	if (ret) {
// 		printk("Cannot read capslock value (err: %d)\n", ret);
// 	}
// }


// static void button_capslock_rsp(void)
// {
// 	if (!bt_hogp_ready_check(&hogp)) {
// 		printk("HID device not ready\n");
// 		return;
// 	}
// 	if (!hogp.rep_boot.kbd_out) {
// 		printk("HID device does not have Keyboard OUT report\n");
// 		return;
// 	}
// 	int err;
// 	uint8_t data;

// 	capslock_state = capslock_state ? 0 : 1;
// 	data = capslock_state ? 0x02 : 0;
// 	err = bt_hogp_rep_write(&hogp, hogp.rep_boot.kbd_out, capslock_write_cb,
// 				&data, sizeof(data));
// 	if (err) {
// 		printk("Keyboard data write error (err: %d)\n", err);
// 		return;
// 	}
// 	printk("Caps lock send using write with response (val: 0x%x)\n", data);
// }


// static void num_comp_reply(bool accept)
// {
// 	if (accept) {
// 		bt_conn_auth_passkey_confirm(auth_conn);
// 		printk("Numeric Match, conn %p\n", auth_conn);
// 	} else {
// 		bt_conn_auth_cancel(auth_conn);
// 		printk("Numeric Reject, conn %p\n", auth_conn);
// 	}

// 	bt_conn_unref(auth_conn);
// 	auth_conn = NULL;
// }


// static void button_handler(uint32_t button_state, uint32_t has_changed)
// {
// 	uint32_t button = button_state & has_changed;

// 	if (auth_conn) {
// 		if (button & KEY_PAIRING_ACCEPT) {
// 			num_comp_reply(true);
// 		}

// 		if (button & KEY_PAIRING_REJECT) {
// 			num_comp_reply(false);
// 		}

// 		return;
// 	}

// 	if (button & KEY_BOOTMODE_MASK) {
// 		button_bootmode();
// 	}
// 	if (button & KEY_CAPSLOCK_MASK) {
// 		button_capslock();
// 	}
// 	if (button & KEY_CAPSLOCK_RSP_MASK) {
// 		button_capslock_rsp();
// 	}
// }
