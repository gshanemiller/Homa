#include <nlohmann/json.hpp>
#include <vector>

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

struct Thread {
  // TYPEDEF
  typedef std::vector<const rte_mempool*> Mempool;

  // DATA
  lohmann::json&            d_jsonConfig;
  rte_eth_conf             *d_rteConfig
  rte_eth_dev_info         *d_rteNicInfo;
  u_int64_t                 d_txFlowMask;
  u_int32_t                 d_deviceId;
  u_int32_t                 d_numaNode;
  u_int32_t                 d_cpu;
  std::string               d_name;
  std::vector<u_int32_t>    d_rxqIndex;
  std::vector<u_int32_t>    d_txqIndex;
  Mempool                   d_rxqMempool;
  Mempool                   d_txqMempool;

  // CREATORS
  explicit Thread(const rte_eth_conf *rteConfig, const rte_eth_dev_info *info, const lohmann::json& json);
  Thread(const Thread& other) = default;
  ~Thread() = default;

  // MANIPULATORS
  Thread& operator=(const Thread& rhs) = default;
};

// INLINE DEFINITIONS
// CREATORS
inline
Thread(lohmann::json& json)
: d_jsonConfig(json)
: d_rteConfig(0)
, d_rteNicInfo(0)
, d_txFlowMask(0)
, d_deviceId(0)
, d_numaNode(0)
, d_cpu(0) 
{
}

} // dpdkinit
} // Network
} // HOMA
