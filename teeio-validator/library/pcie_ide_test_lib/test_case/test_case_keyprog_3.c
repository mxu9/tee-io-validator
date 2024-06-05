/**
 *  Copyright Notice:
 *  Copyright 2023-2024 Intel. All rights reserved.
 *  License: BSD 3-Clause License.
 **/

#include <stdlib.h>
#include <ctype.h>

#include "assert.h"
#include "hal/base.h"
#include "hal/library/debuglib.h"

#include "hal/library/memlib.h"
#include "library/spdm_requester_lib.h"
#include "library/pci_ide_km_requester_lib.h"
#include "library/spdm_crypt_lib.h"
#include "ide_test.h"
#include "utils.h"
#include "teeio_debug.h"
#include "pcie_ide_test_lib.h"
#include "pcie_ide_test_internal.h"

bool test_keyprog_setup_common(void *test_context);
extern uint8_t m_keyprog_max_port;
void dump_key_iv(pci_ide_km_aes_256_gcm_key_buffer_t* key_buffer);

static const char* mKeyProgAssersion[] = {
    "ide_km_key_prog send receive_data",        // .0
    "sizeof(IdeKmMessage) == sizeof(KP_ACK)",   // .1
    "IdeKmMessage.ObjectID == KP_ACK",          // .2
    "IdeKmMessage.Status == Unsupported value in PortIndex",  // .3
    "IdeKmMessage.PortIndex == KEY_PROG.PortIndex", // .4
    "IdeKmMessage.StreamID == KEY_PROG.StreamID",   // .5
    "IdeKmMessage.KeySet == KEY_PROG.KeySet && IdeKmMessage.RxTx == KEY_PROG.RxTx && IdeKmMessage.SubStream == KEY_PROG.SubStream" // .6
};

// KeyProg Case 2.3
libspdm_return_t test_ide_km_key_prog_case3(const void *pci_doe_context,
                                     void *spdm_context, const uint32_t *session_id,
                                     uint8_t stream_id, uint8_t key_sub_stream, uint8_t port_index,
                                     const pci_ide_km_aes_256_gcm_key_buffer_t *key_buffer,
                                     uint8_t *kp_ack_status)
{
    libspdm_return_t status;
    test_pci_ide_km_key_prog_t request;
    pci_ide_km_kp_ack_t response;
    size_t response_size;
    size_t request_size;
    bool res = false;
    const char* case_msg = "  Assertion 2.3";

    libspdm_zero_mem (&request, sizeof(request));
    request.header.object_id = PCI_IDE_KM_OBJECT_ID_KEY_PROG;
    request.stream_id = stream_id;
    request.key_sub_stream = key_sub_stream;
    request.port_index = port_index;
    libspdm_copy_mem (&request.key_buffer, sizeof(request.key_buffer),
                      key_buffer, sizeof(pci_ide_km_aes_256_gcm_key_buffer_t));

    request_size = sizeof(request);
    response_size = sizeof(response);
    status = pci_ide_km_send_receive_data(spdm_context, session_id,
                                          &request, request_size,
                                          &response, &response_size);
    
    // Assertion.0
    TEEIO_PRINT(("         %s.0: %s(0x%x) %s.\n", case_msg, mKeyProgAssersion[0], status, !LIBSPDM_STATUS_IS_ERROR(status) ? "Pass" : "failed"));
    if (LIBSPDM_STATUS_IS_ERROR(status))
    {
        return false;
    }

    // Assertion.1
    res = response_size == sizeof(pci_ide_km_kp_ack_t);
    TEEIO_PRINT(("         %s.1: %s %s\n", case_msg, mKeyProgAssersion[1], res ? "Pass" : "Fail"));
    if(!res) {
        return false;
    }

    // Assertion.2
    res = response.header.object_id == PCI_IDE_KM_OBJECT_ID_KP_ACK;
    TEEIO_PRINT(("         %s.2: %s %s\n", case_msg, mKeyProgAssersion[2], res ? "Pass" : "Fail"));
    if(!res) {
        return false;
    }

    // Assertion.3
    res = response.status == PCI_IDE_KM_KP_ACK_STATUS_UNSUPPORTED_PORT_INDEX;
    TEEIO_PRINT(("         %s.3: %s(0x%x) %s\n", case_msg, mKeyProgAssersion[3], status, res ? "Pass" : "Fail"));
    if(!res) {
        return false;
    }

    // Assertion.4
    res = (response.port_index == request.port_index);
    TEEIO_PRINT(("         %s.3: %s %s\n", case_msg, mKeyProgAssersion[4], res ? "Pass" : "Fail"));
    if(!res) {
        return false;
    }

    // Assertion.5
    res = (response.stream_id == request.stream_id);
    TEEIO_PRINT(("         %s.2: %s %s\n", case_msg, mKeyProgAssersion[5], res ? "Pass" : "Fail"));
    if(!res) {
        return false;
    }

    // Assertion.6
    res = (response.key_sub_stream == request.key_sub_stream);
    TEEIO_PRINT(("         %s.2: %s %s\n", case_msg, mKeyProgAssersion[6], res ? "Pass" : "Fail"));
    if(!res) {
        return false;
    }
    
    *kp_ack_status = response.status;

    return true;
}

bool pcie_ide_test_keyprog_3_setup(void *test_context)
{
  ide_common_test_case_context_t *case_context = (ide_common_test_case_context_t *)test_context;
  TEEIO_ASSERT(case_context);
  TEEIO_ASSERT(case_context->signature == CASE_CONTEXT_SIGNATURE);

  ide_common_test_group_context_t *group_context = case_context->group_context;
  TEEIO_ASSERT(group_context);
  TEEIO_ASSERT(group_context->signature == GROUP_CONTEXT_SIGNATURE);
  TEEIO_ASSERT(group_context->spdm_context);
  TEEIO_ASSERT(group_context->session_id);

  bool res = test_keyprog_setup_common(test_context);
  if(res) {
    if(m_keyprog_max_port == 0xff) {
        TEEIO_DEBUG((TEEIO_DEBUG_INFO, "[idetest]         MaxPortIndex is 0xff. Skip the case."));
        case_context->action = IDE_COMMON_TEST_ACTION_SKIP;
    }
  }

  return res;
}

bool pcie_ide_test_keyprog_3_run(void *test_context)
{
  bool status;
  bool pass = true;
  ide_common_test_case_context_t *case_context = (ide_common_test_case_context_t *)test_context;
  TEEIO_ASSERT(case_context);
  TEEIO_ASSERT(case_context->signature == CASE_CONTEXT_SIGNATURE);

  ide_common_test_group_context_t *group_context = (ide_common_test_group_context_t *)case_context->group_context;
  TEEIO_ASSERT(group_context);
  TEEIO_ASSERT(group_context->signature == GROUP_CONTEXT_SIGNATURE);

  uint8_t stream_id = group_context->stream_id;
  void *doe_context = group_context->doe_context;
  void *spdm_context = group_context->spdm_context;
  uint32_t session_id = group_context->session_id;

  pci_ide_km_aes_256_gcm_key_buffer_t key_buffer;
  uint8_t kp_ack_status;
  uint8_t ks;
  uint8_t direction;
  uint8_t substream;
  uint8_t port_index = 0;

  uint8_t k_sets[] = {PCI_IDE_KM_KEY_SET_K0, PCI_IDE_KM_KEY_SET_K1};
  uint8_t directions[] = {PCI_IDE_KM_KEY_DIRECTION_RX, PCI_IDE_KM_KEY_DIRECTION_TX};
  uint8_t substreams[] = {PCI_IDE_KM_KEY_SUB_STREAM_PR, PCI_IDE_KM_KEY_SUB_STREAM_NPR, PCI_IDE_KM_KEY_SUB_STREAM_CPL};
  bool result = libspdm_get_random_number(sizeof(key_buffer.key), (void *)key_buffer.key);
  TEEIO_ASSERT(result);

  key_buffer.iv[0] = 0;
  key_buffer.iv[1] = 1;

  // KS0|RX|PR
  ks = PCIE_IDE_STREAM_KS0;
  direction = PCIE_IDE_STREAM_RX;
  substream = PCIE_IDE_SUB_STREAM_PR;
  result = libspdm_get_random_number(sizeof(key_buffer.key), (void *)key_buffer.key);
  port_index = m_keyprog_max_port + 1;
  TEEIO_ASSERT(result);
  dump_key_iv(&key_buffer);
  TEEIO_DEBUG((TEEIO_DEBUG_INFO, "[idetest]       Test KeyProg K0|RX|PR with port_index=%d(max=%d)\n", port_index, m_keyprog_max_port));
  status = test_ide_km_key_prog_case3(doe_context, spdm_context, &session_id, stream_id,
                                k_sets[ks] | directions[direction] | substreams[substream],
                                port_index, &key_buffer, &kp_ack_status);
  pass = pass & status;

  // KS0|RX|NPR
  ks = PCIE_IDE_STREAM_KS0;
  direction = PCIE_IDE_STREAM_RX;
  substream = PCIE_IDE_SUB_STREAM_NPR;
  result = libspdm_get_random_number(sizeof(key_buffer.key), (void *)key_buffer.key);
  port_index = m_keyprog_max_port + 2;
  TEEIO_ASSERT(result);
  dump_key_iv(&key_buffer);
  TEEIO_DEBUG((TEEIO_DEBUG_INFO, "[idetest]       Test KeyProg K0|RX|NPR with port_index=%d(max=%d)\n", port_index, m_keyprog_max_port));
  status = test_ide_km_key_prog_case3(doe_context, spdm_context, &session_id, stream_id,
                                k_sets[ks] | directions[direction] | substreams[substream],
                                port_index, &key_buffer, &kp_ack_status);
  pass = pass & status;

  // KS0|RX|CPL
  ks = PCIE_IDE_STREAM_KS0;
  direction = PCIE_IDE_STREAM_RX;
  substream = PCIE_IDE_SUB_STREAM_CPL;
  result = libspdm_get_random_number(sizeof(key_buffer.key), (void *)key_buffer.key);
  port_index = m_keyprog_max_port + 3;
  TEEIO_ASSERT(result);
  dump_key_iv(&key_buffer);
  TEEIO_DEBUG((TEEIO_DEBUG_INFO, "[idetest]       Test KeyProg K0|RX|CPL with port_index=%d(max=%d)\n", port_index, m_keyprog_max_port));
  status = test_ide_km_key_prog_case3(doe_context, spdm_context, &session_id, stream_id,
                                k_sets[ks] | directions[direction] | substreams[substream],
                                port_index, &key_buffer, &kp_ack_status);
  pass = pass & status;

  // KS0|TX|PR
  ks = PCIE_IDE_STREAM_KS0;
  direction = PCIE_IDE_STREAM_TX;
  substream = PCIE_IDE_SUB_STREAM_PR;
  result = libspdm_get_random_number(sizeof(key_buffer.key), (void *)key_buffer.key);
  port_index = m_keyprog_max_port + 4;
  TEEIO_ASSERT(result);
  dump_key_iv(&key_buffer);
  TEEIO_DEBUG((TEEIO_DEBUG_INFO, "[idetest]       Test KeyProg K0|TX|PR with port_index=%d(max=%d)\n", port_index, m_keyprog_max_port));
  status = test_ide_km_key_prog_case3(doe_context, spdm_context, &session_id, stream_id,
                                k_sets[ks] | directions[direction] | substreams[substream],
                                port_index, &key_buffer, &kp_ack_status);
  pass = pass & status;

  // KS0|TX|NPR
  ks = PCIE_IDE_STREAM_KS0;
  direction = PCIE_IDE_STREAM_TX;
  substream = PCIE_IDE_SUB_STREAM_NPR;
  result = libspdm_get_random_number(sizeof(key_buffer.key), (void *)key_buffer.key);
  port_index = m_keyprog_max_port + 5;
  TEEIO_ASSERT(result);
  dump_key_iv(&key_buffer);
  TEEIO_DEBUG((TEEIO_DEBUG_INFO, "[idetest]       Test KeyProg K0|TX|NPR with port_index=%d(max=%d)\n", port_index, m_keyprog_max_port));
  status = test_ide_km_key_prog_case3(doe_context, spdm_context, &session_id, stream_id,
                                k_sets[ks] | directions[direction] | substreams[substream],
                                port_index, &key_buffer, &kp_ack_status);
  pass = pass & status;

  // KS0|TX|CPL
  ks = PCIE_IDE_STREAM_KS0;
  direction = PCIE_IDE_STREAM_TX;
  substream = PCIE_IDE_SUB_STREAM_CPL;
  result = libspdm_get_random_number(sizeof(key_buffer.key), (void *)key_buffer.key);
  port_index = m_keyprog_max_port + 6;
  TEEIO_ASSERT(result);
  dump_key_iv(&key_buffer);
  TEEIO_DEBUG((TEEIO_DEBUG_INFO, "[idetest]       Test KeyProg K0|TX|CPL with port_index=%d(max=%d)\n", port_index, m_keyprog_max_port));
  status = test_ide_km_key_prog_case3(doe_context, spdm_context, &session_id, stream_id,
                                k_sets[ks] | directions[direction] | substreams[substream],
                                port_index, &key_buffer, &kp_ack_status);
  pass = pass & status;

  case_context->result = pass ? IDE_COMMON_TEST_CASE_RESULT_SUCCESS : IDE_COMMON_TEST_CASE_RESULT_FAILED;

  return true;
}

bool pcie_ide_test_keyprog_3_teardown(void *test_context)
{
  return true;
}