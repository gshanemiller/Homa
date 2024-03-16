#include <nlohmann/json.hpp>
#include <map>
#include <string>
#include <sys/types.h>

namespace HOMA {
namespace Network {
namespace cfgparse {

struct NICNode {
  static int32_t Name(const u_int32_t index, nlohmann::json& json, std::string *value);
  static int32_t DeviceId(const u_int32_t index, nlohmann::json& json, u_int32_t *value);
  static int32_t PciDeviceId(const u_int32_t index, nlohmann::json& json, std::string *value);
  static int32_t NumaNode(const u_int32_t index, nlohmann::json& json, u_int32_t *value);
  static int32_t MTUSizeBytes(const u_int32_t index, nlohmann::json& json, u_int32_t *value);
  static int32_t LinkSpeed(const u_int32_t index, nlohmann::json& json, u_int64_t *value);
  static int32_t RXMQMode(const u_int32_t index, nlohmann::json& json, u_int64_t *value);                               
  static int32_t RXQOffloadMask(const u_int32_t index, nlohmann::json& json, u_int64_t *value);                         
  static int32_t TXMQMode(const u_int32_t index, nlohmann::json& json, u_int64_t *value);                               
  static int32_t TXQOffloadMask(const u_int32_t index, nlohmann::json& json, u_int64_t *value); 
  static int32_t RequestedRXQCount(const u_int32_t index, nlohmann::json& json, u_int32_t *value);
  static int32_t RequestedTXQCount(const u_int32_t index, nlohmann::json& json, u_int32_t *value);
};

struct NIC {
  static int32_t Count(nlohmann::json& json, u_int32_t *value);
  static int32_t Find(nlohmann::json& json, const std::string& name, u_int32_t *index);
  static int32_t Validate(nlohmann::json& json);
  static int32_t SetRXQueueCount(nlohmann::json& json, const std::map<std::string, u_int32_t> count);
  static int32_t SetTXQueueCount(nlohmann::json& json, const std::map<std::string, u_int32_t> count);
};

} // cfgparse
} // Network
} // HOMA
