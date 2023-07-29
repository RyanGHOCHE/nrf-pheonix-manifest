#include <zephyr/types.h>
#include <stdbool.h>
#include <bluetooth/scan.h>
#include "gap_layer.h"

#define FW_VERSION 00000001
#define API_VERSION 00000001

typedef uint32_t api_error;
uint32_t get_api_version(void);
uint32_t get_fw_version(void);
api_error set_scan(bool status);
void set_filter(uint8_t * manuf_id, uint8_t length);
const void * get_manufacturer_id (void);

void set_scan_timeout_min(uint32_t timeout);
