#include "MemConfigBinTool.h"
#include "MemConfigurationTable.h"
#include "BoardConfig.h"

MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = BOARD_ID_MASK_DEFAULT
  },
  .MaxFreq          = DDR5500_FREQUENCY,
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 6,
  .DeviceWidth      = 16,
  .RankNum          = 1,
};

MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock_16G = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = ORION_O6T_16G_MASK
  },
  .MaxFreq          = DDR3200_FREQUENCY,
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 8,
  .DeviceWidth      = 16,
  .RankNum          = 2,
};
