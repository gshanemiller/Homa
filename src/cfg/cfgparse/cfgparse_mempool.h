#include <nlohmann/json.hpp>
#include <string>
#include <sys/types.h>

namespace HOMA {
namespace Network {
namespace cfgparse {

struct MempoolNode {
  static int32_t Name(const u_int32_t index, nlohmann::json& json, std::string *value);
  static int32_t CacheSizeBytes(const u_int32_t index, nlohmann::json& json, u_int64_t *value);
  static int32_t PrivateSizeBytes(const u_int32_t index, nlohmann::json& json, u_int64_t *value);
  static int32_t DataRoomSizeBytes(const u_int32_t index, nlohmann::json& json, u_int64_t *value);
  static int32_t MbufCount(const u_int32_t index, nlohmann::json& json, u_int64_t *value);
  static int32_t MemzoneName(const u_int32_t index, nlohmann::json& json, std::string *value);
};

struct Mempool {
  static int32_t Count(nlohmann::json& json, u_int32_t *value);
  static int32_t Find(nlohmann::json& json, const std::string& name, u_int32_t *index);
  static int32_t Validate(nlohmann::json& json);
};

} // cfgparse
} // Network
} // HOMA
