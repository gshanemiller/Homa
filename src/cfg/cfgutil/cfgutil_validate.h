#include <string>
#include <sys/types.h>

namespace HOMA {
namespace Network {
namespace cfgutil {

struct Validate {
  static bool IsNonNegative(const int64_t value);
  static bool InSignedRange(const int64_t min, const int64_t max, const int64_t value);
  static bool InUnsignedRange(const u_int64_t min, const u_int64_t max, const u_int64_t value);
  static bool IsNonEmptyString(const std::string& value);
  static bool IsPciDeviceId(const std::string& value);
};

} // cfgutil
} // Network
} // HOMA
