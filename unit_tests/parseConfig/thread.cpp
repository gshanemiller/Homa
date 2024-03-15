#include <gtest/gtest.h>
#include <string>
#include <fmt/core.h>
#include <cfgparse_thread.h>

TEST(parseConfig, thread) {
  std::string config = R"(
  {
    "Memzone": [
      {
        "Name"                    : "DefaultMemZone",
        "SizeBytes"               : 1024,
        "Mask"                    : 65536,
        "NumaNode"                : 0
      }
    ],
    "Mempool": [
      {
        "Name"                    : "Mempool1",
        "CacheSizeBytes"          : 0,
        "PrivateSizeBytes"        : 0,
        "DataRoomSizeBytes"       : 1024,
        "MbufCount"               : 10240,
        "MemzoneName"             : "DefaultMemZone"
      }
    ],
    "RXQ": [
      {
        "Name"                    : "RXQ1",
        "MQMask"                  : 0,
        "OffloadMask"             : 0,
        "RingSize"                : 1024,
        "MempoolName"             : "Mempool1"
      }
    ],
    "Thread": [
      {
        "Name"                    : "ClientThread",
        "CPU"                     : 3,
        "NumaNode"                : 0
      }
    ]
  }
  )";

  nlohmann::json json; 

  try {
    json = nlohmann::json::parse(config);
  }
  catch (const nlohmann::json::exception& e) {
    fmt::print("json config invalid: '{}'\n{}\n", config, e.what());
    EXPECT_TRUE(0);
  }
  
  std::string sValue;
  u_int32_t   uValue;

  EXPECT_EQ(0, HOMA::Network::cfgparse::Thread::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::Thread::Validate(json));

  std::string name = "ClientThread";
  EXPECT_EQ(0, HOMA::Network::cfgparse::Thread::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "BubbaThread";
  EXPECT_NE(0, HOMA::Network::cfgparse::Thread::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "ClientThread");
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::CPU(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)3);
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::NumaNode(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::RXQRefCount(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::TXQRefCount(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)0);

  EXPECT_NE(0, HOMA::Network::cfgparse::ThreadNode::Name(1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::ThreadNode::CPU(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::ThreadNode::NumaNode(1, json, &uValue));
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::RXQRefCount(1, json, &uValue));
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::TXQRefCount(1, json, &uValue));
}

TEST(parseConfig, thread1) {
  std::string config = R"(
  {
    "NIC": [ 
      {
        "Name"                    : "DefaultNic",
        "DeviceId"                : 0,
        "PciDeviceId"             : "0000:7f:00.0",
        "NumaNode"                : 0,
        "MTUSizeBytes"            : 1500,
        "LinkSpeed"               : 0,
        "TxFlowMask"              : 2048
      }
    ],
    "Memzone": [
      {
        "Name"                    : "DefaultMemZone",
        "SizeBytes"               : 1024,
        "Mask"                    : 65536,
        "NumaNode"                : 0
      }
    ],
    "Mempool": [
      {
        "Name"                    : "Mempool1",
        "CacheSizeBytes"          : 0,
        "PrivateSizeBytes"        : 0,
        "DataRoomSizeBytes"       : 1024,
        "MbufCount"               : 10240,
        "MemzoneName"             : "DefaultMemZone"
      }
    ],
    "RXQ": [
      {
        "Name"                    : "RXQ1",
        "MQMask"                  : 0,
        "OffloadMask"             : 0,
        "RingSize"                : 1024,
        "MempoolName"             : "Mempool1"
      }
    ],
    "TXQ": [                                                                                                            
      {                                                                                                                 
        "Name"                    : "TXQ1",                                                                             
        "MQMask"                  : 1,                                                                                  
        "OffloadMask"             : 2,                                                                                  
        "PThreshold"              : 32,                                                                                 
        "HThreshold"              : 4,                                                                                  
        "WThreshold"              : 5,                                                                                  
        "RSThreshold"             : 32,                                                                                 
        "FreeThreshold"           : 33,                                                                                 
        "RingSize"                : 1024,                                                                               
        "MempoolName"             : "Mempool1"                                                                          
      }                                                                                                                 
    ],
    "Thread": [
      {
        "Name"                    : "ClientThread",
        "CPU"                     : 3,
        "NumaNode"                : 0,
        "RXQ": [
          {
            "RefQueueName"        : "RXQ1",
            "RefNicName"          : "DefaultNic",
            "Mode"                : "Allocate"
          }
        ],
        "TXQ": [
          {
            "RefQueueName"        : "TXQ1",
            "RefNicName"          : "DefaultNic",
            "Mode"                : "Allocate"
          }
        ]
      }
    ]
  }
  )";

  nlohmann::json json; 

  try {
    json = nlohmann::json::parse(config);
  }
  catch (const nlohmann::json::exception& e) {
    fmt::print("json config invalid: '{}'\n{}\n", config, e.what());
    EXPECT_TRUE(0);
  }
  
  std::string sValue;
  u_int32_t   uValue;

  EXPECT_EQ(0, HOMA::Network::cfgparse::Thread::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::Thread::Validate(json));

  std::string name = "ClientThread";
  EXPECT_EQ(0, HOMA::Network::cfgparse::Thread::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "BubbaThread";
  EXPECT_NE(0, HOMA::Network::cfgparse::Thread::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "ClientThread");
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::CPU(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)3);
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::NumaNode(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::RXQRefCount(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)1);
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::TXQRefCount(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)1);

  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQRefNode::Mode(0, 0, json, &sValue));
  EXPECT_EQ(sValue, "Allocate");
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQRefNode::RefNicName(0, 0, json, &sValue));
  EXPECT_EQ(sValue, "DefaultNic");
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQRefNode::RefQueueName(0, 0, json, &sValue));
  EXPECT_EQ(sValue, "RXQ1");

  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQRefNode::Mode(0, 0, json, &sValue));
  EXPECT_EQ(sValue, "Allocate");
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQRefNode::RefNicName(0, 0, json, &sValue));
  EXPECT_EQ(sValue, "DefaultNic");
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQRefNode::RefQueueName(0, 0, json, &sValue));
  EXPECT_EQ(sValue, "TXQ1");

  EXPECT_NE(0, HOMA::Network::cfgparse::RXQRefNode::Mode(0, 1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQRefNode::RefNicName(0, 1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQRefNode::RefQueueName(0, 1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQRefNode::Mode(1, 0, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQRefNode::RefNicName(1, 0, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQRefNode::RefQueueName(1, 0, json, &sValue));

  EXPECT_NE(0, HOMA::Network::cfgparse::TXQRefNode::Mode(0, 1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQRefNode::RefNicName(0, 1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQRefNode::RefQueueName(0, 1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQRefNode::Mode(1, 0, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQRefNode::RefNicName(1, 0, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQRefNode::RefQueueName(1, 0, json, &sValue));

  EXPECT_NE(0, HOMA::Network::cfgparse::ThreadNode::Name(1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::ThreadNode::CPU(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::ThreadNode::NumaNode(1, json, &uValue));
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::RXQRefCount(1, json, &uValue));
  EXPECT_EQ(0, HOMA::Network::cfgparse::ThreadNode::TXQRefCount(1, json, &uValue));
}
