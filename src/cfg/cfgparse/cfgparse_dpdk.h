#include <nlohmann/json.hpp>
#include <string>
#include <sys/types.h>

namespace HOMA {
namespace Network {
namespace cfgparse {

struct Dpdk {
  static int32_t Argument(nlohmann::json& json, std::string *value);
  static int32_t MemoryChannelCount(nlohmann::json& json, u_int32_t *value);
};

} // cfgparse
} // Network
} // HOMA
