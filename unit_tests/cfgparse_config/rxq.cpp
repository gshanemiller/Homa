#include <gtest/gtest.h>
#include <string>
#include <fmt/core.h>
#include <cfgparse_rxq.h>

TEST(parseConfig, rxq) {
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

  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQ::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQ::Validate(json));

  std::string name = "RXQ1";
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQ::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "BubbaRXQ";
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQ::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "RXQ1");
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::MQMask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::OffloadMask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::RingSize(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int64_t)1024);
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::MempoolName(0, json, &sValue));
  EXPECT_EQ(sValue, "Mempool1");

  EXPECT_NE(0, HOMA::Network::cfgparse::RXQNode::Name(1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQNode::MQMask(1, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQNode::OffloadMask(1, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQNode::RingSize(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQNode::MempoolName(1, json, &sValue));
}

TEST(parseConfig, rxq1) {
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
    "RXQ": [
      {
        "Name"                    : "RXQ1",
        "MQMask"                  : 0,
        "OffloadMask"             : 0,
        "RingSize"                : 1024,
        "MempoolName"             : "Mempool1"
      },
      {
        "Name"                    : "RXQ2",
        "MQMask"                  : 40,
        "OffloadMask"             : 10,
        "RingSize"                : 11,
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

  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQ::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)2, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQ::Validate(json));

  std::string name = "RXQ1";
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQ::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "RXQ2";
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQ::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);

  name = "BubbaRXQ";
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQ::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "RXQ1");
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::MQMask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::OffloadMask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::RingSize(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int64_t)1024);
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::MempoolName(0, json, &sValue));
  EXPECT_EQ(sValue, "Mempool1");

  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::Name(1, json, &sValue));
  EXPECT_EQ(sValue, "RXQ2");
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::MQMask(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)40);
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::OffloadMask(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)10);
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::RingSize(1, json, &uValue));
  EXPECT_EQ(uValue, (u_int64_t)11);
  EXPECT_EQ(0, HOMA::Network::cfgparse::RXQNode::MempoolName(1, json, &sValue));
  EXPECT_EQ(sValue, "Mempool2");

  EXPECT_NE(0, HOMA::Network::cfgparse::RXQNode::Name(2, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQNode::MQMask(2, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQNode::OffloadMask(2, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQNode::RingSize(2, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQNode::MempoolName(2, json, &sValue));
}

TEST(parseConfig, rxq2) {
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
    "RXQ": [
      {
        "Name"                    : "RXQ1",
        "MQMask"                  : 0,
        "OffloadMask"             : 0,
        "RingSize"                : 1024,
        "MempoolName"             : "Mempool1"
      },
      {
        "Name"                    : "RXQ1",
        "MQMask"                  : 40,
        "OffloadMask"             : 10,
        "RingSize"                : 50,
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
  
  EXPECT_NE(0, HOMA::Network::cfgparse::RXQ::Validate(json));
} 
