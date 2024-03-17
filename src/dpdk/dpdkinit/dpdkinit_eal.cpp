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
#include <rte_bus.h>                                                                                                    
#include <rte_mbuf.h>
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

int32_t dpdkinit::Eal::startNics() {
  int32_t rc = 0;
  for (auto iter = d_nicConfMap.begin(); iter!=d_nicConfMap.end() && 0==rc; ++iter) {
    // Get the NICs deviceId
    u_int32_t index, deviceId;
    rc += cfgparse::NIC::Find(d_config, iter->first, &index);
    rc += cfgparse::NICNode::DeviceId(index, d_config, &deviceId);
    // Start deviceId
    if (0==rc) {
      rc = rte_eth_dev_start(deviceId);
    }
  }
  return rc;
}

int32_t dpdkinit::Eal::createPerQueueMempool(const std::string& mempoolName, rte_mempool **pool) {
  assert(pool);
  *pool = 0;

  // Collect the memory pool, zone's attributes
  int32_t rc = 0;
  std::string memzoneName;
  u_int32_t mempoolIndex, memzoneIndex, numaNode;
  u_int64_t cacheSizeBytes, privateSizeBytes, dataRoomSizeBytes, mbufCount;

  rc += cfgparse::Mempool::Find(d_config, mempoolName, &mempoolIndex);
  rc += cfgparse::MempoolNode::MemzoneName(mempoolIndex, d_config, &memzoneName);
  rc += cfgparse::MempoolNode::CacheSizeBytes(mempoolIndex, d_config, &cacheSizeBytes);
  rc += cfgparse::MempoolNode::PrivateSizeBytes(mempoolIndex, d_config, &privateSizeBytes);
  rc += cfgparse::MempoolNode::DataRoomSizeBytes(mempoolIndex, d_config, &dataRoomSizeBytes);
  rc += cfgparse::MempoolNode::MbufCount(mempoolIndex, d_config, &mbufCount);
  rc += cfgparse::Memzone::Find(d_config, memzoneName, &memzoneIndex);
  rc += cfgparse::MemzoneNode::NumaNode(memzoneIndex, d_config, &numaNode);
  auto iter = d_memzoneMap.find(memzoneName);
  assert(iter!=d_memzoneMap.end() && iter->second);
  if (rc || 0==iter->second) {
    return 1;
  }

  // Allocate memory pool and return. Note, I believe, the memory pool comes from zone memzone
  // allocated earlier on the same numa node
  *pool = rte_pktmbuf_pool_create(mempoolName.c_str(), mbufCount, cacheSizeBytes, privateSizeBytes, dataRoomSizeBytes, numaNode);
  return (rc==0 && *pool) ? 0 : 1;
}

int32_t dpdkinit::Eal::createMemzone(const std::string& name) {
  assert(d_memzoneMap.find(name)==d_memzoneMap.end());
  int32_t rc = 0;

  // Get memzone's attributes
  u_int32_t index, numaNode;
  u_int64_t sizeBytes, mask;
  rc += cfgparse::Memzone::Find(d_config, name, &index);
  rc += cfgparse::MemzoneNode::SizeBytes(index, d_config, &sizeBytes);
  rc += cfgparse::MemzoneNode::Mask(index, d_config, &mask);
  rc += cfgparse::MemzoneNode::NumaNode(index, d_config, &numaNode);
  if (rc) {
    return 0;
  }

  // Allocate memory of specified size, numaNode, mask
  const rte_memzone *zone = rte_memzone_reserve(name.c_str(), sizeBytes, numaNode, mask);
  if (zone) {
    // Cache on success
    d_memzoneMap[name] = zone;
  } else {
    ++rc;
  }
 
  return rc;
}

int32_t dpdkinit::Eal::initializeNic(const std::string& name) {
  assert(d_nicConfMap.find(name)==d_nicConfMap.end());
  assert(d_nicInfoMap.find(name)==d_nicInfoMap.end());
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
    d_nicConfMap[name] = deviceConfig;
    d_nicInfoMap[name] = ethDeviceInfo;
  }

  return rc;
}                                                                      

int32_t dpdkinit::Eal::createRXQ(const u_int32_t threadIndex, const u_int32_t rxqIndex) {
  int32_t rc=0;

  // Get the NIC to which this RXQ refers
  std::string refNicName;
  rc += cfgparse::RXQRefNode::RefNicName(threadIndex, rxqIndex, d_config, &refNicName);
  if (rc) {
    return 1;
  }

  // See if we've initialized the NIC the RXQ refers
  auto nicInfoIter = d_nicInfoMap.find(refNicName);
  if (nicInfoIter==d_nicInfoMap.end()) {
    if (0!=(rc=initializeNic(refNicName))) {
      return rc;
    }
  }

  // Find the RXQ's memory attributes
  u_int32_t mempoolIndex, memzoneIndex, refRxqIndex;
  std::string refQueueName, mempoolName, memzoneName, mode;
  rc += cfgparse::RXQRefNode::RefQueueName(threadIndex, rxqIndex, d_config, &refQueueName);
  rc += cfgparse::RXQRefNode::Mode(threadIndex, rxqIndex, d_config, &mode);
  rc += cfgparse::RXQ::Find(d_config, refQueueName, &refRxqIndex);
  rc += cfgparse::RXQNode::MempoolName(refRxqIndex, d_config, &mempoolName);
  rc += cfgparse::Mempool::Find(d_config, mempoolName, &mempoolIndex);
  rc += cfgparse::MempoolNode::MemzoneName(mempoolIndex, d_config, &memzoneName);
  rc += cfgparse::Memzone::Find(d_config, memzoneName, &memzoneIndex);
  if (rc) {
    return rc;
  }

  // See if we've created the memzone this RXQ uses
  auto memzoneIter = d_memzoneMap.find(memzoneName);
  if (memzoneIter==d_memzoneMap.end()) {
    if (0!=(rc=createMemzone(memzoneName))) {
      return rc;
    }
  }

  // Currently only support 'Allocate': each thread queue gets its own memory
  assert(mode=="Allocate");
  rte_mempool *pool = 0;
  if (0!=(rc=createPerQueueMempool(mempoolName, &pool)) || 0==pool) {
    return rc;
  }

  // Create/Init one RXQ by taking default then modifying with config supported here
  // Copy default config to local variable
  rte_eth_rxconf rxCfg = nicInfoIter->second.default_rxconf;

  // Find RXQ configs we support
  u_int32_t pthresh, hthresh, wthresh, freeThresh, ringSize;
  rc += cfgparse::RXQNode::PThreshold(refRxqIndex, d_config, &pthresh);
  rc += cfgparse::RXQNode::HThreshold(refRxqIndex, d_config, &hthresh);
  rc += cfgparse::RXQNode::WThreshold(refRxqIndex, d_config, &wthresh);
  rc += cfgparse::RXQNode::FreeThreshold(refRxqIndex, d_config, &freeThresh);
  rc += cfgparse::RXQNode::RingSize(refRxqIndex, d_config, &ringSize);
  if (rc) {
    return 1;
  }
  rxCfg.rx_thresh.pthresh = pthresh;
  rxCfg.rx_thresh.hthresh = hthresh;
  rxCfg.rx_thresh.wthresh = wthresh;
  rxCfg.rx_free_thresh = freeThresh;

  // Get the RXQ's NIC attributes required for initialization
  u_int64_t offloads;
  std::string threadName;
  u_int32_t nicIndex, numaNode, deviceId;
  rc += cfgparse::NIC::Find(d_config, refNicName, &nicIndex);
  rc += cfgparse::NICNode::NumaNode(nicIndex, d_config, &numaNode);
  rc += cfgparse::NICNode::DeviceId(nicIndex, d_config, &deviceId);
  rc += cfgparse::NICNode::RXQOffloadMask(nicIndex, d_config, &offloads);
  rc += cfgparse::ThreadNode::Name(threadIndex, d_config, &threadName);
  if (rc) {
    return 1;
  }
  rxCfg.offloads = offloads;

  // Find the index of the queue we're initializing
  u_int32_t queueIndex = 0;
  auto nicRxqIter = d_nicInitRxqMap.find(refNicName);
  if (nicRxqIter!=d_nicInitRxqMap.end()) {
    queueIndex = nicRxqIter->second+1;
  }

  // Initialize rxq
  if (0!=(rc=rte_eth_rx_queue_setup(deviceId, queueIndex, ringSize, numaNode, &rxCfg, pool))) {
    return 1;
  }

  // Cache last RXQ initialized for NIC
  d_nicInitRxqMap[refNicName] = queueIndex;
  // Cache RXQ for thread 
  d_threadRxqMap[threadName].push_back(queueIndex);

  return rc;
}

int32_t dpdkinit::Eal::createTXQ(const u_int32_t threadIndex, const u_int32_t txqIndex) {
  int32_t rc=0;

  // Get the NIC to which this TXQ refers
  std::string refNicName;
  rc += cfgparse::TXQRefNode::RefNicName(threadIndex, txqIndex, d_config, &refNicName);
  if (rc) {
    return 1;
  }

  // See if we've initialized the NIC the RXQ refers
  auto nicInfoIter = d_nicInfoMap.find(refNicName);
  if (nicInfoIter==d_nicInfoMap.end()) {
    if (0!=(rc=initializeNic(refNicName))) {
      return rc;
    }
  }

  // Find the TXQ's memory attributes
  u_int32_t mempoolIndex, memzoneIndex, refTxqIndex;
  std::string refQueueName, mempoolName, memzoneName, mode;
  rc += cfgparse::TXQRefNode::RefQueueName(threadIndex, txqIndex, d_config, &refQueueName);
  rc += cfgparse::TXQRefNode::Mode(threadIndex, txqIndex, d_config, &mode);
  rc += cfgparse::TXQ::Find(d_config, refQueueName, &refTxqIndex);
  rc += cfgparse::TXQNode::MempoolName(refTxqIndex, d_config, &mempoolName);
  rc += cfgparse::Mempool::Find(d_config, mempoolName, &mempoolIndex);
  rc += cfgparse::MempoolNode::MemzoneName(mempoolIndex, d_config, &memzoneName);
  rc += cfgparse::Memzone::Find(d_config, memzoneName, &memzoneIndex);
  if (rc) {
    return rc;
  }

  // See if we've created the memzone this TXQ uses
  auto memzoneIter = d_memzoneMap.find(memzoneName);
  if (memzoneIter==d_memzoneMap.end()) {
    if (0!=(rc=createMemzone(memzoneName))) {
      return rc;
    }
  }

  // Currently only support 'Allocate': each thread queue gets its own memory
  assert(mode=="Allocate");
  rte_mempool *pool = 0;
  if (0!=(rc=createPerQueueMempool(mempoolName, &pool)) || 0==pool) {
    return rc;
  }

  // Create/Init one TXQ by taking default then modifying with config supported here
  // Copy default config to local variable
  rte_eth_txconf txCfg = nicInfoIter->second.default_txconf;

  // Find TXQ configs we support
  u_int32_t pthresh, hthresh, wthresh, rsThresh, freeThresh, ringSize;
  rc += cfgparse::TXQNode::PThreshold(refTxqIndex, d_config, &pthresh);
  rc += cfgparse::TXQNode::HThreshold(refTxqIndex, d_config, &hthresh);
  rc += cfgparse::TXQNode::WThreshold(refTxqIndex, d_config, &wthresh);
  rc += cfgparse::TXQNode::RSThreshold(refTxqIndex, d_config, &rsThresh);
  rc += cfgparse::TXQNode::FreeThreshold(refTxqIndex, d_config, &freeThresh);
  rc += cfgparse::TXQNode::RingSize(refTxqIndex, d_config, &ringSize);
  if (rc) {
    return 1;
  }
  txCfg.tx_thresh.pthresh = pthresh;
  txCfg.tx_thresh.hthresh = hthresh;
  txCfg.tx_thresh.wthresh = wthresh;
  txCfg.tx_rs_thresh = rsThresh;
  txCfg.tx_free_thresh = freeThresh;

  // Get the TXQ's NIC attributes required for initialization
  u_int64_t offloads;
  std::string threadName;
  u_int32_t nicIndex, numaNode, deviceId;
  rc += cfgparse::NIC::Find(d_config, refNicName, &nicIndex);
  rc += cfgparse::NICNode::NumaNode(nicIndex, d_config, &numaNode);
  rc += cfgparse::NICNode::DeviceId(nicIndex, d_config, &deviceId);
  rc += cfgparse::NICNode::TXQOffloadMask(nicIndex, d_config, &offloads);
  rc += cfgparse::ThreadNode::Name(threadIndex, d_config, &threadName);
  if (rc) {
    return 1;
  }
  txCfg.offloads = offloads;

  // Find the index of the queue we're initializing
  u_int32_t queueIndex = 0;
  auto nicTxqIter = d_nicInitTxqMap.find(refNicName);
  if (nicTxqIter!=d_nicInitTxqMap.end()) {
    queueIndex = nicTxqIter->second+1;
  }

  // Initialize txq
  if (0!=(rc=rte_eth_tx_queue_setup(deviceId, queueIndex, ringSize, numaNode, &txCfg))) {
    return 1;
  }

  // Cache last TXQ initialized for NIC
  d_nicInitTxqMap[refNicName] = queueIndex;
  // Cache TXQ for thread 
  d_threadTxqMap[threadName].push_back(queueIndex);

  return rc;
}

int32_t dpdkinit::Eal::prepareThread(const u_int32_t threadIndex) {
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

int32_t dpdkinit::Eal::start() {
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

  // Prepare all threads
  std::string threadName;
  for (u_int32_t i=0; i<max && rc==0; ++i) {
    if (0!=(rc=cfgparse::ThreadNode::Name(i, d_config, &threadName))) {
      d_status = UNDEFINED;
      return rc;
    }
    if (0!=(rc=prepareThread(i))) {
      d_status = UNDEFINED;
      return rc;
    }
  }

  // Start all the NICs
  if (0!=(rc=startNics())) {
    return rc;
  }

  // Set status and exit
  d_status = (rc==0) ? START_SUCCESS : UNDEFINED;
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
