#include "MemConfigBinTool.h"
#include "MemConfigurationTable.h"

MEM_CONFIG_BLOCK_CONFIG  GlobalConfigBlock = {
  {
    .Signature      = MEM_CONFIG_BLOCK_CONFIG_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_CONFIG),
    .BoardMask      = BOARD_ID_MASK_DEFAULT
  },
  .MaxFreq          = DDR5500_FREQUENCY,
  .ChMask           = 0xF,
  .DdrType          = DDR_TYPE_LPDDR5,
  .DeviceDensity    = 0xFF,
  .DeviceWidth      = 0xFF,
  .RankNum          = 0xFF,
};
