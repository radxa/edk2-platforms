#include "MemConfigBinTool.h"
#include "MemConfigurationTable.h"

#ifndef VF_CFG_VOL_MAJOR_VER
#define VF_CFG_VOL_MAJOR_VER   VOL_Major_VER_AUTO
#endif

#ifndef VF_CFG_VOL_MINOR_VER
#define VF_CFG_VOL_MINOR_VER   VOL_Sub_VER_AUTO
#endif

#ifndef VF_CFG_VARIANT
#define VF_CFG_VARIANT         DFS_DVFS_AUTO
#endif

MEM_QUICK_CONFIG   QuickConfigBlock = {
  .BoardIdGettingMethod = MULTI_BOARD_METHOD_GPIO,
  .BoardId              = 0xFF,
  .BoardIdChanged       = 0xFF,
  .BootEnv              = ENV_AUTO,
  .LogFilter            = 0xFFFFFFFF,
  .vol_major_ver        = 14,
  .dfs_dvfs             = 0,
  .vol_sub_ver          = 0
};
