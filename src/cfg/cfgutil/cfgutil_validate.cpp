#include <cfgutil_validate.h>

namespace HOMA {
namespace Network {
namespace cfgutil {

bool Validate::IsNonNegative(const int64_t value) {
  return value>=0;
}

bool Validate::InSignedRange(const int64_t min, const int64_t max, const int64_t value) {
  return value>=min && value<=max;
}

bool Validate::InUnsignedRange(const u_int64_t min, const u_int64_t max, const u_int64_t value) {
  return value>=min && value<=max;
}

bool Validate::IsNonEmptyString(const std::string& value) {
  return !value.empty();
}

bool Validate::IsPciDeviceId(const std::string& value) {
  return value.size()>0;
}

} // cfgutil
} // Network
} // HOMA
