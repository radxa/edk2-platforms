#include "MemConfigBinTool.h"
#include "BoardConfig.h"
#include "MemConfigurationTable.h"

#define ENTRY_COUNT 10

// PCBID[6][2:0]
#define RS600  4

// PCBID[7][5:3]
#define DRAM_4G_SR_315       0
#define DRAM_2G_SR_315       2
#define DRAM_3G_SR_315       5
#define DRAM_8G_DR_315_x8_HS 6
#define DRAM_6G_SR_315_HS    7
#define DRAM_8G_DR_315_LO    8
#define DRAM_12G_DR_315_HS   9
#define DRAM_16G_DR_x8_315   10
#define DRAM_8G_DR_315       11
#define DRAM_16G_RAYSON_315  12

#define MSK_PCB   0xFFB8
#define MSK_DRAM  0xFF47
#define MSK_REV   0xFF
#define MSK_ALL   0

//               PCBID[2:0]    PCBID[5:3]         PCBID[6]           PCBID[7]             PCBID[9:8]
#define PCB(b,d,r) (((b) & 7) + (((d) & 7) << 3) + (((b) & 8) << 3) + (((d) & 8) << 4)) + (((r) & 3) << 8)

MEM_CONFIG_BLOCK_BOARDID_MAP BoardIdMapBlock = {
  {
    .Signature      = MEM_CONFIG_BLOCK_BOARDID_MAP_SIGNAUTE,
    .BlockSize      = sizeof(MEM_CONFIG_BLOCK_HEADER) + sizeof(BOARD_ID_MAP_ENTRY) * ENTRY_COUNT,
    .BoardMask      = BOARD_ID_MASK_DEFAULT
  },
  {
    {PCB(RS600, DRAM_2G_SR_315,       0), MSK_PCB & MSK_DRAM, RS600_8G_ID        },
    {PCB(RS600, DRAM_3G_SR_315,       0), MSK_PCB & MSK_DRAM, RS600_12G_ID       },
    {PCB(RS600, DRAM_4G_SR_315,       0), MSK_PCB & MSK_DRAM, RS600_16G_ID       },
    {PCB(RS600, DRAM_6G_SR_315_HS,    0), MSK_PCB & MSK_DRAM, RS600_24G_HS_ID    },
    {PCB(RS600, DRAM_8G_DR_315,       0), MSK_PCB & MSK_DRAM, RS600_32G_ID       },
    {PCB(RS600, DRAM_8G_DR_315_LO,    0), MSK_PCB & MSK_DRAM, RS600_32G_LO_ID    },
    {PCB(RS600, DRAM_8G_DR_315_x8_HS, 0), MSK_PCB & MSK_DRAM, RS600_32G_x8_HS_ID },
    {PCB(RS600, DRAM_12G_DR_315_HS,   0), MSK_PCB & MSK_DRAM, RS600_48G_HS_ID    },
    {PCB(RS600, DRAM_16G_DR_x8_315,   0), MSK_PCB & MSK_DRAM, RS600_64G_x8_ID    },
    {PCB(RS600, DRAM_16G_RAYSON_315,  0), MSK_PCB & MSK_DRAM, RS600_64G_RAYSON_ID},
  }
};
