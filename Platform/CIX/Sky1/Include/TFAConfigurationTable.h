/** @file

  Copyright 2024 Cix Technology Group Co., Ltd. All Rights Reserved

**/
/*
 * Copyright 2024 - Cix Technology Group Co., Ltd. All Rights Reserved.
 */
#ifndef __CIX_TRUSTZONE_CONFIG_H__
#define __CIX_TRUSTZONE_CONFIG_H__

typedef UINT8   uint8_t;
typedef UINT16  uint16_t;
typedef UINT32  uint32_t;
typedef UINT64  uint64_t;
typedef INT16   int16_t;

/* TrustZone configuration constants */
//#define TRUSTZONE_CONFIG_SIGNATURE 0x545A4346 /* "TZCF" */
#define TABLE_SIGNATURE( x, y, m, n )                                              \
    ( uint32_t )( ( 0xFF & (uint8_t)( x ) ) | ( ( 0xFF & (uint8_t)( y ) ) << 8 ) | \
                  ( ( 0xFF & (uint8_t)( m ) ) << 16 ) | ( ( 0xFF & (uint8_t)( n ) ) << 24 ) )
#define TABLE_ALIGN_U32(_size)            (((_size) + 0x3) & ~0x3)
#define TABLE_ALIGN_U32_PADDING(_size)    (TABLE_ALIGN_U32(_size) - (_size))
#define TRUSTZONE_CONFIG_SIGNATURE TABLE_SIGNATURE('T', 'Z', 'C', 'F')
#define TRUSTZONE_CONFIG_VERSION_MAJOR 1
#define TRUSTZONE_CONFIG_VERSION_MINOR 0

#pragma pack(push, 1)
/* Secure Storage Type */
typedef enum {
    SEC_STORAGE_TYPE_REE_FS    = 0x00,  // REE File System
    SEC_STORAGE_TYPE_NOR_FS    = 0x01,  // NOR Flash File System
    SEC_STORAGE_TYPE_RMPB_FS   = 0x02,  // RMPB (Replay Protected Memory Block) File System
    SEC_STORAGE_TYPE_INVALID   = 0xFF
} SecureStorageType;

/**
 * @brief Configuration table header structure
 */
typedef struct {
    uint8_t versionId;    // Version ID, fixed as TRUSTZONE_CONFIG_versionMAJOR (0x01)
    uint8_t length;        // Total length of configuration table (including Header), unit: bytes
} TrustZoneConfigHeader;

/**
 * @brief Configuration table data structure
 */
typedef struct {
    uint8_t  SPEFeatureEn;        // SPE feature enable (0x00/0xFF=disabled, 0x01=enabled)
    uint8_t  SecureStorageType;   // Secure storage type (values from SecureStorageType_e)
    uint32_t SecureStorageAddr;   // Secure storage address
    uint32_t SecureStorageSize;   // Secure storage size, unit: bytes
} TrustZoneConfigData;

/**
 * @brief Trustzone core configuration structure
 */
typedef struct {
    TrustZoneConfigHeader header;  // Configuration table header
    TrustZoneConfigData   data;    // Configuration table data
} TrustZoneExportConfig;

/**
 * @brief Configuration structure with CRC checks
 */
typedef struct {
    uint32_t versionMajor : 16;  // Major version number (TRUSTZONE_CONFIG_versionMAJOR)
    uint32_t versionMinor : 16;  // Minor version number (TRUSTZONE_CONFIG_versionMINOR)
    uint32_t timestamp;           // Configuration generation timestamp
    uint32_t length;              // Total configuration length (including CRC segment)
    uint32_t signature;           // Configuration signature (TRUSTZONE_CONFIG_SIGNATURE)
    uint32_t crc1;                // Primary CRC check (covers version~signature fields)
    uint32_t crc2;                // Secondary CRC check (covers TrustZoneExportConfig field)
    TrustZoneExportConfig config;  // Core configuration content
    uint8_t padding[3];           // 32-bit alignment padding
} CixTrustZoneConfig;

#pragma pack(pop)

#endif /* __CIX_TRUSTZONE_CONFIG_H__ */