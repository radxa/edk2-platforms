
#include "Base.h"
#include "ProcessorBind.h"
#include <Uefi.h>


#define CIX_VAR_SYNC_SIGNATURE  SIGNATURE_32 ('G', 'P', 'N', 'V')

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH          Vendor;
  UINT8                       Index;
  EFI_DEVICE_PATH_PROTOCOL    End;
} NOR_FLASH_DEVICE_PATH;

typedef struct {
  UINT32    Signature;           // "GPNV"
  UINT32    RecordSize;          // Size of this record in bytes
  UINT32    VariableNameSize;    // Size of variable name in bytes
  UINT32    VariableSize;        // Size of variable data in bytes
  UINT32    Data[0];             // VariableName+VariableData
} VARIABLE_RECORD_HEADER;

typedef struct {
  LIST_ENTRY Link;
  VARIABLE_RECORD_HEADER *Record;
} VARIABLE_RECORD_LIST;
#pragma pack()

/**
  Synchronize variables between gVendorGuid Variable and FlashRegionBase storage.
  This function will check the records in the flash region and the gVendorGuid Variable.
  If the record is not in the flash region, it will be added to the flash region.
  If the record is not in the gVendorGuid Variable, it will be added to the gVendorGuid Variable.
  If the record is different, it will be updated in the flash region or gVendorGuid Variable.
  @param  Guid             Vendor Guid
  @param  FlashRegionBase  Flash Region Base Address
  @param  FlashRegionSize  Flash Region Size
  @param  RecordSize       Record Size

  @retval EFI_SUCCESS      Get all vendor guid var success
  @retval Other            Get all vendor guid var failed
**/
EFI_STATUS
EFIAPI
CixGPNVSync(
  EFI_GUID *VendorGuid,
  UINTN    FlashRegionBase,
  UINTN    FlashRegionSize,
  UINTN    RecordSize
);