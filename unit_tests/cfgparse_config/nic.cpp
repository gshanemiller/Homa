#include <gtest/gtest.h>
#include <string>
#include <fmt/core.h>
#include <cfgparse_nic.h>

TEST(parseConfig, nic) {
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

  EXPECT_EQ(0, HOMA::Network::cfgparse::NIC::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NIC::Validate(json));

  std::string name = "DefaultNic";
  EXPECT_EQ(0, HOMA::Network::cfgparse::NIC::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "BubbaNic";
  EXPECT_NE(0, HOMA::Network::cfgparse::NIC::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "DefaultNic");
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::DeviceId(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::PciDeviceId(0, json, &sValue));
  EXPECT_EQ(sValue, "0000:7f:00.0");
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::NumaNode(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::MTUSizeBytes(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)1500);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::LinkSpeed(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::TxFlowMask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)2048);

  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::Name(1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::DeviceId(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::PciDeviceId(1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::NumaNode(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::MTUSizeBytes(1, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::LinkSpeed(1, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::TxFlowMask(1, json, &vValue));
}

TEST(parseConfig, nic1) {
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
        },
        {
            "Name"                    : "AltNic",
            "DeviceId"                : 1,
            "PciDeviceId"             : "0000:7f:00.1",
            "NumaNode"                : 1,
            "MTUSizeBytes"            : 1600,
            "LinkSpeed"               : 4,
            "TxFlowMask"              : 4048
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

  EXPECT_EQ(0, HOMA::Network::cfgparse::NIC::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)2, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NIC::Validate(json));

  std::string name = "DefaultNic";
  EXPECT_EQ(0, HOMA::Network::cfgparse::NIC::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "AltNic";
  EXPECT_EQ(0, HOMA::Network::cfgparse::NIC::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);

  name = "BubbaNic";
  EXPECT_NE(0, HOMA::Network::cfgparse::NIC::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "DefaultNic");
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::DeviceId(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::PciDeviceId(0, json, &sValue));
  EXPECT_EQ(sValue, "0000:7f:00.0");
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::NumaNode(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::MTUSizeBytes(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)1500);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::LinkSpeed(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)0);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::TxFlowMask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)2048);

  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::Name(1, json, &sValue));
  EXPECT_EQ(sValue, "AltNic");
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::DeviceId(1, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)1);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::PciDeviceId(1, json, &sValue));
  EXPECT_EQ(sValue, "0000:7f:00.1");
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::NumaNode(1, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)1);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::MTUSizeBytes(1, json, &uValue));
  EXPECT_EQ(uValue, (u_int32_t)1600);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::LinkSpeed(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)4);
  EXPECT_EQ(0, HOMA::Network::cfgparse::NICNode::TxFlowMask(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)4048);

  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::Name(2, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::DeviceId(2, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::PciDeviceId(2, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::NumaNode(2, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::MTUSizeBytes(2, json, &uValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::LinkSpeed(2, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::NICNode::TxFlowMask(2, json, &vValue));
}

TEST(parseConfig, nic2) {
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
        },
        {
            "Name"                    : "AltNic",
            "DeviceId"                : 1,
            "PciDeviceId"             : "0000:7f:00.1",
            "NumaNode"                : 1,
            "MTUSizeBytes"            : 1600,
            "LinkSpeed"               : 4,
            "TxFlowMask"              : 4048
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
  
  EXPECT_EQ(0, HOMA::Network::cfgparse::NIC::Validate(json));

  try {
    std::string oldName = json["NIC"][0]["Name"];
    u_int32_t oldDeviceId  = json["NIC"][0]["DeviceId"];
    std::string oldPciDeviceId = json["NIC"][0]["PciDeviceId"];
    EXPECT_EQ(oldName, "DefaultNic"); 
    EXPECT_EQ(oldDeviceId, (u_int32_t)0);
    EXPECT_EQ(oldPciDeviceId, "0000:7f:00.0");

    json["NIC"][0]["Name"] = json["NIC"][1]["Name"];
    EXPECT_NE(0, HOMA::Network::cfgparse::NIC::Validate(json));
    json["NIC"][0]["Name"] = oldName;

    json["NIC"][0]["DeviceId"] = json["NIC"][1]["DeviceId"];
    EXPECT_NE(0, HOMA::Network::cfgparse::NIC::Validate(json));
    json["NIC"][0]["DeviceId"] = oldDeviceId;

    json["NIC"][0]["PciDeviceId"] = json["NIC"][1]["PciDeviceId"];
    EXPECT_NE(0, HOMA::Network::cfgparse::NIC::Validate(json));
    json["NIC"][0]["PciDeviceId"] = oldPciDeviceId;
  } catch (const nlohmann::json::exception& e) {
    fmt::print("json test invalid: {}\n", e.what());
    EXPECT_TRUE(0);
  }
} 
