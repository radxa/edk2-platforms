#ifndef _RADXA_SETUP_VAR_H_
#define _RXDXA_SETUP_VAR_H_

#define RADXA_SETUP_VARIABLE_GUID \
  { 0xeedf122d, 0xa912, 0x43d5, {0xaa, 0x11, 0x14, 0x27, 0x62, 0xdd, 0x96, 0xfa}}

#define RADXA_SETUP_VAR  L"RadxaSetupVar"

extern EFI_GUID  gRadxaSetupVariableGuid;

#define RADXA_SETUP_UFS_POWER_AUTO          0
#define RADXA_SETUP_UFS_POWER_ON            1
#define RADXA_SETUP_UFS_POWER_OFF           2
#define RADXA_SETUP_UFS_POWER_NOT_AVAILABLE 0xFF

#pragma pack(1)

typedef struct {
  UINT8     UFSPowerMode;
  UINT8     EnableAcpiScmi;
} RADXA_SETUP_DATA;

#pragma pack()

#endif