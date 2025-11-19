#include "BoardConfig.h"
#include "MemConfigBinTool.h"
#include "MemConfigurationTable.h"

MEM_CONFIG_TRACE_LEN_DIFF MemTraceLenDiffCfg = {
  {
    .Signature      = MEM_CONFIG_TRACE_LEN_DIFF_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_TRACE_LEN_DIFF),
    .BoardMask      = BOARD_ID_MASK_DEFAULT
  },
  {
    // Avg Flight Time      Trace Length Diff
    {{500, 500, 500, 500}, {27, 76,  68,  26 }}, // CH0
    {{500, 500, 500, 500}, {52, 94,  97,  32 }}, // CH1
    {{500, 500, 500, 500}, {60, 128, 100, 32 }}, // CH2
    {{500, 500, 500, 500}, {74, 38,  62,  76 }}, // CH3
  }
};
