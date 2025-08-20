#ifndef _MEM_CONFIG_BIN_LAYOUT_H_
#define _MEM_CONFIG_BIN_LAYOUT_H_

// default
extern MEM_QUICK_CONFIG             QuickConfigBlock;
extern MEM_CONFIG_BLOCK_CONFIG      GlobalConfigBlock;
extern MEM_CONFIG_BLOCK_CONFIG      GlobalConfigBlock_24G;
extern MEM_CONFIG_BLOCK_FEATURE     MemFeatureBlock;
extern MEM_CONFIG_BUSCFG_LP5        MemLpddr5BusCfg;
extern MEM_CONFIG_PHYPADCFG         MemPhyPadCfg;
extern MEM_CONFIG_BIOS_SETUP        MemBiosSetup;
extern MEM_CONFIG_DQ_SWAP           DqDmMapBlock;
extern MEM_CONFIG_TRACE_LEN_DIFF    MemTraceLenDiffCfg;
extern MEM_CONFIG_TRAIN_OPTIMIZE    MemTrainOptimize;
extern MEM_CONFIG_BDID_GET_METHOD   BoardIdGetMethod;

#define CDCB_BLOCK_LIST \
  /* default */ \
  CDCB_BLOCK(MemBiosSetup), \
  CDCB_BLOCK(BoardIdGetMethod), \
  CDCB_BLOCK(GlobalConfigBlock), \
  CDCB_BLOCK(GlobalConfigBlock_24G), \
  CDCB_BLOCK(MemFeatureBlock), \
  CDCB_BLOCK(MemLpddr5BusCfg), \
  CDCB_BLOCK(MemPhyPadCfg), \
  CDCB_BLOCK(DqDmMapBlock), \
  CDCB_BLOCK(MemTraceLenDiffCfg), \
  CDCB_BLOCK(MemTrainOptimize), \

#endif
