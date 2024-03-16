#include <cfgparse_config.h>
#include <cfgparse_thread.h>                                                                                            
#include <cfgparse_mempool.h>                                                                                           
#include <cfgparse_memzone.h>                                                                                           
#include <cfgparse_nic.h>                                                                                               
#include <cfgparse_rxq.h>                                                                                               
#include <cfgparse_txq.h>                                                                                               
#include <cfgutil_validate.h>  
#include <map>

namespace HOMA {
namespace Network {

int32_t cfgparse::Config::Validate(nlohmann::json& json) {
  int32_t rc = 0;
  rc += cfgparse::NIC::Validate(json);                                                                       
  rc += cfgparse::Memzone::Validate(json);                                                                          
  rc += cfgparse::Mempool::Validate(json);                                                                          
  rc += cfgparse::RXQ::Validate(json);                                                                              
  rc += cfgparse::TXQ::Validate(json);                                                                              
  rc += cfgparse::Thread::Validate(json);                                                                           
  return rc;
}

int32_t cfgparse::Config::CalculatePerNicQueueCounts(nlohmann::json& json) {
  // Find the number of RXQs, TXQs by NIC required from all threads
  int32_t rc = 0, rc1;
  std::string name;
  u_int32_t threadMax, queueMax;
  std::map<std::string, u_int32_t> rxqCountByNic;
  std::map<std::string, u_int32_t> txqCountByNic;

  // Find number of threads
  if (cfgparse::Thread::Count(json, &threadMax)) {
    return 1;
  }

  // Visit each thread
  for (u_int32_t i=0; i<threadMax && rc==0; ++i) {
    // Check thread's RXQs if any
    rc1 = cfgparse::ThreadNode::RXQRefCount(i, json, &queueMax);
    if (rc1==0) {
      for (u_int32_t j=0; j<queueMax && rc==0; ++j) {
        // Get the NIC for the RXQ
        rc += cfgparse::RXQRefNode::RefNicName(i, j, json, &name);
        auto iter = rxqCountByNic.find(name);
        if (iter==rxqCountByNic.end()) {
          rxqCountByNic[name]=1;
        } else {
          rxqCountByNic[name]=rxqCountByNic[name]+1;
        }
      }
    }

    // Check thread's TXQs if any
    rc1 = cfgparse::ThreadNode::TXQRefCount(i, json, &queueMax);
    if (rc1==0) {
      for (u_int32_t j=0; j<queueMax && rc==0; ++j) {
        // Get the NIC for the TXQ
        rc += cfgparse::TXQRefNode::RefNicName(i, j, json, &name);
        auto iter = txqCountByNic.find(name);
        if (iter==txqCountByNic.end()) {
          txqCountByNic[name]=1;
        } else {
          txqCountByNic[name]=txqCountByNic[name]+1;
        }
      }
    }
  }

  if (0==rc) {
    rc += cfgparse::NIC::SetRXQueueCount(json, rxqCountByNic);
    rc += cfgparse::NIC::SetTXQueueCount(json, txqCountByNic);
  }

  return rc;
}

} // Network
} // HOMA
