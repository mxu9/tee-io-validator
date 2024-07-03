/**
 *  Copyright Notice:
 *  Copyright 2023-2024 Intel. All rights reserved.
 *  License: BSD 3-Clause License.
 **/

#ifndef __PCIE_IDE_H__
#define __PCIE_IDE_H__

#include "intel_keyp.h"
#include "pcie.h"

// Follow doc/IdeKmTestCase
#define MAX_QUERY_CASE_ID 2
#define MAX_KEYPROG_CASE_ID 6
#define MAX_KSETGO_CASE_ID 4
#define MAX_KSETSTOP_CASE_ID 4
#define MAX_SPDMSESSION_CASE_ID 2
#define MAX_FULL_CASE_ID 1
#define MAX_PCIE_CASE_ID \
  (MAX(MAX_QUERY_CASE_ID, MAX(MAX_KEYPROG_CASE_ID, MAX(MAX_KSETGO_CASE_ID, MAX(MAX_KSETSTOP_CASE_ID, MAX(MAX_SPDMSESSION_CASE_ID, MAX_FULL_CASE_ID))))))

typedef enum {
  IDE_COMMON_TEST_CASE_QUERY = 0,
  IDE_COMMON_TEST_CASE_KEYPROG,
  IDE_COMMON_TEST_CASE_KSETGO,
  IDE_COMMON_TEST_CASE_KSETSTOP,
  IDE_COMMON_TEST_CASE_SPDMSESSION,
  IDE_COMMON_TEST_CASE_TEST,
  IDE_COMMON_TEST_CASE_NUM
} IDE_COMMON_TEST_CASE;

typedef enum {
  IDE_TEST_CONFIGURATION_TYPE_DEFAULT = 0,
  IDE_TEST_CONFIGURATION_TYPE_SWITCH,
  IDE_TEST_CONFIGURATION_TYPE_PARTIAL_HEADER_ENC,
  IDE_TEST_CONFIGURATION_TYPE_PCRC,
  IDE_TEST_CONFIGURATION_TYPE_AGGGEG,
  IDE_TEST_CONFIGURATION_TYPE_SELECTIVE_IDE_FOR_CONFIG,
  IDE_TEST_CONFIGURATION_TYPE_TEE_LIMITED_STREAM,
  IDE_TEST_CONFIGURATION_TYPE_NUM
} IDE_TEST_CONFIGURATION_TYPE;

#define BIT_MASK(n) ((uint32_t)(1<<n))

#define SELECTIVE_IDE_CONFIGURATION_BITMASK \
  ((BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_DEFAULT)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_SWITCH)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_PARTIAL_HEADER_ENC)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_PCRC)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_AGGGEG)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_SELECTIVE_IDE_FOR_CONFIG)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_TEE_LIMITED_STREAM)))

#define LINK_IDE_CONFIGURATION_BITMASK \
  ((BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_DEFAULT)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_SWITCH)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_PARTIAL_HEADER_ENC)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_PCRC)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_AGGGEG)))

#define SELECTIVE_LINK_IDE_CONFIGURATION_BITMASK \
  ((BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_DEFAULT)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_PARTIAL_HEADER_ENC)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_PCRC)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_SELECTIVE_IDE_FOR_CONFIG)) | \
  (BIT_MASK(IDE_TEST_CONFIGURATION_TYPE_AGGGEG)))

typedef struct {
  uint32_t signature;

  INTEL_KEYP_PCIE_STREAM_CAP stream_cap;

  // ecap related data
  PCIE_IDE_CAP ide_cap;
  // ide_id
  uint8_t ide_id;
  // rid/addr assoc_reg_block
  PCIE_SEL_IDE_RID_ASSOC_REG_BLOCK rid_assoc_reg_block;
  PCIE_SEL_IDE_ADDR_ASSOC_REG_BLOCK addr_assoc_reg_block;
} PCIE_PRIV_DATA;


#endif