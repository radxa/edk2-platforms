#ifndef _MEM_OUTPUT_BUFFER_H_
#define _MEM_OUTPUT_BUFFER_H_

#define MEM_OUTPUT_BUFFER_ADDR 0x83C00000

#define MEM_OUTPUT_BUFFER_MAJOR_VER 1
#define MEM_OUTPUT_BUFFER_MINOR_VER 0

#define MEM_OUTPUT_BUFFER_SIG 0x424F4443 // "CDOB"

typedef UINT8   uint8_t;
typedef UINT16  uint16_t;
typedef UINT32  uint32_t;
typedef UINT64  uint64_t;

typedef struct {
  uint32_t Signature;
  uint16_t MajorVer;
  uint16_t MinorVer;
  uint8_t  DdrType;
  uint8_t  ChannelMask;
  uint8_t  RanksPerChannel;
  uint8_t  Reserved0;
  uint32_t TotalSize; // MB
  uint32_t AvailableSize; // MB
} MEM_INIT_OUTPUT_BUFFER;

#endif
