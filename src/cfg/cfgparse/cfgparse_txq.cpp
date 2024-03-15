#include <cfgparse_txq.h>
#include <cfgparse_memzone.h>
#include <cfgparse_mempool.h>
#include <cfgutil_validate.h>
#include <assert.h>
#include <vector>

namespace HOMA {
namespace Network {

int32_t cfgparse::TXQNode::Name(const u_int32_t index, nlohmann::json& json, std::string *value) {
  assert(value);
  value->clear();
  try {
    auto array = json.at("TXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("Name");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::IsNonEmptyString(*value) ? 0 : 1;
}

int32_t cfgparse::TXQNode::MQMask(const u_int32_t index, nlohmann::json& json, u_int64_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("TXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("MQMask");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffffffffffffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::TXQNode::OffloadMask(const u_int32_t index, nlohmann::json& json, u_int64_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("TXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("OffloadMask");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffffffffffffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::TXQNode::PThreshold(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("TXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("PThreshold");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::TXQNode::HThreshold(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("TXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("HThreshold");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::TXQNode::WThreshold(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("TXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("WThreshold");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::TXQNode::RSThreshold(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("TXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("RSThreshold");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::TXQNode::FreeThreshold(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("TXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("FreeThreshold");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::TXQNode::RingSize(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("TXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("RingSize");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(1, 1024, *value) ? 0 : 1;
}

int32_t cfgparse::TXQNode::MempoolName(const u_int32_t index, nlohmann::json& json, std::string *value) {
  assert(value);
  value->clear();
  try {
    auto array = json.at("TXQ");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("MempoolName");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::IsNonEmptyString(*value) ? 0 : 1;
}

int32_t cfgparse::TXQ::Count(nlohmann::json& json, u_int32_t *value) {
  assert(value);
  try {
    auto array = json.at("TXQ");
    *value = array.size();
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return 0;
}

int32_t cfgparse::TXQ::Find(nlohmann::json& json, const std::string& name, u_int32_t *index) {
  assert(index);
  *index = 0;
  try {
    auto array = json.at("TXQ");
    std::string nodeName;
    for (u_int32_t i=0; i<array.size(); ++i) {
      if (0==cfgparse::TXQNode::Name(i, json, &nodeName) && nodeName==name) {
        *index = i;
        return 0;
      } 
    }
  } catch (const nlohmann::json::exception& e) {
  }
  // Not found or empty array 
  return 1;
}

int32_t cfgparse::TXQ::Validate(nlohmann::json& json) {
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
    rc += cfgparse::TXQNode::Name(i, json, &sValue);
    name.push_back(sValue);
    rc += cfgparse::TXQNode::MQMask(i, json, &vValue);
    rc += cfgparse::TXQNode::OffloadMask(i, json, &vValue);
    rc += cfgparse::TXQNode::PThreshold(i, json, &uValue);
    rc += cfgparse::TXQNode::HThreshold(i, json, &uValue);
    rc += cfgparse::TXQNode::WThreshold(i, json, &uValue);
    rc += cfgparse::TXQNode::RSThreshold(i, json, &uValue);
    rc += cfgparse::TXQNode::FreeThreshold(i, json, &uValue);
    rc += cfgparse::TXQNode::RingSize(i, json, &uValue);
    rc += cfgparse::TXQNode::MempoolName(i, json, &sValue);
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
