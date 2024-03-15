#include <nlohmann/json.hpp>
#include <string>
#include <sys/types.h>

namespace HOMA {
namespace Network {
namespace cfgparse {

struct RXQRefNode {
  static int32_t Mode(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value);
  static int32_t RefNicName(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value);
  static int32_t RefQueueName(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value);
};

struct TXQRefNode {
  static int32_t Mode(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value);
  static int32_t RefNicName(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value);
  static int32_t RefQueueName(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value);
};

struct ThreadNode {
  static int32_t Name(const u_int32_t index, nlohmann::json& json, std::string *value);
  static int32_t CPU(const u_int32_t index, nlohmann::json& json, u_int32_t *value);
  static int32_t NumaNode(const u_int32_t index, nlohmann::json& json, u_int32_t *value);
  static int32_t RXQRefCount(const u_int32_t index, nlohmann::json& json, u_int32_t *value);
  static int32_t TXQRefCount(const u_int32_t index, nlohmann::json& json, u_int32_t *value);
};

struct Thread {
  static int32_t Count(nlohmann::json& json, u_int32_t *value);
  static int32_t Find(nlohmann::json& json, const std::string& name, u_int32_t *index);
  static int32_t Validate(nlohmann::json& json);
};

} // cfgparse
} // Network
} // HOMA
