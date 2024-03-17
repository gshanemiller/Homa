#include <gtest/gtest.h>
#include <string>
#include <fmt/core.h>
#include <cfgparse_txq.h>

TEST(parseConfig, txq) {
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
  u_int64_t   vValue;

  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQ::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQ::Validate(json));

  std::string name = "TXQ1";
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQ::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "BubbaTXQ";
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQ::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "TXQ1");
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::MQMask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)1);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::OffloadMask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)2);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::PThreshold(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)32);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::HThreshold(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)4);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::WThreshold(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)5);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::RSThreshold(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)32);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::FreeThreshold(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)33);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::RingSize(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int64_t)1024);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::MempoolName(0, json, &sValue));
  EXPECT_EQ(sValue, "Mempool1");

  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::Name(1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::MQMask(1, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::OffloadMask(1, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::PThreshold(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::HThreshold(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::WThreshold(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::RSThreshold(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::FreeThreshold(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::RingSize(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::MempoolName(1, json, &sValue));
}

TEST(parseConfig, txq1) {
  std::string config = R"(
  {
    "Memzone": [
      {
        "Name"                    : "DefaultMemZone",
        "SizeBytes"               : 1024,
        "Mask"                    : 65536,
        "NumaNode"                : 0
      },
      {
        "Name"                    : "AltMemZone",
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
      },
      {
        "Name"                    : "Mempool2",
        "CacheSizeBytes"          : 10,
        "PrivateSizeBytes"        : 20,
        "DataRoomSizeBytes"       : 32768,
        "MbufCount"               : 1024920,
        "MemzoneName"             : "AltMemZone"
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
      },
      {
        "Name"                    : "TXQ2",
        "MQMask"                  : 11,
        "OffloadMask"             : 12,
        "PThreshold"              : 42,
        "HThreshold"              : 14,
        "WThreshold"              : 15,
        "RSThreshold"             : 42,
        "FreeThreshold"           : 53,
        "RingSize"                : 512,
        "MempoolName"             : "Mempool2"
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
  u_int64_t   vValue;

  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQ::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)2, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQ::Validate(json));

  std::string name = "TXQ1";
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQ::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "TXQ2";
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQ::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);

  name = "BubbaTXQ";
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQ::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "TXQ1");
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::MQMask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)1);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::OffloadMask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)2);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::PThreshold(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)32);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::HThreshold(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)4);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::WThreshold(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)5);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::RSThreshold(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)32);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::FreeThreshold(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)33);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::RingSize(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int64_t)1024);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::MempoolName(0, json, &sValue));
  EXPECT_EQ(sValue, "Mempool1");

  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::Name(1, json, &sValue));
  EXPECT_EQ(sValue, "TXQ2");
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::MQMask(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)11);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::OffloadMask(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)12);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::PThreshold(1, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)42);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::HThreshold(1, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)14);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::WThreshold(1, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)15);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::RSThreshold(1, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)42);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::FreeThreshold(1, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)53);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::RingSize(1, json, &uValue));
  EXPECT_EQ(uValue, (u_int64_t)512);
  EXPECT_EQ(0, HOMA::Network::cfgparse::TXQNode::MempoolName(1, json, &sValue));
  EXPECT_EQ(sValue, "Mempool2");

  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::Name(2, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::MQMask(2, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::OffloadMask(2, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::PThreshold(2, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::HThreshold(2, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::WThreshold(2, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::RSThreshold(2, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::FreeThreshold(2, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::RingSize(2, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQNode::MempoolName(2, json, &sValue));
}

TEST(parseConfig, txq2) {
  std::string config = R"(
  {
    "Memzone": [                                                                                                        
      {                                                                                                                 
        "Name"                    : "DefaultMemZone",                                                                   
        "SizeBytes"               : 1024,                                                                               
        "Mask"                    : 65536,                                                                              
        "NumaNode"                : 0                                                                                   
      },                                                                                                                
      {                                                                                                                 
        "Name"                    : "AltMemZone",                                                                       
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
      },                                                                                                                
      {                                                                                                                 
        "Name"                    : "Mempool1",                                                                         
        "CacheSizeBytes"          : 10,                                                                                 
        "PrivateSizeBytes"        : 20,                                                                                 
        "DataRoomSizeBytes"       : 102444,                                                                             
        "MbufCount"               : 1024920,                                                                            
        "MemzoneName"             : "AltMemzone"                                                                        
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
      },
      {
        "Name"                    : "TXQ1",
        "MQMask"                  : 11,
        "OffloadMask"             : 12,
        "PThreshold"              : 42,
        "HThreshold"              : 14,
        "WThreshold"              : 15,
        "RSThreshold"             : 42,
        "FreeThreshold"           : 53,
        "RingSize"                : 512,
        "MempoolName"             : "Mempool2"
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
  
  EXPECT_NE(0, HOMA::Network::cfgparse::TXQ::Validate(json));
} 
