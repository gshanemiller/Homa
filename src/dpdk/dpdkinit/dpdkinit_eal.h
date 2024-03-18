#include <nlohmann/json.hpp>
#include <map>
#include <string>
#include <dpdkinit_thread.h>

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

  // PRIVATE TYPE
  struct NicInfo {
    u_int32_t         d_rxQueueIndex;
    u_int32_t         d_txQueueIndex;
    rte_eth_conf      d_rteEthConf;
    rte_eth_dev_info  d_rteEthInfo;

    // CREATORS
    NicInfo();
  };
    
  // TYPEDEF
  typedef std::map<std::string, Thread> ThreadMap;
  typedef std::map<std::string, NicInfo> NicInfoMap;
  typedef std::map<std::string, const rte_memzone*> MemzoneMap;

  // PRIVATE DATA
  nlohmann::json&     d_config;
  int32_t             d_status;
  bool                d_configValidated;
  bool                d_rte_eal_init_done;
  ThreadMap           d_threadMap;
  NicInfoMap          d_nicInfoMap;
  MemzoneMap          d_memzoneMap;

  // PRIVATE MANIPULATORS
  int32_t rteEalInit();
  int32_t startNics();
  int32_t createPerQueueMempool(const std::string& mempoolName, const std::string& instanceName, rte_mempool **pool);
  int32_t createMemzone(const std::string& name);
  int32_t initializeNic(const std::string& name);
  int32_t createRXQ(const u_int32_t threadIndex, const u_int32_t rxqIndex);
  int32_t createTXQ(const u_int32_t threadIndex, const u_int32_t txqIndex);
  int32_t prepareThread(const u_int32_t threadIndex);
  int32_t finalizeThreadMap();

public:
  // CREATORS
  explicit Eal(nlohmann::json& config);
  Eal(const Eal& other) = delete;
  ~Eal();

  // PUBLIC MANIPULATORS
  int32_t start();
  int32_t stop();
  int32_t shutdown();
  Eal& operator=(const Eal& rhs) = delete;

  // ACCESSORS
  const 
};

// INLINE DEFINITIONS
// NICINFO CREATORS`
Eal::NicInfo::NicInfo()
: d_rxQueueIndex(0)
, d_txQueueIndex(0)
{
}

// INLINE DEFINITIONS
// EAL CREATORS
inline
Eal::Eal(nlohmann::json& config)
: d_config(config)
, d_status(CREATED)
, d_configValidated(false)
, d_rte_eal_init_done(false)
{
}

inline
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
