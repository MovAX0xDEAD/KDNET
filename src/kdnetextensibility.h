/*++

Copyright (c) Microsoft Corporation

Module Name:

    kdnetextensibility.h

Abstract:

    Defines the public interface for the kdnet extensibility imports supplied
    by kdnet to the extensibility module as well as the kdnet extensibility
    exports supplied by the extensibility module to kdnet.

Author:

    Joe Ballantyne (joeball) 8-31-2011

--*/

#pragma once

//
// Kdnet Extensibility Model Exports.  There are 2 types of KDNET extensibility
// modules.  Packet based and byte based.  Both of them share the same
// KdInitializeController, KdShutdownController, KdSetHibernateRange,
// and KdDeviceControl routines.  The following are the packet based KDNET 
// extensibility routines.
//

typedef
NTSTATUS
(*KD_INITIALIZE_CONTROLLER) (
    PKDNET_SHARED_DATA KdNet
    );

typedef
VOID
(*KD_SHUTDOWN_CONTROLLER) (
    PVOID Adapter
    );

typedef
VOID
(*KD_SET_HIBERNATE_RANGE) (
    VOID
    );

typedef
NTSTATUS
(*KD_DEVICE_CONTROL) (
    PVOID Adapter,
    ULONG RequestCode,
    PVOID InputBuffer,
    ULONG InputBufferLength,
    PVOID OutputBuffer,
    ULONG OutputBufferLength
    );

typedef
NTSTATUS
(*KD_GET_RX_PACKET) (
    PVOID Adapter,
    PULONG Handle,
    PVOID *Packet,
    PULONG Length
    );

typedef
VOID
(*KD_RELEASE_RX_PACKET) (
    PVOID Adapter,
    ULONG Handle
    );

typedef
NTSTATUS
(*KD_GET_TX_PACKET) (
    PVOID Adapter,
    PULONG Handle
    );

typedef
NTSTATUS
(*KD_SEND_TX_PACKET) (
    PVOID Adapter,
    ULONG Handle,
    ULONG Length
    );

typedef
PVOID
(*KD_GET_PACKET_ADDRESS) (
    PVOID Adapter,
    ULONG Handle
    );

typedef
ULONG
(*KD_GET_PACKET_LENGTH) (
    PVOID Adapter,
    ULONG Handle
    );

typedef
ULONG
(*KD_GET_HARDWARE_CONTEXT_SIZE) (
    PDEBUG_DEVICE_DESCRIPTOR Device
    );

typedef
NTSTATUS
(*DEBUG_SERIAL_OUTPUT_INIT) (
    PDEBUG_DEVICE_DESCRIPTOR pDevice,
    PPHYSICAL_ADDRESS PAddress
    );

typedef
VOID
(*DEBUG_SERIAL_OUTPUT_BYTE)(
    const UCHAR byte
    );


//
// KdNet Serial Extensibility Model.
//
// KdNet supports a byte based extensibility model for serial devices.  KDNET
// itself handles all of the framing.
//
// Note that every serial module must support some form of hardware flow control 
// to prevent overrun of data while KDNET is not polling.
//

typedef
NTSTATUS
(*KD_READ_SERIAL_BYTE) (
    PVOID Adapter,
    PUCHAR Byte
    );

typedef
NTSTATUS
(*KD_WRITE_SERIAL_BYTE) (
    PVOID Adapter,
    const UCHAR Byte
    );

//
// Serial Device Control Requests:
//

//
// SERIAL_QUERY_FLOW_CONTROL_SUPPORTED:
//
// Input: NULL
// Output: BOOLEAN
//
// Sent to the serial device to query whether the device (or the extensibility
// module) provides some mechanism of flow control to prevent the overrun of 
// the local and remote sides of the link based on KDNET's polling.
//
// For serial UARTs, this could correspond to either automatic hardware RTS/CTS 
// based flow control as supported in a 16750+ or it could correspond to the
// extensibility module performing "driver based" RTS/CTS flow control and/or
// appropriate FIFO draining in an interrupt handler.
//
// If a serial extensibility module does not support this request, it is assumed
// that the answer is NO and that the framing layer will attempt to perform
// a manual assertion of control over flow if possible.
//

#define KD_DEVICE_CONTROL_SERIAL_QUERY_FLOW_CONTROL_SUPPORTED 0x00000001

//
// SERIAL_SET_FLOW_CONTROL:
//
// Input: BOOLEAN
// Output: NULL 
//
// Sent to the serial device to enable its flow control as reported through
// SERIAL_QUERY_FLOW_CONTROL_SUPPORTED.  If the former request is supported,
// this one must be.  Note that it is appropriate for a device to fail
// a request to turn this off.
//

#define KD_DEVICE_CONTROL_SERIAL_SET_FLOW_CONTROL 0x00000002

//
// SERIAL_SET_REMOTE_FLOW:
//
// Input: BOOLEAN
// Output: NULL
//
// If a device does not support any means of automatic or driver based flow 
// control, the framing layer will attempt to perform flow control of the
// remote side through this call.
//
// For serial UARTs, this request could correspond directly to setting the
// RTS line to the input value.
//

#define KD_DEVICE_CONTROL_SERIAL_SET_REMOTE_FLOW 0x00000003

#define KDNET_EXT_EXPORTS_W10 13

typedef struct _KDNET_EXTENSIBLITY_EXPORTS_W10
{
    ULONG FunctionCount;
    KD_INITIALIZE_CONTROLLER KdInitializeController;
    KD_SHUTDOWN_CONTROLLER KdShutdownController;
    KD_SET_HIBERNATE_RANGE KdSetHibernateRange;
    KD_GET_RX_PACKET KdGetRxPacket;
    KD_RELEASE_RX_PACKET KdReleaseRxPacket;
    KD_GET_TX_PACKET KdGetTxPacket;
    KD_SEND_TX_PACKET KdSendTxPacket;
    KD_GET_PACKET_ADDRESS KdGetPacketAddress;
    KD_GET_PACKET_LENGTH KdGetPacketLength;
    KD_GET_HARDWARE_CONTEXT_SIZE KdGetHardwareContextSize;
    KD_DEVICE_CONTROL KdDeviceControl;
    KD_READ_SERIAL_BYTE KdReadSerialByte;
    KD_WRITE_SERIAL_BYTE KdWriteSerialByte;
    DEBUG_SERIAL_OUTPUT_INIT DebugSerialOutputInit;
    DEBUG_SERIAL_OUTPUT_BYTE DebugSerialOutputByte;
} KDNET_EXTENSIBILITY_EXPORTS_W10, *PKDNET_EXTENSIBILITY_EXPORTS_W10;

//
// Kdnet Extensibility Model Imports
//

typedef
PHYSICAL_ADDRESS
(*KDNET_GET_PHYSICAL_ADDRESS) (
    PVOID Va
    );

typedef
VOID
(*KDNET_STALL_EXECUTION_PROCESSOR) (
    ULONG Microseconds
    );

typedef
UCHAR
(*KDNET_READ_REGISTER_UCHAR) (
    PUCHAR Register
    );

typedef
USHORT
(*KDNET_READ_REGISTER_USHORT) (
    PUSHORT Register
    );

typedef
ULONG
(*KDNET_READ_REGISTER_ULONG) (
    PULONG Register
    );

typedef
ULONG64
(*KDNET_READ_REGISTER_ULONG64) (
    PULONG64 Register
    );

typedef
VOID
(*KDNET_WRITE_REGISTER_UCHAR) (
    PUCHAR Register,
    UCHAR Value
    );

typedef
VOID
(*KDNET_WRITE_REGISTER_USHORT) (
    PUSHORT Register,
    USHORT Value
    );

typedef
VOID
(*KDNET_WRITE_REGISTER_ULONG) (
    PULONG Register,
    ULONG Value
    );

typedef
VOID
(*KDNET_WRITE_REGISTER_ULONG64) (
    PULONG64 Register,
    ULONG64 Value
    );

typedef
UCHAR
(*KDNET_READ_PORT_UCHAR) (
    PUCHAR Port
    );

typedef
USHORT
(*KDNET_READ_PORT_USHORT) (
    PUSHORT Port
    );

typedef
ULONG
(*KDNET_READ_PORT_ULONG) (
    PULONG Port
    );

typedef
ULONG
(*KDNET_READ_PORT_ULONG64) (
    PULONG64 Port
    );

typedef
VOID
(*KDNET_WRITE_PORT_UCHAR) (
    PUCHAR Port,
    UCHAR Value
    );

typedef
VOID
(*KDNET_WRITE_PORT_USHORT) (
    PUSHORT Port,
    USHORT Value
    );

typedef
VOID
(*KDNET_WRITE_PORT_ULONG) (
    PULONG Port,
    ULONG Value
    );

typedef
VOID
(*KDNET_WRITE_PORT_ULONG64) (
    PULONG Port,
    ULONG64 Value
    );

typedef
ULONG
(*KDNET_GET_PCI_DATA_BY_OFFSET) (
    ULONG BusNumber,
    ULONG SlotNumber,
    __out_bcount(Length) PVOID Buffer,
    ULONG Offset,
    ULONG Length
    );

typedef
ULONG
(*KDNET_SET_PCI_DATA_BY_OFFSET) (
    ULONG BusNumber,
    ULONG SlotNumber,
    PVOID Buffer,
    ULONG Offset,
    ULONG Length
    );

typedef
VOID
(*KDNET_SET_DEBUGGER_NOT_PRESENT) (
    BOOLEAN NotPresent
    );

typedef
VOID
(*KDNET_SET_HIBER_RANGE) (
    PVOID MemoryMap,
    ULONG     Flags,
    PVOID     Address,
    ULONG_PTR Length,
    ULONG     Tag
    );

typedef
VOID
(*KDNET_BUGCHECK_EX) (
    ULONG BugCheckCode,
    ULONG_PTR BugCheckParameter1,
    ULONG_PTR BugCheckParameter2,
    ULONG_PTR BugCheckParameter3,
    ULONG_PTR BugCheckParameter4
    );

typedef
PVOID
(*KDNET_MAP_PHYSICAL_MEMORY_64) (
    PHYSICAL_ADDRESS PhysicalAddress,
    ULONG NumberPages,
    BOOLEAN FlushCurrentTLB
    );

typedef
VOID
(*KDNET_UNMAP_VIRTUAL_ADDRESS)(
    PVOID VirtualAddress,
    ULONG NumberPages,
    BOOLEAN FlushCurrentTLB
    );

typedef
ULONG64
(*KDNET_READ_CYCLE_COUNTER) (
    PULONG64 Frequency
);

typedef
VOID
(*KDNET_DBGPRINT)(
     PCHAR pFmt,
     ...
   );

#define KDNET_EXT_IMPORTS_W10 30

typedef struct _KDNET_EXTENSIBLITY_IMPORTS_W10
{
    ULONG FunctionCount;

    //
    // The structure pointed to by the following pointer must be properly
    // initialized by the KDNET extensibility module in its KdInitializeLibrary
    // routine.  After initialization it should contain pointers to all of the
    // routines exported by the KDNET extensibility module other than
    // KdInitializeLibrary.
    //

    PKDNET_EXTENSIBILITY_EXPORTS_W10 Exports;

    //
    // The following routines are exported from KDNET for use by the KDNET
    // extensibility module.  KDNET extensibility modules cannot have any
    // imports, and thus the only externally available routines are the ones
    // passed in this structure.  These fields are mapped by the #defines that
    // follow below to the standard routine names.
    //

    KDNET_GET_PCI_DATA_BY_OFFSET GetPciDataByOffset;
    KDNET_SET_PCI_DATA_BY_OFFSET SetPciDataByOffset;
    KDNET_GET_PHYSICAL_ADDRESS GetPhysicalAddress;
    KDNET_STALL_EXECUTION_PROCESSOR StallExecutionProcessor;
    KDNET_READ_REGISTER_UCHAR ReadRegisterUChar;
    KDNET_READ_REGISTER_USHORT ReadRegisterUShort;
    KDNET_READ_REGISTER_ULONG ReadRegisterULong;
    KDNET_READ_REGISTER_ULONG64 ReadRegisterULong64;
    KDNET_WRITE_REGISTER_UCHAR WriteRegisterUChar;
    KDNET_WRITE_REGISTER_USHORT WriteRegisterUShort;
    KDNET_WRITE_REGISTER_ULONG WriteRegisterULong;
    KDNET_WRITE_REGISTER_ULONG64 WriteRegisterULong64;
    KDNET_READ_PORT_UCHAR ReadPortUChar;
    KDNET_READ_PORT_USHORT ReadPortUShort;
    KDNET_READ_PORT_ULONG ReadPortULong;
    KDNET_READ_PORT_ULONG64 ReadPortULong64;
    KDNET_WRITE_PORT_UCHAR WritePortUChar;
    KDNET_WRITE_PORT_USHORT WritePortUShort;
    KDNET_WRITE_PORT_ULONG WritePortULong;
    KDNET_WRITE_PORT_ULONG64 WritePortULong64;
    KDNET_SET_HIBER_RANGE SetHiberRange;
    KDNET_BUGCHECK_EX BugCheckEx;
    KDNET_MAP_PHYSICAL_MEMORY_64 MapPhysicalMemory64;
    KDNET_UNMAP_VIRTUAL_ADDRESS UnmapVirtualAddress;
    KDNET_READ_CYCLE_COUNTER ReadCycleCounter;
    KDNET_DBGPRINT KdNetDbgPrintf;

    //
    // The following pointers allow KDNET extensibility modules to write error
    // information back to KDNET so that it can be reported in the registry
    // and by telemetry.  These fields are mapped by the #defines that follow
    // below to the normal variable names.
    //

    NTSTATUS *_KdNetErrorStatus;
    PWCHAR *_KdNetErrorString;
    PULONG _KdNetHardwareID;
} KDNET_EXTENSIBILITY_IMPORTS_W10, *PKDNET_EXTENSIBILITY_IMPORTS_W10;


#define KDNET_EXT_IMPORTS_W81 24

typedef struct _KDNET_EXTENSIBLITY_IMPORTS_W81
{
    ULONG FunctionCount;

    //
    // The structure pointed to by the following pointer must be properly
    // initialized by the KDNET extensibility module in its KdInitializeLibrary
    // routine.  After initialization it should contain pointers to all of the
    // routines exported by the KDNET extensibility module other than
    // KdInitializeLibrary.
    //

    //PKDNET_EXTENSIBILITY_EXPORTS Exports;

    //
    // The following routines are exported from KDNET for use by the KDNET
    // extensibility module.  KDNET extensibility modules cannot have any
    // imports, and thus the only externally available routines are the ones
    // passed in this structure.  These fields are mapped by the #defines that
    // follow below to the standard routine names.
    //

    KDNET_GET_PCI_DATA_BY_OFFSET GetPciDataByOffset;
    KDNET_SET_PCI_DATA_BY_OFFSET SetPciDataByOffset;
    KDNET_GET_PHYSICAL_ADDRESS GetPhysicalAddress;
    KDNET_STALL_EXECUTION_PROCESSOR StallExecutionProcessor;
    KDNET_READ_REGISTER_UCHAR ReadRegisterUChar;
    KDNET_READ_REGISTER_USHORT ReadRegisterUShort;
    KDNET_READ_REGISTER_ULONG ReadRegisterULong;
    KDNET_READ_REGISTER_ULONG64 ReadRegisterULong64;
    KDNET_WRITE_REGISTER_UCHAR WriteRegisterUChar;
    KDNET_WRITE_REGISTER_USHORT WriteRegisterUShort;
    KDNET_WRITE_REGISTER_ULONG WriteRegisterULong;
    KDNET_WRITE_REGISTER_ULONG64 WriteRegisterULong64;
    KDNET_READ_PORT_UCHAR ReadPortUChar;
    KDNET_READ_PORT_USHORT ReadPortUShort;
    KDNET_READ_PORT_ULONG ReadPortULong;
    KDNET_READ_PORT_ULONG64 ReadPortULong64;
    KDNET_WRITE_PORT_UCHAR WritePortUChar;
    KDNET_WRITE_PORT_USHORT WritePortUShort;
    KDNET_WRITE_PORT_ULONG WritePortULong;
    KDNET_WRITE_PORT_ULONG64 WritePortULong64;
    KDNET_SET_HIBER_RANGE SetHiberRange;
    //KDNET_BUGCHECK_EX BugCheckEx;
    //KDNET_MAP_PHYSICAL_MEMORY_64 MapPhysicalMemory64;
    //KDNET_UNMAP_VIRTUAL_ADDRESS UnmapVirtualAddress;
    //KDNET_READ_CYCLE_COUNTER ReadCycleCounter;
    //KDNET_DBGPRINT KdNetDbgPrintf;

    //
    // The following pointers allow KDNET extensibility modules to write error
    // information back to KDNET so that it can be reported in the registry
    // and by telemetry.  These fields are mapped by the #defines that follow
    // below to the normal variable names.
    //

    NTSTATUS *_KdNetErrorStatus;
    PWCHAR *_KdNetErrorString;
    PULONG _KdNetHardwareID;
} KDNET_EXTENSIBILITY_IMPORTS_W81, *PKDNET_EXTENSIBILITY_IMPORTS_W81;

/*
#if !defined(_KDNET_INTERNAL_)

extern PKDNET_EXTENSIBILITY_IMPORTS_W10 KdNetExtensibilityImports;

#define KdGetPhysicalAddress KdNetExtensibilityImports->GetPhysicalAddress 
#define READ_REGISTER_UCHAR KdNetExtensibilityImports->ReadRegisterUChar
#define READ_REGISTER_USHORT KdNetExtensibilityImports->ReadRegisterUShort
#define READ_REGISTER_ULONG KdNetExtensibilityImports->ReadRegisterULong
#define WRITE_REGISTER_UCHAR KdNetExtensibilityImports->WriteRegisterUChar
#define WRITE_REGISTER_USHORT KdNetExtensibilityImports->WriteRegisterUShort
#define WRITE_REGISTER_ULONG KdNetExtensibilityImports->WriteRegisterULong
#define WRITE_PORT_UCHAR KdNetExtensibilityImports->WritePortUChar
#define WRITE_PORT_USHORT KdNetExtensibilityImports->WritePortUShort
#define WRITE_PORT_ULONG KdNetExtensibilityImports->WritePortULong
#define READ_PORT_UCHAR KdNetExtensibilityImports->ReadPortUChar
#define READ_PORT_USHORT KdNetExtensibilityImports->ReadPortUShort
#define READ_PORT_ULONG KdNetExtensibilityImports->ReadPortULong

#undef READ_REGISTER_ULONG64
#undef WRITE_REGISTER_ULONG64

#if defined(_WIN64)

#define READ_REGISTER_ULONG64 KdNetExtensibilityImports->ReadRegisterULong64
#define WRITE_REGISTER_ULONG64 KdNetExtensibilityImports->WriteRegisterULong64

#endif

#undef KdGetPciDataByOffset
#define KdGetPciDataByOffset KdNetExtensibilityImports->GetPciDataByOffset

#undef KdSetPciDataByOffset
#define KdSetPciDataByOffset KdNetExtensibilityImports->SetPciDataByOffset

#undef KeStallExecutionProcessor
#define KeStallExecutionProcessor KdNetExtensibilityImports->StallExecutionProcessor

#undef PoSetHiberRange
#define PoSetHiberRange KdNetExtensibilityImports->SetHiberRange

#undef _KeBugCheckEx
#define _KeBugCheckEx KdNetExtensibilityImports->BugCheckEx

#undef KdMapPhysicalMemory64
#define KdMapPhysicalMemory64 KdNetExtensibilityImports->MapPhysicalMemory64

#undef KdUnmapVirtualAddress
#define KdUnmapVirtualAddress KdNetExtensibilityImports->UnmapVirtualAddress

#undef KdReadCycleCounter
#define KdReadCycleCounter KdNetExtensibilityImports->ReadCycleCounter

#undef KdDbgPrintf
#define KdDbgPrintf KdNetExtensibilityImports->KdNetDbgPrintf

#define KdNetErrorStatus *(KdNetExtensibilityImports->KdNetErrorStatus)
#define KdNetErrorString *(KdNetExtensibilityImports->KdNetErrorString)
#define KdNetHardwareID *(KdNetExtensibilityImports->KdNetHardwareID)

//
// For ARM platform force strictly aligned copy as unaligned access 
// isn't supported by hardware on stricty ordered memory used for
// DMA transfers (as it is today)
//

#if (_ARM_) || (_ARM64_)
#undef RtlCopyMemory
#define RtlCopyMemory(d, s, l)  _memcpy_strict_align(d, s, l)
#endif

#else

#if !defined(_KDNETEXTENSIBILITY_C_)

extern PKDNET_EXTENSIBILITY_EXPORTS KdNetExtensibilityExports;

#define KdInitializeController KdNetExtensibilityExports->KdInitializeController
#define KdShutdownController KdNetExtensibilityExports->KdShutdownController
#define KdGetRxPacket KdNetExtensibilityExports->KdGetRxPacket
#define KdReleaseRxPacket KdNetExtensibilityExports->KdReleaseRxPacket
#define KdGetTxPacket KdNetExtensibilityExports->KdGetTxPacket
#define KdSendTxPacket KdNetExtensibilityExports->KdSendTxPacket
#define KdGetPacketAddress KdNetExtensibilityExports->KdGetPacketAddress
#define KdGetPacketLength KdNetExtensibilityExports->KdGetPacketLength
#define KdGetHardwareContextSize KdNetExtensibilityExports->KdGetHardwareContextSize
#define KdSetHibernateRange KdNetExtensibilityExports->KdSetHibernateRange
#define KdReadSerialByte KdNetExtensibilityExports->KdReadSerialByte
#define KdWriteSerialByte KdNetExtensibilityExports->KdWriteSerialByte
#define KdDeviceControl KdNetExtensibilityExports->KdDeviceControl

#endif

#endif
*/

