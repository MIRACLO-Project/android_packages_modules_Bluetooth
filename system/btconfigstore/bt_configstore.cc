/*
 * SPDX-FileCopyrightText: MiraCLO
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Bluetooth config store for devices without Qualcomm's btconfigstore HAL
 * in the system image.
 *
 * Qualcomm's btconfigstore interface definitions are not part of the open
 * CLO sources, so the system image cannot carry a client for that HAL. This
 * implementation keeps the bt_configstore.h API and reports that no config
 * store is present; the stack then uses its defaults, exactly as it does on
 * devices whose vendor has no btconfigstore HAL. The two helper conversions
 * work as usual.
 */

#define LOG_TAG "bt_configstore"

#include "bt_configstore.h"

#include <string.h>
#include <strings.h>

#include <vector>

namespace {

bool get_vendor_properties(uint32_t /* propType */,
                           std::vector<vendor_property_t>& /* vPropList */) {
  return false;
}

bool get_controller_add_on_features(controller_add_on_features_list_t* /* features_list */) {
  return false;
}

bool get_host_add_on_features(host_add_on_features_list_t* /* features_list */) {
  return false;
}

bool set_vendor_property(uint32_t /* type */, const char* /* value */) { return false; }

struct SocName {
  const char* name;
  bt_soc_type_t type;
};

const SocName kSocNames[] = {
    {"smd", BT_SOC_TYPE_SMD},
    {"ar3k", BT_SOC_TYPE_AR3K},
    {"rome", BT_SOC_TYPE_ROME},
    {"cherokee", BT_SOC_TYPE_CHEROKEE},
    {"hastings", BT_SOC_TYPE_HASTINGS},
    {"moselle", BT_SOC_TYPE_MOSELLE},
    {"hamilton", BT_SOC_TYPE_HAMILTON},
    {"ganges", BT_SOC_TYPE_GANGES},
    {"orne", BT_SOC_TYPE_ORNE},
    {"evros", BT_SOC_TYPE_EVROS},
    {"cologne", BT_SOC_TYPE_COLOGNE},
    {"congo", BT_SOC_TYPE_CONGO},
    {"europa", BT_SOC_TYPE_EUROPA},
};

bt_soc_type_t convert_bt_soc_name_to_soc_type(const char* soc_name) {
  if (soc_name == nullptr) return BT_SOC_TYPE_DEFAULT;
  for (const SocName& soc : kSocNames) {
    if (strcasecmp(soc_name, soc.name) == 0) return soc.type;
  }
  return BT_SOC_TYPE_DEFAULT;
}

const char* convert_prop_type_to_string_format(uint32_t propType) {
  switch (propType) {
    case BT_PROP_ALL:
      return "BT_PROP_ALL";
    case BT_PROP_SOC_TYPE:
      return "BT_PROP_SOC_TYPE";
    case BT_PROP_A2DP_OFFLOAD_CAP:
      return "BT_PROP_A2DP_OFFLOAD_CAP";
    case BT_PROP_SPILT_A2DP:
      return "BT_PROP_SPILT_A2DP";
    case BT_PROP_AAC_FRAME_CTL:
      return "BT_PROP_AAC_FRAME_CTL";
    case BT_PROP_WIPOWER:
      return "BT_PROP_WIPOWER";
    case BT_PROP_SWB_ENABLE:
      return "BT_PROP_SWB_ENABLE";
    case BT_PROP_SWBPM_ENABLE:
      return "BT_PROP_SWBPM_ENABLE";
    case BT_PROP_A2DP_MCAST_TEST:
      return "BT_PROP_A2DP_MCAST_TEST";
    case BT_PROP_TWSP_STATE:
      return "BT_PROP_TWSP_STATE";
    case BT_PROP_STACK_TIMEOUT:
      return "BT_PROP_STACK_TIMEOUT";
    case BT_PROP_MAX_POWER:
      return "BT_PROP_MAX_POWER";
    default:
      return "UNKNOWN_PROPERTY";
  }
}

bt_configstore_interface_t sInterface = {
    sizeof(bt_configstore_interface_t),
    get_vendor_properties,
    get_controller_add_on_features,
    get_host_add_on_features,
    set_vendor_property,
    convert_bt_soc_name_to_soc_type,
    convert_prop_type_to_string_format,
};

}  // namespace

bt_configstore_interface_t* get_btConfigStore_interface() { return &sInterface; }
