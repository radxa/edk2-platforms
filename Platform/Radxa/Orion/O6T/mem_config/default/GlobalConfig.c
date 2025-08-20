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
  .DeviceDensity    = 12,
  .DeviceWidth      = 16,
  .RankNum          = 2,
};

MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock_24G = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = ORION_O6T_24G_MASK
  },
  .MaxFreq          = DDR3200_FREQUENCY,
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 12,
  .DeviceWidth      = 16,
  .RankNum          = 2,
};
