#include <cfgparse_dpdk.h>
#include <cfgutil_validate.h>
#include <assert.h>

namespace HOMA {
namespace Network {

int32_t cfgparse::Dpdk::Argument(nlohmann::json& json, std::string *value) {
  assert(value);
  value->clear();
  try {
    *value = json.at("Dpdk").at("Initialization").at("Argument");
  } catch (const nlohmann::json::exception& e) {
    // Default empty string
    json["Dpdk"]["Initialization"]["Argument"] = *value;
  }
  return 0;
}

int32_t cfgparse::Dpdk::MemoryChannelCount(nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 1;
  try {
    *value = json.at("Dpdk").at("MemoryChannelCount");
  } catch (const nlohmann::json::exception& e) {
    // Default 1
    json["Dpdk"]["MemoryChannelCount"] = *value; 
  }
  return cfgutil::Validate::InUnsignedRange(1, 64, *value) ? 0 : 1;
}

} // Network
} // HOMA
