/**
 *  Copyright Notice:
 *  Copyright 2025 Intel. All rights reserved.
 *  License: BSD 3-Clause License.
 **/

#ifndef __SOC_CXL_TEST_H__
#define __SOC_CXL_TEST_H__

#include <stdint.h>
#include <stdbool.h>
#include <sys/param.h>

// TODO
// Test cases are added here
typedef enum {
  SOC_CXL_TEST_CASE_IDE_CAPABILITY_STRUCTURE = 0,
  SOC_CXL_TEST_CASE_NUM
} SOC_TEST_CASE;

#define MAX_SOC_CXL_TEST_CASE_IDE_CAPABILITY_STRUCTURE_CASE_ID 1

typedef enum {
  SOC_CXL_TEST_CONFIGURATION_TYPE_DEFAULT = 0,
  SOC_CXL_TEST_CONFIGURATION_TYPE_NUM
} SOC_CXL_TEST_CONFIGURATION_TYPE;

#define SOC_CXL_TEST_CONFIGURATION_BITMASK 1

#endif