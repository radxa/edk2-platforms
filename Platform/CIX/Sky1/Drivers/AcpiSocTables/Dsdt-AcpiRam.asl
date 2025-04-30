/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Name (GNVA, 0xFFFFFFFF)  // System GPNV Base address
Name (GNVL, 0xAA55)      // System GPNV Length

Method(GETV,1,Serialized)
{
    if(LGreaterEqual(Arg0, ARV_VARIABLE_TABLE_TOTAL_SIZE)){
        Return (0)  //Error
    }

    Add(Arg0,GNVA,Local0)
    OperationRegion(GPNV,SystemMemory,Local0,1)
    Field(GPNV,ByteAcc,NoLock,Preserve)
    {
        VARV,8
    }
    ToInteger(VARV,Local0)
    Return (Local0)
}
/*
 * Memory valid check
 * Arg0: Power domain group_id
 */
Method(MVCK, 1, Serialized)
{
    OperationRegion(S5R1,SystemMemory,SW_USED_REG1, 0x04)
    Field (S5R1, DWordAcc, NoLock, Preserve) {
        MSK0, 32,
    }
    Local0 = Arg0
    Local1 = MSK0
    Local1 = (Local1 >> Local0) & 0x01
    printf ("ACPI debug:arg0=%o, MVCK.valid = %o\n",Arg0, Local1)
    return (Local1)
}

/* Do memory repair after Power On
 *
 * Arg0: MEM_REPAIR_EN
 * Arg1: Power domain group_id
 * Arg2: Power domain rcsu_addr
 * Arg3: Mem repair mem_index
 */
Method(DMRP, 4, Serialized)
{
    printf ("ACPI debug: Arg0:Arg1:Arg2:Arg3 = %o:%o:%o:%o\n", Arg0, Arg1, Arg2, Arg3)
    if(Arg0 && \_SB.MVCK(Arg1)) {
        OperationRegion(PDRG, SystemMemory, Arg2, 0x20)
        Field (PDRG, DWordAcc, NoLock, Preserve) {
            offset (0x10),
            PASS, 32,
            ENBL, 32,
            BUSY, 32,
        }
        Local0 = MEMORY_REAPIR_CNT

        Local1 = BUSY
        Local1 = (Local1 >> 16) & 0xffff
        while (Local1 != 0x0 && Local0 !=0)
        {
            Local0--
            if (Local0 == 0x0) {
                printf ("Do memory busy, status = %o!\n", Local1)
            }
            Local1 = BUSY
            Local1 = (Local1 >> 16) & 0xffff
        }

        ENBL = Arg3
        printf ("group_en = 0x%o!\n", ENBL)

        Local1 = PASS
        Local1 = (Local1  >> 0x1) & 0x3
        while (Local1 != 0x3 && Local0 !=0)
        {
            Local0--
            if (Local0 == 0x0) {
                printf ("Done and pass failed, status = %o!\n", Local1)
            }
            Local1 = PASS
            Local1 = (Local1  >> 0x1) & 0x3
        }

        ENBL = 0x0
        printf("group_en = 0x%o!\n", ENBL)

        return (ENBL)
    }
    return (0)
}
