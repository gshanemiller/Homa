#include <dpdkinit_eal.h>
#include <cfgparse_mempool.h>
#include <cfgparse_memzone.h>
#include <cfgparse_nic.h>
#include <cfgparse_rxq.h>
#include <cfgparse_thread.h>
#include <cfgparse_txq.h>
#include <cfgparse_dpdk.h>
#include <cfgparse_config.h>
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

  // Prepare lcores argument
  // Append lcores argument

  // Call DPDK init method
  char **argvc = const_cast<char **>(dpdkArgs.data());
  if ((rc = rte_eal_init(static_cast<int32_t>(dpdkArgs.size()), argvc))<0) {
    return 1;
  }

  d_rte_eal_init_done = true;
  return 0;
}

int32_t dpdkinit::Eal::initializeNic(const std::string& name) {
  assert(d_nicMap.find(name)==d_nicMap.end());
  int32_t rc=0;

  // Collect some NIC attributes
  std::string pciDeviceId;
  u_int32_t index, deviceId, rxqCount, txqCount;

  rc += cfgparse::NIC::Find(d_config, name, &index);
  rc += cfgparse::NICNode::DeviceId(index, d_config, &deviceId);
  rc += cfgparse::NICNode::PciDeviceId(index, d_config, &pciDeviceId);
  rc += cfgparse::NICNode::RequestedRXQCount(index, d_config, &rxqCount);
  rc += cfgparse::NICNode::RequestedTXQCount(index, d_config, &txqCount);
  if (rc) {
    return rc;
  }

  // Make sure config deviceId valid
  if (deviceId>=rte_eth_dev_count_avail()) {
    return 1;
  }

  // Get NIC capabilities according to DPDK
  rte_eth_dev_info ethDeviceInfo;
  if (0!=(rc=rte_eth_dev_info_get(deviceId, &ethDeviceInfo))) {
    return rc;
  }

  // Make sure not asking for more queues than NIC can support
  rc += rxqCount>ethDeviceInfo.max_rx_queues;
  rc += txqCount>ethDeviceInfo.max_tx_queues;
  if (rc) {
    return 1;
  }

  // See if the DPDK device has the same PCI ID requested
  bool ok = false;
  const struct rte_bus *bus = rte_bus_find_by_device(ethDeviceInfo.device);
  if (bus && !strcmp(rte_bus_name(bus), "pci")) {
    uint16_t foundPortId;
    if (0==rte_eth_dev_get_port_by_name(pciDeviceId.c_str(), &foundPortId) && foundPortId==deviceId) {
      ok=true;
    }
  }
  if (!ok) {
    return 1;
  }

  // Collect rest of NIC attributes
  u_int64_t linkSpeed;
  u_int32_t mtuSizeBytes;
  u_int64_t rxMqMode, rxqOffloadMask;
  u_int64_t txMqMode, txqOffloadMask;
  rc += cfgparse::NICNode::MTUSizeBytes(index, d_config, &mtuSizeBytes);
  rc += cfgparse::NICNode::LinkSpeed(index, d_config, &linkSpeed);
  rc += cfgparse::NICNode::RXMQMode(index, d_config, &rxMqMode);
  rc += cfgparse::NICNode::RXQOffloadMask(index, d_config, &rxqOffloadMask);
  rc += cfgparse::NICNode::TXMQMode(index, d_config, &txMqMode);
  rc += cfgparse::NICNode::TXQOffloadMask(index, d_config, &txqOffloadMask);
  if (rc) {
    return rc;
  }

  //                                                                                                                    
  // Initalize device with a configuration.                                                                             
  // Advance RX/TX configs not yet supported                                                                            
  //                                                                                                                    
  rte_eth_conf deviceConfig;
  memset(&deviceConfig, 0, sizeof(rte_eth_conf));                                                                        
  deviceConfig.link_speeds       = linkSpeed;
  deviceConfig.rxmode.mtu        = mtuSizeBytes;
  deviceConfig.rxmode.mq_mode    = static_cast<rte_eth_rx_mq_mode>(rxMqMode);
  deviceConfig.rxmode.offloads   = rxqOffloadMask;
  deviceConfig.txmode.mq_mode    = static_cast<rte_eth_tx_mq_mode>(txMqMode);
  deviceConfig.txmode.offloads   = txqOffloadMask;
                                                                                                                        
  // Enable fast-free if available
  if (ethDeviceInfo.tx_offload_capa & RTE_ETH_TX_OFFLOAD_MBUF_FAST_FREE) {                                             
    deviceConfig.txmode.offloads |= RTE_ETH_TX_OFFLOAD_MBUF_FAST_FREE;                                                 
  }

  // Initialize NIC
  if (0!=(rc=rte_eth_dev_configure(deviceId, rxqCount, txqCount, &deviceConfig))) {
    return rc;
  }

  // Copy-n-paste from example code: Not totally sure what this does
  u_int16_t adjustedRxq, adjustedTxq;                                                                                    
  if (0!=(rc=rte_eth_dev_adjust_nb_rx_tx_desc(deviceId, &adjustedRxq, &adjustedTxq))) {
    return rc;
  } 

  // Cache config so don't do it again
  if (0==rc) {
    d_nicMap[name] = deviceConfig;
  }

  return rc;
}                                                                      

int32_t dpdkinit::Eal::createRXQ(const u_int32_t threadIndex, const u_int32_t rxqIndex) {
  int32_t rc=0;

  // Collect some RXQ attributes
  std::string mode, refNicName, refQueueName;
  rc += cfgparse::RXQRefNode::Mode(threadIndex, rxqIndex, d_config, &mode);
  rc += cfgparse::RXQRefNode::RefNicName(threadIndex, rxqIndex, d_config, &refNicName);
  rc += cfgparse::RXQRefNode::RefQueueName(threadIndex, rxqIndex, d_config, &refQueueName);
  if (rc) {
    return 1;
  }

  // See if we've initialized the NIC the RXQ refers
  auto nicIter = d_nicMap.find(refNicName);
  if (nicIter==d_nicMap.end()) {
    if (0!=(rc=initializeNic(refNicName))) {
      return rc;
    }
  }

  return 0;
}

int32_t dpdkinit::Eal::createTXQ(const u_int32_t threadIndex, const u_int32_t txqIndex) {
  return 0;
}

int32_t dpdkinit::Eal::startThread(const u_int32_t threadIndex) {
  int32_t rc = 0;
  if (!d_rte_eal_init_done) {
    if (0!=(rc=rteEalInit())) {
      return rc;
    }
  }

  // Get the number of RXQs for this thread and start them
  u_int32_t queueMax;
  if (0!=(rc=cfgparse::ThreadNode::RXQRefCount(threadIndex, d_config, &queueMax))) {
    return rc;
  }
  for (u_int32_t i=0; i<queueMax; ++i) {
    if (0!=(rc=createRXQ(threadIndex, i))) {
      return rc;
    }
  }
 
  // Get the number of TXQs for this thread and start them
  if (0!=(rc=cfgparse::ThreadNode::TXQRefCount(threadIndex, d_config, &queueMax))) {
    return rc;
  }
  for (u_int32_t i=0; i<queueMax; ++i) {
    if (0!=(rc=createTXQ(threadIndex, i))) {
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
  
  // Basic json config check
  if (!d_configValidated) {
    rc += cfgparse::Config::Validate(d_config);
    rc += cfgparse::Config::CalculatePerNicQueueCounts(d_config);
    if (rc) {
      d_status = UNDEFINED;
      return rc;
    }
    d_configValidated = true;
  }

  // Find the number of thread defns in config
  u_int32_t max;
  if (0!=(rc=cfgparse::Thread::Count(d_config, &max))) {
    d_status = UNDEFINED;
    return rc;
  }

  // Start all threads if 'name' empty else thread with 'name'
  bool found = false;
  std::string threadName;
  for (u_int32_t i=0; i<max && rc==0; ++i) {
    if (0!=(rc=cfgparse::ThreadNode::Name(i, d_config, &threadName))) {
      d_status = UNDEFINED;
      return rc;
    }
    if (name.empty() || name==threadName) {
      found = true;
      if (0!=(rc=startThread(i))) {
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
