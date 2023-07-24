#include "hogp_api.h"

static struct bt_hogp hogp;
static uint8_t remote_volume=0;
static uint8_t remote_status=0;
static uint8_t button_count=0;

K_WORK_DEFINE(hids_ready_work, hids_on_ready);

static const struct bt_hogp_init_params hogp_init_params = {
	.ready_cb      = hogp_ready_cb,
	.prep_error_cb = hogp_prep_fail_cb,
};

uint8_t notification_cb(uint8_t rep)
{
	if (rep == 4)
	{
		return remote_volume;
	}

	if (rep == 7)
	{
		return remote_status;
	}
	else
	{
		printk("Report id %d not supported",rep);
		return 1;
	}
}

uint8_t hogp_notify_cb(struct bt_hogp *hogp,
			     struct bt_hogp_rep_info *rep,
			     uint8_t err,
			     const uint8_t *data)
{
	// uint8_t size = bt_hogp_rep_size(rep);
	// uint8_t i;

	if (!data) {
		return BT_GATT_ITER_STOP;
	}

	if (bt_hogp_rep_id(rep)==4)
	{
		remote_volume=data[0];
	}
	if (bt_hogp_rep_id(rep)==7)
	{
		button_count = button_count + 1;
		if (button_count == 1)
		{
			remote_status=data[0];
		}
		if (button_count == 2)
		{
			button_count = 0;
		}
	}

	return BT_GATT_ITER_CONTINUE;
}

void hogp_ready_cb(struct bt_hogp *hogp)
{
	k_work_submit(&hids_ready_work);
}

void hids_on_ready(struct k_work *work)
{
	int err;
	struct bt_hogp_rep_info *rep = NULL;

	printk("HIDS is ready to work\n");

	while (NULL != (rep = bt_hogp_rep_next(&hogp, rep)))
	{
		if (bt_hogp_rep_type(rep) ==
		    BT_HIDS_REPORT_TYPE_INPUT)
		{
			printk("Subscribe to report id: %u\n",
			       bt_hogp_rep_id(rep));
			err = bt_hogp_rep_subscribe(&hogp, rep,
							   hogp_notify_cb);
			if (err)
			{
				printk("Subscribe error (%d)\n", err);
			}
		}
	}
}

void hogp_prep_fail_cb(struct bt_hogp *hogp, int err)
{
	printk("ERROR: HIDS client preparation failed!\n");
}

void hidc_write_cb(struct bt_hogp *hidc,
			  struct bt_hogp_rep_info *rep,
			  uint8_t err)
{
	printk("Volume sent\n");
}
void hogp_init(void)
{
	bt_hogp_init(&hogp, &hogp_init_params);
}

struct bt_hogp * get_hogp(void)
{
	return &hogp;
}