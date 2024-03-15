#include <cfgparse_nic.h>
#include <cfgutil_validate.h>
#include <assert.h>
#include <vector>

namespace HOMA {
namespace Network {

int32_t cfgparse::NICNode::Name(const u_int32_t index, nlohmann::json& json, std::string *value) {
  assert(value);
  value->clear();
  try {
    auto array = json.at("NIC");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("Name");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::IsNonEmptyString(*value) ? 0 : 1;
}

int32_t cfgparse::NICNode::DeviceId(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("NIC");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("DeviceId");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 16, *value) ? 0 : 1;
}

int32_t cfgparse::NICNode::PciDeviceId(const u_int32_t index, nlohmann::json& json, std::string *value) {
  assert(value);
  value->clear();
  try {
    auto array = json.at("NIC");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("PciDeviceId");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::IsPciDeviceId(*value) ? 0 : 1;
}

int32_t cfgparse::NICNode::NumaNode(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("NIC");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("NumaNode");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 16, *value) ? 0 : 1;
}

int32_t cfgparse::NICNode::MTUSizeBytes(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("NIC");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("MTUSizeBytes");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 10240, *value) ? 0 : 1;
}

int32_t cfgparse::NICNode::LinkSpeed(const u_int32_t index, nlohmann::json& json, u_int64_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("NIC");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("LinkSpeed");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffffffffffffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::NICNode::TxFlowMask(const u_int32_t index, nlohmann::json& json, u_int64_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("NIC");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("TxFlowMask");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffffffffffffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::NIC::Count(nlohmann::json& json, u_int32_t *value) {
  assert(value);
  try {
    auto array = json.at("NIC");
    *value = array.size();
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return 0;
}

int32_t cfgparse::NIC::Find(nlohmann::json& json, const std::string& name, u_int32_t *index) {
  assert(index);
  *index = 0;
  try {
    auto array = json.at("NIC");
    std::string nodeName;
    for (u_int32_t i=0; i<array.size(); ++i) {
      if (0==cfgparse::NICNode::Name(i, json, &nodeName) && nodeName==name) {
        *index = i;
        return 0;
      } 
    }
  } catch (const nlohmann::json::exception& e) {
  }
  // Not found or empty array 
  return 1;
}

int32_t cfgparse::NIC::Validate(nlohmann::json& json) {
  u_int32_t count;
  if (Count(json, &count)) {
    return 1;
  }

  int32_t rc = 0;
  u_int32_t uValue;
  u_int64_t vValue;
  std::string sValue;
  std::vector<u_int32_t> deviceId;
  std::vector<std::string> name;
  std::vector<std::string> pciDeviceId;

  for (u_int32_t i=0; i<count && rc==0; ++i) {
    rc += cfgparse::NICNode::Name(i, json, &sValue);
    name.push_back(sValue);
    rc += cfgparse::NICNode::DeviceId(i, json, &uValue);
    deviceId.push_back(uValue);
    rc += cfgparse::NICNode::PciDeviceId(i, json, &sValue);
    pciDeviceId.push_back(sValue);
    rc += cfgparse::NICNode::NumaNode(i, json, &uValue);
    rc += cfgparse::NICNode::MTUSizeBytes(i, json, &uValue);
    rc += cfgparse::NICNode::LinkSpeed(i, json, &vValue);
    rc += cfgparse::NICNode::TxFlowMask(i, json, &vValue);
  }

  if (rc) {
    return rc;
  }

  assert(rc==0);
  assert(deviceId.size()==name.size());
  assert(name.size()==pciDeviceId.size());

  // Make sure no dups
  for (u_int32_t outer=0; outer<name.size(); ++outer) {
    for (u_int32_t inner=outer+1; inner<name.size(); ++inner) {
      rc += name[outer]==name[inner];
      rc += deviceId[outer]==deviceId[inner];
      rc += pciDeviceId[outer]==pciDeviceId[inner];
    }
  }

  return rc;
}

} // Network
} // HOMA
