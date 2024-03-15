#include <nlohmann/json.hpp>
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
  static int32_t TxFlowMask(const u_int32_t index, nlohmann::json& json, u_int64_t *value);
};

struct NIC {
  static int32_t Count(nlohmann::json& json, u_int32_t *value);
  static int32_t Find(nlohmann::json& json, const std::string& name, u_int32_t *index);
  static int32_t Validate(nlohmann::json& json);
};

} // cfgparse
} // Network
} // HOMA
