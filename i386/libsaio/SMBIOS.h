/*
 * Copyright (c) 1998-2006 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

/* This file is a stripped-down version of the one found in the AppleSMBIOS project.
 * Changes:
 * - Don't use pragma pack but instead use GCC's packed attribute
 */

#ifndef _LIBSAIO_SMBIOS_H
#define _LIBSAIO_SMBIOS_H

/*
 * Based on System Management BIOS Reference Specification v2.5
 */

typedef uint8_t		SMBString;
typedef uint8_t		SMBByte;
typedef uint16_t	SMBWord;
typedef uint32_t	SMBDWord;
typedef uint64_t	SMBQWord;

struct DMIHeader {
	SMBByte			type;
	SMBByte			length;
	SMBWord			handle;
} __attribute__((packed));

struct DMIEntryPoint {
	SMBByte			anchor[5];
	SMBByte			checksum;
	SMBWord			tableLength;
	SMBDWord		tableAddress;
	SMBWord			structureCount;
	SMBByte			bcdRevision;
} __attribute__((packed));

struct SMBEntryPoint {
	SMBByte			anchor[4];
	SMBByte			checksum;
	SMBByte			entryPointLength;
	SMBByte			majorVersion;
	SMBByte			minorVersion;
	SMBWord			maxStructureSize;
	SMBByte			entryPointRevision;
	SMBByte			formattedArea[5];
	struct DMIEntryPoint	dmi;
} __attribute__((packed));

//
// BIOS Information (Type 0)
//
struct SMBBIOSInformation {
    struct DMIHeader	dmiHeader;               // Type 0
    SMBString  vendor;              // BIOS vendor name
    SMBString  version;             // BIOS version
    SMBWord    startSegment;        // BIOS segment start
    SMBString  releaseDate;         // BIOS release date
    SMBByte    romSize;             // (n); 64K * (n+1) bytes
    SMBQWord   characteristics;     // supported BIOS functions
	SMBByte    extChars[2];			//BIOS Characteristics Extension Bytes
};

//BIOS Characteristics Extension Bits
/*
 Characteristics Ext1	0x87
 Bit0	ACPI supported - 1 (Yes)
 Bit1	USB Legacy is supported - 1 (Yes)
 Bit2	AGP is supported - 1 (Yes)
 Bit3	I2O boot is supported - 0 (No)
 Bit4	LS-120 boot is supported - 0 (No)
 Bit5	ATAPI ZIP Drive boot is supported - 0 (No)
 Bit6	1394 boot is supported - 0 (No)
 Bit7	Smart Battery supported - 1 (Yes)
 Characteristics Ext2	0x07
 Bit0	BIOS Boot Specification supported - 1 (Yes)
 Bit1	Function key-initiated Network Service boot supported - 1 (Yes)
 Bit2	Enable Targeted Content Distribution - 1 (Yes)
 */

//
// System Information (Type 1)
//

struct DMISystemInformation {
    // 2.0+ spec (8 bytes)
    struct DMIHeader	dmiHeader;              // Type 1
    SMBString  manufacturer;
    SMBString  productName;
    SMBString  version;
    SMBString  serialNumber;
    // 2.1+ spec (25 bytes)
    SMBByte    uuid[16];            // can be all 0 or all 1's
    SMBByte    wakeupReason;        // reason for system wakeup
};

//
// Base Board (Type 2)
//

struct DMIBaseBoard {
    struct DMIHeader	dmiHeader;              // Type 2
    SMBString	manufacturer;
    SMBString	product;
    SMBString	version;
    SMBString	serialNumber;
    SMBString	assetTagNumber;
    SMBByte		featureFlags;
    SMBString	locationInChassis;
    SMBWord		chassisHandle;
    SMBByte		boardType;
    SMBByte		numberOfContainedHandles;
	// 0 - 255 contained handles go here but we do not include
	// them in our structure. Be careful to use numberOfContainedHandles
	// times sizeof(SMBWord) when computing the actual record size,
	// if you need it.
};

// Values for boardType in Type 2 records
enum {
    kSMBBaseBoardUnknown				= 0x01,
    kSMBBaseBoardOther					= 0x02,
    kSMBBaseBoardServerBlade			= 0x03,
    kSMBBaseBoardConnectivitySwitch		= 0x04,
    kSMBBaseBoardSystemMgmtModule		= 0x05,
    kSMBBaseBoardProcessorModule		= 0x06,
    kSMBBaseBoardIOModule				= 0x07,
    kSMBBaseBoardMemoryModule			= 0x08,
    kSMBBaseBoardDaughter				= 0x09,
    kSMBBaseBoardMotherboard			= 0x0A,
    kSMBBaseBoardProcessorMemoryModule	= 0x0B,
    kSMBBaseBoardProcessorIOModule		= 0x0C,
    kSMBBaseBoardInterconnect			= 0x0D,
};


//
// System Enclosure (Type 3)
//

struct DMISystemEnclosure {
    struct DMIHeader	dmiHeader;             // Type 3
    SMBString  manufacturer;
    SMBByte    type;
    SMBString  version;
    SMBString  serialNumber;
    SMBString  assetTagNumber;
    SMBByte    bootupState;
    SMBByte    powerSupplyState;
    SMBByte    thermalState;
    SMBByte    securityStatus;
    SMBDWord   oemDefined;
};

//
// Processor Information (Type 4)
//

struct DMIProcessorInformation {
    // 2.0+ spec (26 bytes)
    struct DMIHeader	dmiHeader;              // Type 4
    SMBString  socketDesignation;
    SMBByte    processorType;       // CPU = 3
    SMBByte    processorFamily;     // processor family enum
    SMBString  manufacturer;
    SMBQWord   processorID;         // based on CPUID
    SMBString  processorVersion;
    SMBByte    voltage;             // bit7 cleared indicate legacy mode
    SMBWord    externalClock;       // external clock in MHz
    SMBWord    maximumClock;        // max internal clock in MHz
    SMBWord    currentClock;        // current internal clock in MHz
    SMBByte    status;
    SMBByte    processorUpgrade;    // processor upgrade enum
    // 2.1+ spec (32 bytes)
    SMBWord    L1CacheHandle;
    SMBWord    L2CacheHandle;
    SMBWord    L3CacheHandle;
    // 2.3+ spec (35 bytes)
    SMBString  serialNumber;
    SMBString  assetTag;
    SMBString  partNumber;
} __attribute__((packed));

#define kSMBProcessorInformationMinSize     26



struct DMIMemoryControllerInfo {/* 3.3.6 Memory Controller Information (Type 5) */
	struct DMIHeader	dmiHeader;
	SMBByte			errorDetectingMethod;
	SMBByte			errorCorrectingCapability;
	SMBByte			supportedInterleave;
	SMBByte			currentInterleave;
	SMBByte			maxMemoryModuleSize;
	SMBWord			supportedSpeeds;
	SMBWord			supportedMemoryTypes;
	SMBByte			memoryModuleVoltage;
	SMBByte			numberOfMemorySlots;
} __attribute__((packed));

struct DMIMemoryModuleInfo {	/* 3.3.7 Memory Module Information (Type 6) */
	struct DMIHeader	dmiHeader;
	SMBByte			socketDesignation;
	SMBByte			bankConnections;
	SMBByte			currentSpeed;
	SMBWord			currentMemoryType;
	SMBByte			installedSize;
	SMBByte			enabledSize;
	SMBByte			errorStatus;
} __attribute__((packed));

struct DMIPhysicalMemoryArray {	/* 3.3.17 Physical Memory Array (Type 16) */
	struct DMIHeader	dmiHeader;
	SMBByte			location;
	SMBByte			use;
	SMBByte			memoryCorrectionError;
	SMBDWord		maximumCapacity;
	SMBWord			memoryErrorInformationHandle;
	SMBWord			numberOfMemoryDevices;
} __attribute__((packed));

struct DMIMemoryDevice {	/* 3.3.18 Memory Device (Type 17) */
	struct DMIHeader	dmiHeader;
	SMBWord			physicalMemoryArrayHandle;
	SMBWord			memoryErrorInformationHandle;
	SMBWord			totalWidth;
	SMBWord			dataWidth;
	SMBWord			size;
	SMBByte			formFactor;
	SMBByte			deviceSet;
	SMBByte			deviceLocator;
	SMBByte			bankLocator;
	SMBByte			memoryType;
	SMBWord			typeDetail;
    // 2.3+ spec (27 bytes)
    SMBWord    memorySpeed;         // speed of device in MHz (0 for unknown)
    SMBString  manufacturer;
    SMBString  serialNumber;
    SMBString  assetTag;
    SMBString  partNumber;
} __attribute__((packed));

#if UNUSED
static const char *
SMBMemoryDeviceTypes[] =
{
    "RAM",          /* 00h  Undefined */
    "RAM",          /* 01h  Other */
    "RAM",          /* 02h  Unknown */
    "DRAM",         /* 03h  DRAM */
    "EDRAM",        /* 04h  EDRAM */
    "VRAM",         /* 05h  VRAM */
    "SRAM",         /* 06h  SRAM */
    "RAM",          /* 07h  RAM */
    "ROM",          /* 08h  ROM */
    "FLASH",        /* 09h  FLASH */
    "EEPROM",       /* 0Ah  EEPROM */
    "FEPROM",       /* 0Bh  FEPROM */
    "EPROM",        /* 0Ch  EPROM */
    "CDRAM",        /* 0Dh  CDRAM */
    "3DRAM",        /* 0Eh  3DRAM */
    "SDRAM",        /* 0Fh  SDRAM */
    "SGRAM",        /* 10h  SGRAM */
    "RDRAM",        /* 11h  RDRAM */
    "DDR SDRAM",    /* 12h  DDR */
    "DDR2 SDRAM",   /* 13h  DDR2 */
    "DDR2 FB-DIMM", /* 14h  DDR2 FB-DIMM */
    "RAM",			/* 15h  unused */
    "RAM",			/* 16h  unused */
    "RAM",			/* 17h  unused */
    "DDR3",			/* 18h  DDR3, chosen in [5776134] */
};
#endif

#endif /* !_LIBSAIO_SMBIOS_H */
