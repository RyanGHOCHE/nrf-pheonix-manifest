//------------------------------------------------------------------------------
// Copyright (c) 2018 Devialet. All rights reserved.
//
// WARNING: This file is auto-generated!
//------------------------------------------------------------------------------

#ifndef _API_H_
#define _API_H_

#include <stdint.h>

#define API_VERSION 0x03060000
#define FW_VERSION 0x00000001
#define API_HEADER 0xf0
#define ARRAY_MAX_SIZE 64
#define PROTOCOL_ERROR_CRC 1
#define PROTOCOL_ERROR_UNKNOWN_FCT 2
#define PROTOCOL_ERROR_SIZE 3
#define API_ERROR 1

typedef uint32_t api_error_t;

/**
 * Get API version.
 * @return API version in 0xMMmmpppp format.
 */
static uint32_t get_api_version(void);

/**
 * Get firmware version.
 * @return Firmware version in 0xMMmmpppp format.
 */
static uint32_t get_fw_version(void);

/**
 * Load data into an ASP memory
 * @param offset Offset in word in selected memory
 * @param length Data length in word size
 * @param data Array to load into the memory
 * @return Number of words written
 */
static int32_t load_asp_memory(uint32_t offset, uint8_t length, int32_t *data);


#endif /* _API_H_ */