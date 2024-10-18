[PcdsFixedAtBuild.common]

!if $(DEBUG_ON_UART3) == FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x040D0000 #0x091e1000 #0x0a051000
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x040E0000
!endif

  # Default State After G3 is Power On
  gCixTokenSpaceGuid.PcdStateAfterG3|0x01

  # Currently we only support Device Tree boot
  gCixTokenSpaceGuid.PcdSystemTableSelect|0x01

[Components.common]

!if $(DEBUG_ON_UART3) == FALSE
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf
!else
  MdeModulePkg/Universal/SerialDxe/SerialDxe.inf {
    <PcdsFixedAtBuild>
      gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x040D0000
  }
!endif

  Platform/Radxa/Drivers/Logo/LogoDxe.inf
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
