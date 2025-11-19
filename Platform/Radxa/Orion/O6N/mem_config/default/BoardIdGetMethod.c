#include "MemConfigBinTool.h"
#include "MemConfigurationTable.h"

MEM_CONFIG_BDID_GET_METHOD BoardIdGetMethod = {
  {
    .Signature      = MEM_CONFIG_BDID_GET_METHOD_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BDID_GET_METHOD) + sizeof(BDID_GET_METHOD_GPIO_S),
    .BoardMask      = BOARD_ID_MASK_DEFAULT
  },
  BDID_GET_METHOD_GPIO,
  // LSB
  {
    49, 50, 57, 60, 0xFF, 0xFF, 0xFF, 0xFF
  }
};
