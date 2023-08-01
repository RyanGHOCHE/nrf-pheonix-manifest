
#include "api.h"
struct bt_scan_manufacturer_data manufacturer_id ;

api_error set_scan(bool status)
{
    int err;
    if (status)
    {
        err = bt_scan_start(BT_SCAN_TYPE_SCAN_ACTIVE);
	    if (err)
        {
		    printk("Scanning failed to start (err %d)\n", err);
		    return err;
	    }
        return err;
    }
    err = bt_scan_stop();
    if (err)
    {
		printk("Scanning failed to stop (err %d)\n", err);
		return err;
	}
    return err;
}

void set_filter(uint8_t * manuf_id, uint8_t length)
{
    manufacturer_id.data = manuf_id;
    manufacturer_id.data_len = length;
}

const void * get_manufacturer_id (void)
{
    return &manufacturer_id;
}

uint32_t get_api_version(void)
{
    return API_VERSION;
}

uint32_t get_fw_version(void)
{
    return FW_VERSION;
}

void set_scan_timeout_min(uint32_t timeout)
{
    uint32_t tmp=0;
    tmp = timeout * 100 * 60;
    set_scan_timeout_value_s(tmp);

}
