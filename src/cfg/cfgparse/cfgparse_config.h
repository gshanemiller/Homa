#include <nlohmann/json.hpp>
#include <string>
#include <sys/types.h>

namespace HOMA {
namespace Network {
namespace cfgparse {

struct Config {
  static int32_t Validate(nlohmann::json& json);
  static int32_t CalculatePerNicQueueCounts(nlohmann::json& json);
};

} // cfgparse
} // Network
} // HOMA
