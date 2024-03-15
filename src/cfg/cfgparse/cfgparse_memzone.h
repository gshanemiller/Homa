#include <nlohmann/json.hpp>
#include <string>
#include <sys/types.h>

namespace HOMA {
namespace Network {
namespace cfgparse {

struct MemzoneNode {
  static int32_t Name(const u_int32_t index, nlohmann::json& json, std::string *value);
  static int32_t SizeBytes(const u_int32_t index, nlohmann::json& json, u_int64_t *value);
  static int32_t Mask(const u_int32_t index, nlohmann::json& json, u_int64_t *value);
  static int32_t NumaNode(const u_int32_t index, nlohmann::json& json, u_int32_t *value);
};

struct Memzone {
  static int32_t Count(nlohmann::json& json, u_int32_t *value);
  static int32_t Find(nlohmann::json& json, const std::string& name, u_int32_t *index);
  static int32_t Validate(nlohmann::json& json);
};

} // cfgparse
} // Network
} // HOMA
