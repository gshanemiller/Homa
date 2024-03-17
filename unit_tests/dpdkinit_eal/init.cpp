#include <gtest/gtest.h>
#include <string>
#include <fmt/core.h>
#include <cfgparse_dpdk.h>
#include <dpdkinit_eal.h>

std::string config = R"(
{
    "Dpdk": {
        "Initialization": {
            "Argument"                : "--proc-type,primary,--in-memory,--log-level,7,--allow,0000:02:00.1,--huge-dir,/dev/hugepages"
        },
        "MemoryChannelCount"          : 4
    },

    "NIC": [ 
         {
            "Name"                    : "DefaultNic",
            "DeviceId"                : 0,
            "PciDeviceId"             : "0000:02:00.1",
            "NumaNode"                : 0,
            "MTUSizeBytes"            : 1500,
            "LinkSpeed"               : 0,
            "RXMQMode"                : 1,
            "RXQOffloadMask"          : 0,
            "TXMQMode"                : 0,
            "TXQOffloadMask"          : 0
        }
    ],

    "Memzone": [
        {
            "Name"                    : "DefaultMemZone",
            "SizeBytes"               : 100000000,
            "Mask"                    : 2,
            "NumaNode"                : 0
        }
    ],

    "Mempool": [
        {
            "Name"                    : "Mempool1",
            "CacheSizeBytes"          : 0,
            "PrivateSizeBytes"        : 0,
            "DataRoomSizeBytes"       : 2048,
            "MbufCount"               : 1024,
            "MemzoneName"             : "DefaultMemZone"
        }
    ],

    "RXQ": [
        {
            "Name"                    : "RXQ1",
            "PThreshold"              : 32,
            "HThreshold"              : 0,
            "WThreshold"              : 0,
            "FreeThreshold"           : 32,
            "RingSize"                : 128,
            "MempoolName"             : "Mempool1"
        }
    ],

    "TXQ": [
        {
            "Name"                    : "TXQ1",
            "PThreshold"              : 32,
            "HThreshold"              : 0,
            "WThreshold"              : 0,
            "RSThreshold"             : 32,
            "FreeThreshold"           : 32,
            "RingSize"                : 128,
            "FlowMask"                : 2048,
            "MempoolName"             : "Mempool1"
       }
    ],
            
    "Thread": [
        {
            "Name"                    : "Main",
            "CPU"                     : 0,
            "NumaNode"                : 0,
            "RXQ": [
                {
                    "RefQueueName"    : "RXQ1",
                    "RefNicName"      : "DefaultNic",
                    "Mode"            : "Allocate"
                }
            ],
            "TXQ": [
                {
                    "RefQueueName"    : "TXQ1",
                    "RefNicName"      : "DefaultNic",
                    "Mode"            : "Allocate"
                }
            ]
        }
    ]
}
)";

TEST(dpdkinit, init) {
  nlohmann::json json; 

  try {
    json = nlohmann::json::parse(config);
  }
  catch (const nlohmann::json::exception& e) {
    fmt::print("json config invalid: '{}'\n{}\n", config, e.what());
    EXPECT_TRUE(0);
  }
  
  HOMA::Network::dpdkinit::Eal eal(json);
  EXPECT_EQ(0, eal.start());
  EXPECT_EQ(0, eal.stop());
  EXPECT_EQ(0, eal.shutdown());
}
