#include <gtest/gtest.h>
#include <string>
#include <fmt/core.h>
#include <cfgparse_memzone.h>

TEST(parseConfig, memzone) {
  std::string config = R"(
  {
    "Memzone": [
      {
        "Name"                    : "DefaultMemZone",
        "SizeBytes"               : 1024,
        "Mask"                    : 65536,
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
  u_int64_t   vValue;

  EXPECT_EQ(0, HOMA::Network::cfgparse::Memzone::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::Memzone::Validate(json));

  std::string name = "DefaultMemZone";
  EXPECT_EQ(0, HOMA::Network::cfgparse::Memzone::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "BubbaZone";
  EXPECT_NE(0, HOMA::Network::cfgparse::Memzone::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "DefaultMemZone");
  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::SizeBytes(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)1024);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::Mask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)65536);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::NumaNode(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int64_t)0);

  EXPECT_NE(0, HOMA::Network::cfgparse::MemzoneNode::Name(1, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MemzoneNode::SizeBytes(1, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MemzoneNode::Mask(1, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MemzoneNode::NumaNode(1, json, &uValue));
}

TEST(parseConfig, memzone1) {
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
        "SizeBytes"               : 2024,
        "Mask"                    : 5536,
        "NumaNode"                : 1
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

  EXPECT_EQ(0, HOMA::Network::cfgparse::Memzone::Count(json, &uValue));
  EXPECT_EQ((u_int32_t)2, uValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::Memzone::Validate(json));

  std::string name = "DefaultMemZone";
  EXPECT_EQ(0, HOMA::Network::cfgparse::Memzone::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)0, uValue);

  name = "AltMemZone";
  EXPECT_EQ(0, HOMA::Network::cfgparse::Memzone::Find(json, name, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);

  name = "BubbaMemzone";
  EXPECT_NE(0, HOMA::Network::cfgparse::Memzone::Find(json, name, &uValue));

  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::Name(0, json, &sValue));
  EXPECT_EQ(sValue, "DefaultMemZone");
  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::SizeBytes(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)1024);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::Mask(0, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)65536);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::NumaNode(0, json, &uValue));
  EXPECT_EQ(uValue, (u_int64_t)0);

  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::Name(1, json, &sValue));
  EXPECT_EQ(sValue, "AltMemZone");
  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::SizeBytes(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)2024);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::Mask(1, json, &vValue));
  EXPECT_EQ(vValue, (u_int64_t)5536);
  EXPECT_EQ(0, HOMA::Network::cfgparse::MemzoneNode::NumaNode(1, json, &uValue));
  EXPECT_EQ(uValue, (u_int64_t)1);

  EXPECT_NE(0, HOMA::Network::cfgparse::MemzoneNode::Name(2, json, &sValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MemzoneNode::SizeBytes(2, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MemzoneNode::Mask(2, json, &vValue));
  EXPECT_NE(0, HOMA::Network::cfgparse::MemzoneNode::NumaNode(2, json, &uValue));
}

TEST(parseConfig, memzone2) {
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
        "Name"                    : "DefaultMemZone",
        "SizeBytes"               : 2024,
        "Mask"                    : 5536,
        "NumaNode"                : 1
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
  
  EXPECT_NE(0, HOMA::Network::cfgparse::Memzone::Validate(json));
} 
