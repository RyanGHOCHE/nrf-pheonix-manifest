#ifndef GAP_LAYER_H
#define GAP_LAYER_H
#include <bluetooth/scan.h>
#include <bluetooth/gatt_dm.h>
#include "api.h"
#include "bas.h"
void scan_init(void);
void set_scan_timeout_value_s(uint32_t value_s);
#endif