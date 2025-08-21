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
    {{500, 500, 500, 500}, {131, 135, 34,  66}}, // CH0
    {{500, 500, 500, 500}, {99,  53,  80,  72}}, // CH1
    {{500, 500, 500, 500}, {94,  42,  53,  37}}, // CH2
    {{500, 500, 500, 500}, {168, 138, 49,  126}}, // CH3
  }
};
