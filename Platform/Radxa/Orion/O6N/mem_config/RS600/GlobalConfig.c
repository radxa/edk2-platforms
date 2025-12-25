#include "BoardConfig.h"
#include "MemConfigBinTool.h"
#include "MemConfigurationTable.h"


MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock_16G_HS = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = RS602_16G_HS_MASK
  },
  .MaxFreq          = DDR4800_FREQUENCY,
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 8,
  .DeviceWidth      = 16,
  .RankNum          = 2,
};

MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock_16G_Hynix = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = RS602_16G_HYNIX_MASK
  },
  .MaxFreq          = DDR6000_FREQUENCY,
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 8,
  .DeviceWidth      = 16,
  .RankNum          = 2,
};

MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock_24G_HS = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = RS602_24G_HS_MASK
  },
  .MaxFreq          = DDR5500_FREQUENCY,
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 12,
  .DeviceWidth      = 16,
  .RankNum          = 2,
};

MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock_32G_x8_HS = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = RS602_32G_x8_HS_MASK
  },
  .MaxFreq          = DDR5500_FREQUENCY,
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 8,
  .DeviceWidth      = 8,
  .RankNum          = 2,
};

MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock_32G_Hynix = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = RS602_32G_HYNIX_MASK
  },
  .MaxFreq          = DDR6000_FREQUENCY,
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 16,
  .DeviceWidth      = 16,
  .RankNum          = 2,
};

MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock_48G_HS = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = RS602_48G_HS_MASK
  },
  .MaxFreq          = DDR5000_FREQUENCY,
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 12,
  .DeviceWidth      = 8,
  .RankNum          = 2,
};

MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock_48G_Hynix = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = RS602_48G_HYNIX_MASK
  },
  .MaxFreq          = DDR6000_FREQUENCY,  
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 12,
  .DeviceWidth      = 8,
  .RankNum          = 2,
};

MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock_64G_Rayson = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = RS602_64G_RAYSON_MASK
  },
  .MaxFreq          = DDR4800_FREQUENCY,
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 16,
  .DeviceWidth      = 8,
  .RankNum          = 2,
};
