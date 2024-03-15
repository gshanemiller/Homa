#include <cfgparse_rxq.h>
#include <cfgparse_memzone.h>
#include <cfgparse_mempool.h>
#include <cfgutil_validate.h>
#include <assert.h>
#include <vector>

namespace HOMA {
namespace Network {

int32_t cfgparse::RXQNode::Name(const u_int32_t index, nlohmann::json& json, std::string *value) {
  assert(value);
  value->clear();
  try {
    auto array = json.at("RXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("Name");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::IsNonEmptyString(*value) ? 0 : 1;
}

int32_t cfgparse::RXQNode::MQMask(const u_int32_t index, nlohmann::json& json, u_int64_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("RXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("MQMask");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffffffffffffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::RXQNode::OffloadMask(const u_int32_t index, nlohmann::json& json, u_int64_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("RXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("OffloadMask");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffffffffffffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::RXQNode::RingSize(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("RXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("RingSize");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(1, 1024, *value) ? 0 : 1;
}

int32_t cfgparse::RXQNode::MempoolName(const u_int32_t index, nlohmann::json& json, std::string *value) {
  assert(value);
  value->clear();
  try {
    auto array = json.at("RXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("MempoolName");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::IsNonEmptyString(*value) ? 0 : 1;
}

int32_t cfgparse::RXQ::Count(nlohmann::json& json, u_int32_t *value) {
  assert(value);
  try {
    auto array = json.at("RXQ");
    *value = array.size();
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return 0;
}

int32_t cfgparse::RXQ::Find(nlohmann::json& json, const std::string& name, u_int32_t *index) {
  assert(index);
  *index = 0;
  try {
    auto array = json.at("RXQ");
    std::string nodeName;
    for (u_int32_t i=0; i<array.size(); ++i) {
      if (0==cfgparse::RXQNode::Name(i, json, &nodeName) && nodeName==name) {
        *index = i;
        return 0;
      } 
    }
  } catch (const nlohmann::json::exception& e) {
  }
  // Not found or empty array 
  return 1;
}

int32_t cfgparse::RXQ::Validate(nlohmann::json& json) {
  u_int32_t count;
  if (Count(json, &count)) {
    return 1;
  }

  int32_t rc = 0;
  u_int32_t uValue;
  u_int64_t vValue;
  std::string sValue;
  std::vector<std::string> name;

  for (u_int32_t i=0; i<count && rc==0; ++i) {
    rc += cfgparse::RXQNode::Name(i, json, &sValue);
    name.push_back(sValue);
    rc += cfgparse::RXQNode::MQMask(i, json, &vValue);
    rc += cfgparse::RXQNode::OffloadMask(i, json, &vValue);
    rc += cfgparse::RXQNode::RingSize(i, json, &uValue);
    rc += cfgparse::RXQNode::MempoolName(i, json, &sValue);
    rc += cfgparse::Mempool::Find(json, sValue, &uValue);
  }

  if (rc) {
    return rc;
  }

  assert(rc==0);

  // Make sure no dups
  for (u_int32_t outer=0; outer<name.size(); ++outer) {
    for (u_int32_t inner=outer+1; inner<name.size(); ++inner) {
      rc += name[outer]==name[inner];
    }
  }

  return rc;
}

} // Network
} // HOMA
