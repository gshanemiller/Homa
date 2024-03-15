#include <cfgparse_thread.h>
#include <cfgparse_mempool.h>
#include <cfgparse_memzone.h>
#include <cfgparse_nic.h>
#include <cfgparse_rxq.h>
#include <cfgparse_txq.h>
#include <cfgutil_validate.h>
#include <assert.h>
#include <vector>

namespace HOMA {
namespace Network {

int32_t cfgparse::RXQRefNode::Mode(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value) {
  assert(value);
  u_int32_t max;
  value->clear();
  if (0==cfgparse::Thread::Count(json, &max) && pIndex<max) {
    if (0==cfgparse::ThreadNode::RXQRefCount(pIndex, json, &max) && qIndex<max) {
      try {
        *value = json["Thread"][pIndex]["RXQ"][qIndex].at("Mode");
        if (*value=="Allocate" || *value=="Share") {
          return 0;
        }
      } catch (const nlohmann::json::exception& e) {
      }
    }
  }
  return 1;
}

int32_t cfgparse::RXQRefNode::RefNicName(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value) {
  assert(value);
  u_int32_t max;
  value->clear();
  if (0==cfgparse::Thread::Count(json, &max) && pIndex<max) {
    if (0==cfgparse::ThreadNode::RXQRefCount(pIndex, json, &max) && qIndex<max) {
      try {
        *value = json["Thread"][pIndex]["RXQ"][qIndex].at("RefNicName");
      } catch (const nlohmann::json::exception& e) {
        return 1;
      }
    }
  }
  return cfgutil::Validate::IsNonEmptyString(*value) ? 0 : 1;
}

int32_t cfgparse::RXQRefNode::RefQueueName(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value) {
  assert(value);
  u_int32_t max;
  value->clear();
  if (0==cfgparse::Thread::Count(json, &max) && pIndex<max) {
    if (0==cfgparse::ThreadNode::RXQRefCount(pIndex, json, &max) && qIndex<max) {
      try {
        *value = json["Thread"][pIndex]["RXQ"][qIndex].at("RefQueueName");
      } catch (const nlohmann::json::exception& e) {
        return 1;
      }
    }
  }
  return cfgutil::Validate::IsNonEmptyString(*value) ? 0 : 1;
}

int32_t cfgparse::TXQRefNode::Mode(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value) {
  assert(value);
  u_int32_t max;
  value->clear();
  if (0==cfgparse::Thread::Count(json, &max) && pIndex<max) {
    if (0==cfgparse::ThreadNode::TXQRefCount(pIndex, json, &max) && qIndex<max) {
      try {
        *value = json["Thread"][pIndex]["TXQ"][qIndex].at("Mode");
        if (*value=="Allocate" || *value=="Share") {
          return 0;
        }
      } catch (const nlohmann::json::exception& e) {
      }
    }
  }
  return 1;
}

int32_t cfgparse::TXQRefNode::RefNicName(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value) {
  assert(value);
  u_int32_t max;
  value->clear();
  if (0==cfgparse::Thread::Count(json, &max) && pIndex<max) {
    if (0==cfgparse::ThreadNode::TXQRefCount(pIndex, json, &max) && qIndex<max) {
      try {
        *value = json["Thread"][pIndex]["TXQ"][qIndex].at("RefNicName");
      } catch (const nlohmann::json::exception& e) {
      }
    }
  }
  return cfgutil::Validate::IsNonEmptyString(*value) ? 0 : 1;
}

int32_t cfgparse::TXQRefNode::RefQueueName(const u_int32_t pIndex, const u_int32_t qIndex, nlohmann::json& json, std::string *value) {
  assert(value);
  u_int32_t max;
  value->clear();
  if (0==cfgparse::Thread::Count(json, &max) && pIndex<max) {
    if (0==cfgparse::ThreadNode::TXQRefCount(pIndex, json, &max) && qIndex<max) {
      try {
        *value = json["Thread"][pIndex]["TXQ"][qIndex].at("RefQueueName");
      } catch (const nlohmann::json::exception& e) {
      }
    }
  }
  return cfgutil::Validate::IsNonEmptyString(*value) ? 0 : 1;
}

int32_t cfgparse::ThreadNode::Name(const u_int32_t index, nlohmann::json& json, std::string *value) {
  assert(value);
  value->clear();
  try {
    auto array = json.at("Thread");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("Name");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::IsNonEmptyString(*value) ? 0 : 1;
}

int32_t cfgparse::ThreadNode::CPU(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("Thread");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("CPU");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 256, *value) ? 0 : 1;
}

int32_t cfgparse::ThreadNode::NumaNode(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("Thread");
    if (index>=array.size()) {
      return 1;
    }
    *value = array[index].at("NumaNode");
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return cfgutil::Validate::InUnsignedRange(0, 16, *value) ? 0 : 1;
}

int32_t cfgparse::ThreadNode::RXQRefCount(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  u_int32_t max;
  *value = 0;
  try {
    if (0==cfgparse::Thread::Count(json, &max) && index<max) {
      auto array = json["Thread"][index].at("RXQ");
      *value = array.size();
    }
  } catch (const nlohmann::json::exception& e) {
  }
  return 0;
}

int32_t cfgparse::ThreadNode::TXQRefCount(const u_int32_t index, nlohmann::json& json, u_int32_t *value) {
  assert(value);
  u_int32_t max;
  *value = 0;
  try {
    if (0==cfgparse::Thread::Count(json, &max) && index<max) {
      auto array = json["Thread"][index].at("TXQ");
      *value = array.size();
    }
  } catch (const nlohmann::json::exception& e) {
  }
  return 0;
}

int32_t cfgparse::Thread::Count(nlohmann::json& json, u_int32_t *value) {
  assert(value);
  *value = 0;
  try {
    auto array = json.at("Thread");
    *value = array.size();
  } catch (const nlohmann::json::exception& e) {
    return 1;
  }
  return 0;
}

int32_t cfgparse::Thread::Find(nlohmann::json& json, const std::string& name, u_int32_t *index) {
  assert(index);
  *index = 0;
  try {
    auto array = json.at("Thread");
    std::string nodeName;
    for (u_int32_t i=0; i<array.size(); ++i) {
      if (0==cfgparse::ThreadNode::Name(i, json, &nodeName) && nodeName==name) {
        *index = i;
        return 0;
      } 
    }
  } catch (const nlohmann::json::exception& e) {
  }
  // Not found or empty array 
  return 1;
}

int32_t cfgparse::Thread::Validate(nlohmann::json& json) {
  u_int32_t count;
  if (Count(json, &count)) {
    return 1;
  }

  int32_t rc = 0;
  int32_t rc1 = 0;
  u_int32_t max;
  u_int32_t value;
  u_int32_t numaNode;
  u_int32_t index;
  std::string str;
  std::vector<std::string> name;

  for (u_int32_t i=0; i<count && rc==0; ++i) {
    rc += cfgparse::ThreadNode::Name(i, json, &str);
    name.push_back(str);
    rc += cfgparse::ThreadNode::CPU(i, json, &value);
    rc += cfgparse::ThreadNode::NumaNode(i, json, &numaNode);

    // Make sure RXQs OK
    rc1 = cfgparse::ThreadNode::RXQRefCount(i, json, &max);
    if (rc1==0) {
      for (u_int32_t j=0; j<max && rc==0; ++j) {
        rc += cfgparse::RXQRefNode::Mode(i, j, json, &str);

        // Make sure NIC on same numa node as thread's
        rc += cfgparse::RXQRefNode::RefNicName(i, j, json, &str);
        rc += cfgparse::NIC::Find(json, str, &index);
        rc += cfgparse::NICNode::NumaNode(index, json, &value);
        rc += (value!=numaNode) ? 1 : 0;

        // Make sure queue's memory on same numa node as thread's
        rc += cfgparse::RXQRefNode::RefQueueName(i, j, json, &str);
        rc += cfgparse::RXQ::Find(json, str, &index);
        rc += cfgparse::RXQNode::MempoolName(index, json, &str);
        rc += cfgparse::Mempool::Find(json, str, &index);
        rc += cfgparse::MempoolNode::MemzoneName(index, json, &str);
        rc += cfgparse::Memzone::Find(json, str, &index);
        rc += cfgparse::MemzoneNode::NumaNode(index, json, &value);
        rc += (value!=numaNode) ? 1 : 0;
        rc += cfgparse::NICNode::NumaNode(index, json, &value);
        rc += (rc==0 && value!=numaNode) ? 1 : 0;
      }
    }

    // Make sure TXQs ref good nic, queue names, and numa aligned
    rc1 = cfgparse::ThreadNode::TXQRefCount(i, json, &max);
    if (rc1==0) {
      for (u_int32_t j=0; j<max && rc==0; ++j) {
        rc += cfgparse::RXQRefNode::Mode(i, j, json, &str);

        // Make sure NIC on same numa node as thread's
        rc += cfgparse::TXQRefNode::RefNicName(i, j, json, &str);
        rc += cfgparse::NIC::Find(json, str, &index);
        rc += cfgparse::NICNode::NumaNode(index, json, &value);
        rc += (value!=numaNode) ? 1 : 0;

        // Make sure queue's memory on same numa node as thread's
        rc += cfgparse::TXQRefNode::RefQueueName(i, j, json, &str);
        rc += cfgparse::TXQ::Find(json, str, &index);
        rc += cfgparse::TXQNode::MempoolName(index, json, &str);
        rc += cfgparse::Mempool::Find(json, str, &index);
        rc += cfgparse::MempoolNode::MemzoneName(index, json, &str);
        rc += cfgparse::Memzone::Find(json, str, &index);
        rc += cfgparse::MemzoneNode::NumaNode(index, json, &value);
        rc += (value!=numaNode) ? 1 : 0;
        rc += cfgparse::NICNode::NumaNode(index, json, &value);
        rc += (rc==0 && value!=numaNode) ? 1 : 0;
      }
    }
  }

  return rc;
}

} // Network
} // HOMA
