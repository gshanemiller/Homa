#include <cfgparse_memzone.h>
#include <cfgutil_validate.h>
#include <assert.h>
#include <vector>

namespace HOMA {
namespace Network {

int32_t cfgparse::MemzoneNode::Name(const u_int32_t index, nlohmann::json& json, std::string *value) {
  assert(value);
  value->clear();
  try {
    auto array = json.at("Memzone");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("Name");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::IsNonEmptyString(*value) ? 0 : 1;
}

int32_t cfgparse::MemzoneNode::SizeBytes(const u_int32_t index, nlohmann::json& json, u_int64_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("Memzone");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("SizeBytes");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffffffffffffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::MemzoneNode::Mask(const u_int32_t index, nlohmann::json& json, u_int64_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("Memzone");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("Mask");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 0xffffffffffffffffUL, *value) ? 0 : 1;
}

int32_t cfgparse::MemzoneNode::NumaNode(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("Memzone");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("NumaNode");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 16, *value) ? 0 : 1;
}

int32_t cfgparse::Memzone::Count(nlohmann::json& json, u_int32_t *value) {
  assert(value);
  try {
    auto array = json.at("Memzone");
    *value = array.size();
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return 0;
}

int32_t cfgparse::Memzone::Find(nlohmann::json& json, const std::string& name, u_int32_t *index) {
  assert(index);
  *index = 0;
  try {
    auto array = json.at("Memzone");
    std::string nodeName;
    for (u_int32_t i=0; i<array.size(); ++i) {
      if (0==cfgparse::MemzoneNode::Name(i, json, &nodeName) && nodeName==name) {
        *index = i;
        return 0;
      } 
    }
  } catch (const nlohmann::json::exception& e) {
  }
  // Not found or empty array 
  return 1;
}


int32_t cfgparse::Memzone::Validate(nlohmann::json& json) {
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
    rc += cfgparse::MemzoneNode::Name(i, json, &sValue);
    name.push_back(sValue);
    rc += cfgparse::MemzoneNode::SizeBytes(i, json, &vValue);
    rc += cfgparse::MemzoneNode::Mask(i, json, &vValue);
    rc += cfgparse::MemzoneNode::NumaNode(i, json, &uValue);
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
