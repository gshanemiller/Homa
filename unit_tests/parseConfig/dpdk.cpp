#include <gtest/gtest.h>
#include <string>
#include <fmt/core.h>
#include <cfgparse_dpdk.h>

TEST(parseConfig, dpdk) {
  std::string config = R"(
  {
    "Dpdk": {
      "Initialization": {
          "Argument"                : ""
      },
      "MemoryChannelCount"          : 1
    }
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

  EXPECT_EQ(0, HOMA::Network::cfgparse::Dpdk::Argument(json, &sValue));
  EXPECT_EQ("", sValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::Dpdk::MemoryChannelCount(json, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);
}

TEST(parseConfig, dpdk1) {
  std::string config = R"(
  {
    "Dpdk": {
        "Initialization": {
            "Argument"                : "test"
        },
        "MemoryChannelCount"          : 32
    }
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
  u_int32_t    uValue;

  EXPECT_EQ(0, HOMA::Network::cfgparse::Dpdk::Argument(json, &sValue));
  EXPECT_EQ("test", sValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::Dpdk::MemoryChannelCount(json, &uValue));
  EXPECT_EQ((u_int32_t)32, uValue);
}

TEST(parseConfig, dpdk2) {
  std::string config = R"(
  {
    "Dpdk": {
        "Initialization": {
        }
    }
  }
  )";

  std::string defaultJson = R"({
  "Dpdk": {
    "Initialization": {
      "Argument": ""
    },
    "MemoryChannelCount": 1
  }
})";

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

  EXPECT_EQ(0, HOMA::Network::cfgparse::Dpdk::Argument(json, &sValue));
  EXPECT_EQ("", sValue);
  EXPECT_EQ(0, HOMA::Network::cfgparse::Dpdk::MemoryChannelCount(json, &uValue));
  EXPECT_EQ((u_int32_t)1, uValue);

  std::string updatedJson = json.dump(2);
  EXPECT_EQ(updatedJson, defaultJson);
}
