#ifndef BLE_API_H
#define BLE_API_H

#include "scan_api.h"

void set_filter (uint8_t *manufacturer_id, uint8_t size);
int scan_start(void);
int scan_stop(void);
uint8_t get_remote_volume(void);
uint8_t get_remote_status(void);
#endif