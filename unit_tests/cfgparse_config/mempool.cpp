#include <gtest/gtest.h>
#include <string>
#include <fmt/core.h>
#include <cfgparse_mempool.h>

TEST(parseConfig, mempool) {
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

  EXPECT_EQ(0, HOMA::Network::cfgparse::Mempool::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::Mempool::Validate(json));

  std::string name = "Mempool1";
  EXPECT_EQ(0, HOMA::Network::cfgparse::Mempool::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "BubbaMempool";
  EXPECT_NE(0, HOMA::Network::cfgparse::Mempool::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "Mempool1");
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::CacheSizeBytes(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::PrivateSizeBytes(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::DataRoomSizeBytes(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)1024);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::MbufCount(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)10240);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::MemzoneName(0, json, &sValue));
  EXPECT_EQ(sValue, "DefaultMemZone");

  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::Name(1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::CacheSizeBytes(1, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::PrivateSizeBytes(1, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::DataRoomSizeBytes(1, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::MbufCount(1, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::MemzoneName(1, json, &sValue));
}

TEST(parseConfig, mempool1) {
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

  EXPECT_EQ(0, HOMA::Network::cfgparse::Mempool::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)2, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::Mempool::Validate(json));

  std::string name = "Mempool1";
  EXPECT_EQ(0, HOMA::Network::cfgparse::Mempool::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "Mempool2";
  EXPECT_EQ(0, HOMA::Network::cfgparse::Mempool::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);

  name = "BubbaMempool";
  EXPECT_NE(0, HOMA::Network::cfgparse::Mempool::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "Mempool1");
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::CacheSizeBytes(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::PrivateSizeBytes(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::DataRoomSizeBytes(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)1024);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::MbufCount(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)10240);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::MemzoneName(0, json, &sValue));
  EXPECT_EQ(sValue, "DefaultMemZone");

  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::Name(1, json, &sValue));
  EXPECT_EQ(sValue, "Mempool2");
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::CacheSizeBytes(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)10);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::PrivateSizeBytes(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)20);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::DataRoomSizeBytes(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)32768);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::MbufCount(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)1024920);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MempoolNode::MemzoneName(1, json, &sValue));
  EXPECT_EQ(sValue, "AltMemZone");

  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::Name(2, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::CacheSizeBytes(2, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::PrivateSizeBytes(2, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::DataRoomSizeBytes(2, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::MbufCount(2, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MempoolNode::MemzoneName(2, json, &sValue));
}

TEST(parseConfig, mempool2) {
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
  
  EXPECT_NE(0, HOMA::Network::cfgparse::Mempool::Validate(json));
} 
