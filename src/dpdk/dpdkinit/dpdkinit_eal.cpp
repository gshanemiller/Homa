#include <dpdkinit_eal.h>
#include <cfgparse_mempool.h>
#include <cfgparse_memzone.h>
#include <cfgparse_nic.h>
#include <cfgparse_rxq.h>
#include <cfgparse_thread.h>
#include <cfgparse_txq.h>
#include <cfgparse_dpdk.h>
#include <fmt/core.h>                                                           

//                                                                                                                      
// Tell GCC to not enforce '-Wpendantic' for DPDK headers                                                               
//                                                                                                                      
#pragma GCC diagnostic push                                                                                             
#pragma GCC diagnostic ignored "-Wpedantic"                                                                             
#include <rte_flow.h>                                                                                                   
#include <rte_bus.h>                                                                                                    
#include <rte_bus_pci.h>                                                                                                
#pragma GCC diagnostic pop 

namespace HOMA {
namespace Network {

int32_t dpdkinit::Eal::basicValidateConfig() {
  int32_t rc = cfgparse::NIC::Validate(d_config);
  rc += cfgparse::Memzone::Validate(d_config);
  rc += cfgparse::Mempool::Validate(d_config);
  rc += cfgparse::RXQ::Validate(d_config);
  rc += cfgparse::TXQ::Validate(d_config);
  rc += cfgparse::Thread::Validate(d_config);
  return rc;
}

int32_t dpdkinit::Eal::rteEalInit() {
  assert(!d_rte_eal_init_done);

  int32_t rc = 0;
  std::string args;
  u_int32_t memChannels = 0;

  // Get DPDK Eal init arguments and memory channel count from config
  if (0!=(rc=cfgparse::Dpdk::Argument(d_config, &args))) {
    return 1;
  } 
  if (0!=(rc=cfgparse::Dpdk::MemoryChannelCount(d_config, &memChannels))) {
    return 1;
  } 

  // Break 'args' into individual switches if any. We need count
  char *context, *token;
  std::vector<const char *> dpdkArgs;
  const char commaDelimiter[]=",";
  for (token = strtok_r(const_cast<char*>(args.c_str()), ",", &context); token; 
    token = strtok_r(0, commaDelimiter, &context)) {
    dpdkArgs.push_back(token);                                                                                          
  }

  // Append Memory channel argument
  std::string memArg = fmt::format(" -n {}", memChannels);
  dpdkArgs.push_back(memArg.c_str());

  // Call DPDK init method
  char **argvc = const_cast<char **>(dpdkArgs.data());
  if ((rc = rte_eal_init(static_cast<int32_t>(dpdkArgs.size()), argvc))<0) {
    return 1;
  }

  d_rte_eal_init_done = true;
  return 0;
}

int32_t dpdkinit::Eal::startOneThread(const std::string& name) {
  int32_t rc = 0;
  u_int32_t index;
  if (0!=(rc=cfgparse::Thread::Find(d_config, name, &index))) {
    return rc;
  }
  if (!d_rte_eal_init_done) {
    if (0!=(rc=rteEalInit())) {
      return rc;
    }
  }
    
  return rc;
}

int32_t dpdkinit::Eal::start(const std::string& name) {
  int32_t rc = 0;
  if (d_status!=CREATED || d_status!=START_SUCCESS) {
    return 1;
  }
  
  // Make a basic json config check
  if (0!=(rc=basicValidateConfig())) {
    d_status = UNDEFINED;
    return rc;
  }

  // Find the number of thread defns in config
  u_int32_t max;
  if (0!=(rc=cfgparse::Thread::Count(d_config, &max))) {
    d_status = UNDEFINED;
    return rc;
  }

  // Start all threads if name empty else named thread
  bool found = false;
  std::string threadName;
  for (u_int32_t i=0; i<max && rc==0; ++i) {
    if (0!=(rc=cfgparse::ThreadNode::Name(i, d_config, &threadName))) {
      d_status = UNDEFINED;
      return rc;
    }
    if (name.empty() || name==threadName) {
      found = true;
      if (0!=(rc=startOneThread(threadName))) {
        d_status = UNDEFINED;
        return rc;
      }
    }
  }

  // If didn't find anything to start return error leaving d_status unchanged
  if (!found) {
    rc = 1;
  } else {
    d_status = (rc==0) ? START_SUCCESS : UNDEFINED;
  }
  return rc;
}

int32_t dpdkinit::Eal::stop() {
  if (d_status!=START_SUCCESS) {
    return 1;
  }
  return 0;
}

int32_t dpdkinit::Eal::shutdown() {
  if (d_status!=STOP_SUCCESS) {
    return 1;
  }
  return 0;
}

} // Network
} // HOMA
