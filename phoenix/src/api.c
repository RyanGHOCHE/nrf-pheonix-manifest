#include "api.h"
#include "scan_api.h"

uint32_t get_api_version(void)
{
    return API_VERSION;
}

uint32_t get_fw_version(void)
{
    return FW_VERSION;
}

api_error_t set_filter(uint8_t size, uint8_t *manufacturer_id)
{
    struct bt_scan_manufacturer_data manufacturer_data;
	manufacturer_data.data=manufacturer_id;
    manufacturer_data.data_len = size;
	return set_manufacturer_data(manufacturer_data);
}

// api_error_t set_scan(bool state)
// {
//     int err;
//     if (state == 1)
//     {
//         err = bt_scan_start(BT_SCAN_TYPE_SCAN_PASSIVE);
// 	    if (err) {
// 		    printk("Scanning failed to start (err %d)\n", err);
// 		    return err;
// 	    }
//         printk("Scanning successfully started\n");
// 	    return 0;
//     }
//     else
//     {
//         err = bt_scan_stop();
// 	    if (err) {
// 		    printk("Scanning failed to start (err %d)\n", err);
// 		    return err;
// 	    }
//         printk("Scanning successfully stoped\n");
// 	    return 0;
//     }
// }