#include <nlohmann/json.hpp>
#include <map>
#include <vector>
#include <string>

//                                                                                                                      
// Tell GCC to not enforce '-Wpendantic' for DPDK headers                                                               
//                                                                                                                      
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <rte_ethdev.h>
#include <rte_memzone.h>
#pragma GCC diagnostic pop

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

  // TYPEDEF
  typedef std::map<std::string, rte_eth_conf> NicMap;
  typedef std::map<std::string, rte_memzone*> MemzoneMap;

  // PRIVATE DATA
  nlohmann::json&     d_config;
  int32_t             d_status;
  bool                d_configValidated;
  bool                d_rte_eal_init_done;
  NicMap              d_nicMap;
  MemzoneMap          d_memzoneMap;

  // PRIVATE MANIPULATORS
  int32_t rteEalInit();
  int32_t createPerQueueMempool(const std::string& mempoolName, const std::string& memzoneName, rte_mempool **pool);
  int32_t createMemzone(const std::string& name);
  int32_t initializeNic(const std::string& name);
  int32_t createRXQ(const u_int32_t threadIndex, const u_int32_t rxqIndex);
  int32_t createTXQ(const u_int32_t threadIndex, const u_int32_t txqIndex);
  int32_t startThread(const u_int32_t threadIndex);

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
, d_configValidated(false)
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

} // dpdkinit
} // Network
} // HOMA
