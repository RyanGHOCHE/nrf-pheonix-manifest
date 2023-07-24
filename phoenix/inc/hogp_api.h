#ifndef HOGP_API_H
#define HOGP_API_H

#include <zephyr/kernel.h>
#include <bluetooth/services/hogp.h>

struct bt_hogp * get_hogp(void);
void hogp_init(void);
uint8_t notification_cb(uint8_t rep);

uint8_t hogp_notify_cb(struct bt_hogp *hogp,
			     struct bt_hogp_rep_info *rep,
			     uint8_t err,
			     const uint8_t *data);

void hogp_ready_cb(struct bt_hogp *hogp);

void hids_on_ready(struct k_work *work);

void hogp_prep_fail_cb(struct bt_hogp *hogp, int err);

void hidc_write_cb(struct bt_hogp *hidc,
			  struct bt_hogp_rep_info *rep,
			  uint8_t err);
#endif