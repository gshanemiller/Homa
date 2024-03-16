#include <nlohmann/json.hpp>
#include <vector>
#include <string>

namespace HOMA {
namespace Network {
namespace dpdkinit {

class Eal {
  // ENUM
  enum {
    CREATED           = 0,
    START_SUCCESS     = 1,
    STOP_SUCCESS      = 2,
    SHUTDOWN_SUCCESS  = 3,
    UNDEFINED         = 4
  };

  // PRIVATE DATA
  nlohmann::json&     d_config;
  int32_t             d_status;
  bool                d_rte_eal_init_done;

  // PRIVATE MANIPULATORS
  int32_t basicValidateConfig();
  int32_t rteEalInit();
  int32_t startOneThread(const std::string& threadName);

public:
  // CREATORS
  explicit Eal(nlohmann::json& config);
  ~Eal();

  // PUBLIC MANIPULATORS
  int32_t start(const std::string& name);
  int32_t stop();
  int32_t shutdown();
};

// INLINE-DEFINITIONS
// CREATORS
Eal::Eal(nlohmann::json& config)
: d_config(config)
, d_status(CREATED)
, d_rte_eal_init_done(false)
{
}

Eal::~Eal() {
  if (d_status==START_SUCCESS) {
    stop();
  }
  if (d_status==STOP_SUCCESS) {
    shutdown();
  }
}

} // cfgutil
} // Network
} // HOMA
