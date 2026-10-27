/** @file
  Install ACPI TPM2 table matching CIX Ftpm.aslc (CRB + TrEE start method 9).

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/Tpm2Acpi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/AcpiTable.h>

// OEM fields match Platform/CIX/Sky1/Include/AcpiPlatform.h (avoid that
// header here: it pulls AcpiCommon.h from CixPkg, which this driver does not need).
#define EFI_ACPI_OEM_TABLE_ID      SIGNATURE_64 ('S', 'K', 'Y', '1', 'E', 'D', 'K', '2')
#define EFI_ACPI_OEM_REVISION      0x01000101
#define EFI_ACPI_CREATOR_ID        SIGNATURE_32 ('C', 'I', 'X', ' ')
#define EFI_ACPI_CREATOR_REVISION  0x00000001

STATIC CONST UINT8  mOemId[6] = { 'C', 'I', 'X', 'T', 'E', 'K' };

#ifndef EFI_TPM2_ACPI_TABLE_START_METHOD_COMMAND_RESPONSE_BUFFER_INTERFACE_WITH_TREE
#define EFI_TPM2_ACPI_TABLE_START_METHOD_COMMAND_RESPONSE_BUFFER_INTERFACE_WITH_TREE  9
#endif

EFI_STATUS
EFIAPI
Tcg2AcpiEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_ACPI_TABLE_PROTOCOL  *AcpiTable;
  EFI_TPM2_ACPI_TABLE   Table;
  UINTN                 TableKey;

  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  (VOID **)&AcpiTable
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tcg2Acpi: no ACPI protocol: %r\n", Status));
    return Status;
  }

  ZeroMem (&Table, sizeof (Table));
  Table.Header.Signature = EFI_ACPI_5_0_TRUSTED_COMPUTING_PLATFORM_2_TABLE_SIGNATURE;
  Table.Header.Length    = sizeof (EFI_TPM2_ACPI_TABLE);
  Table.Header.Revision  = PcdGet8 (PcdTpm2AcpiTableRev);
  if (Table.Header.Revision == 0) {
    Table.Header.Revision = EFI_TPM2_ACPI_TABLE_REVISION;
  }

  CopyMem (Table.Header.OemId, mOemId, sizeof (Table.Header.OemId));
  Table.Header.OemTableId      = EFI_ACPI_OEM_TABLE_ID;
  Table.Header.OemRevision     = EFI_ACPI_OEM_REVISION;
  Table.Header.CreatorId       = EFI_ACPI_CREATOR_ID;
  Table.Header.CreatorRevision = EFI_ACPI_CREATOR_REVISION;
  Table.Flags                  = 0;
  Table.AddressOfControlArea   = PcdGet32 (PcdTpm2AcpiBufferBase);
  Table.StartMethod            = EFI_TPM2_ACPI_TABLE_START_METHOD_COMMAND_RESPONSE_BUFFER_INTERFACE_WITH_TREE;

  Table.Header.Checksum = 0;
  Table.Header.Checksum = CalculateCheckSum8 ((UINT8 *)&Table, Table.Header.Length);

  TableKey = 0;
  Status   = AcpiTable->InstallAcpiTable (
                          AcpiTable,
                          &Table,
                          Table.Header.Length,
                          &TableKey
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Tcg2Acpi: install failed: %r\n", Status));
    return Status;
  }

  DEBUG ((
    DEBUG_INFO,
    "Tcg2Acpi: TPM2 table installed, CRB=0x%lx method=%u\n",
    Table.AddressOfControlArea,
    Table.StartMethod
    ));
  return EFI_SUCCESS;
}
