/**
 *  Copyright Notice:
 *  Copyright 2023-2024 Intel. All rights reserved.
 *  License: BSD 3-Clause License.
 **/

#ifndef __CXL_H__
#define __CXL_H__

#include <stdint.h>
#include "pcie.h"
//
// Compute Express Link (CXL) Spec
// August 2023, Revision 3.1
//

#pragma pack(1)

typedef union {
  struct {
    uint16_t  vendor_id;
    uint16_t  revision:4;
    uint16_t  length:12;
  };
  uint32_t raw;
} CXL_DVS_HEADER1;

typedef union {
  struct {
    uint16_t id;
  };
  uint16_t raw;
} CXL_DVS_HEADER2;

typedef union {
  struct {
    uint8_t cache_capable:1;
    uint8_t io_capable:1;
    uint8_t mem_capable:1;
    uint8_t mem_hwinit_mode:1;
    uint8_t hdm_count:2;
    uint8_t cache_writeback_and_invalidate_capable:1;
    uint8_t cxl_reset_capable:1;

    uint8_t cxl_reset_timeout:3;
    uint8_t cxl_mem_clr_capable:1;
    uint8_t tsp_capable:1;
    uint8_t multiple_logical_device:1;
    uint8_t viral_capable:1;
    uint8_t pm_init_completion_reporting_capable:1;
  };
  uint16_t raw;
} CXL_CAPABILITY;

typedef union {
  struct {
    uint8_t cache_enable:1;
    uint8_t io_enable:1;
    uint8_t mem_enable:1;
    uint8_t cache_sf_coverage:5;

    uint8_t cache_sf_granularity:3;
    uint8_t cache_clean_eviction:1;
    uint8_t direct_p2p_mem_enable:1;
    uint8_t rsvd0:1;
    uint8_t viral_enable:1;
    uint8_t rsvd1:1;
  };
  uint16_t raw;
} CXL_CONTROL;

typedef union {
  struct {
    uint16_t rsvd0:14;
    uint16_t viral_status:1;
    uint16_t rsvd1:1;
  };
  uint16_t raw;
} CXL_STATUS;

typedef union {
  struct {
    uint16_t disable_caching:1;
    uint16_t initiate_cache_write_back_and_invalidation:1;
    uint16_t initiate_cxl_reset:1;
    uint16_t cxl_reset_mem_clr_enable:1;
    uint16_t desired_volatile_hdm_state_after_hot_reset:1;
    uint16_t modified_completion_enable:1;
    uint16_t rsvd:10;
  };
  uint16_t raw;
} CXL_CONTROL2;

typedef union {
  struct {
    uint16_t cache_invalid:1;
    uint16_t cxl_reset_complete:1;
    uint16_t cxl_reset_error:1;
    uint16_t volatile_hdm_preservation_error:1;
    uint16_t rsvd:11;
    uint16_t power_management_initialization_complete:1;
  };
  uint16_t raw;
} CXL_STATUS2;

typedef union {
  struct {
    uint16_t config_lock:15;
    uint16_t rsvd:1;
  };
  uint16_t raw;
} CXL_LOCK;

typedef union {
  struct {
    uint8_t cache_size_unit:4;
    uint8_t fallback_capability:2;
    uint8_t modified_completion_capable:1;
    uint8_t no_clean_writeback:1;

    uint8_t cache_size;
  };
  uint16_t raw;
} CXL_CAPABILITY2;

typedef union {
  struct {
    uint32_t memory_size_high;
  };
  uint32_t raw;
} CXL_RANGE_SIZE_HIGH;

typedef union {
  struct {
    uint8_t memory_info_valid:1;
    uint8_t memory_active:1;
    uint8_t media_type:3;
    uint8_t memory_class:3;

    uint8_t desired_interleave:5;
    uint8_t memory_active_timeout:3;

    uint16_t memory_active_degraded:1;
    uint16_t rsvd:11;
    uint16_t memory_size_low:4;
  };
  uint32_t raw;
} CXL_RANGE_SIZE_LOW;

typedef union {
  struct {
    uint32_t memory_base_high;
  };
  uint32_t raw;
} CXL_RANGE_BASE_HIGH;

typedef union {
  struct {
    uint32_t rsvd:28;
    uint32_t memory_base_low:4;
  };
  uint32_t raw;
} CXL_RANGE_BASE_LOW;

typedef union {
  struct {
    uint16_t default_volatile_hdm_state_after_cold_reset:1;
    uint16_t default_volatile_hdm_state_after_warm_reset:1;
    uint16_t default_volatile_hdm_state_after_hot_reset:1;
    uint16_t volatile_hdm_state_after_hot_reset:1;
    uint16_t direct_p2p_mem_capable:1;
    uint16_t rsvd:11;
  };
  uint16_t raw;
} CXL_CAPABILITY3;

typedef struct {
  CXL_RANGE_SIZE_HIGH high;
  CXL_RANGE_SIZE_LOW  low;
} CXL_RANGE_SIZE;

typedef struct {
  CXL_RANGE_BASE_HIGH high;
  CXL_RANGE_BASE_LOW  low;
} CXL_RANGE_BASE;

// Figure 8-1
typedef struct {
  PCIE_CAP_ID     ide_ecap;
  CXL_DVS_HEADER1 header1;
  CXL_DVS_HEADER2 header2;
  CXL_CAPABILITY  capability;
  CXL_CONTROL     control;
  CXL_STATUS      status;
  CXL_CONTROL2    control2;
  CXL_STATUS2     status2;
  CXL_LOCK        lock;
  CXL_CAPABILITY2 capability2;

  CXL_RANGE_SIZE  range1_size;
  CXL_RANGE_BASE  range1_base;
  CXL_RANGE_SIZE  range2_size;
  CXL_RANGE_BASE  range2_base;

  CXL_CAPABILITY3 capability3;
  uint16_t        rsvd;
} CXL_DVSEC;

#pragma pack(0)

#endif