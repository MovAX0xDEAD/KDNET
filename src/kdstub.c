// (c) mov ax, 0xdead

#include <ntddk.h>
#include <ntstatus.h>
#include <process.h>
#include "kdnetshareddata.h"
#include "kdnetextensibility.h"


static void* KdShutdownController_tramp;
static void* KdSetHibernateRange_tramp;
static void* KdGetRxPacket_tramp;
static void* KdReleaseRxPacket_tramp;
static void* KdGetTxPacket_tramp;
static void* KdSendTxPacket_tramp;
static void* KdGetPacketAddress_tramp;
static void* KdGetPacketLength_tramp;
static void* KdInitializeController_tramp;

KDNET_EXTENSIBILITY_IMPORTS_W10 ImportTable_W10;
KDNET_EXTENSIBILITY_EXPORTS_W10 ExportTable_W10;

// win10 KdInitializeLibrary
__declspec(dllimport) NTSTATUS KdInitializeLibrary(
   PKDNET_EXTENSIBILITY_IMPORTS_W10 ImportTable,
   PCHAR LoaderOptions,
   PDEBUG_DEVICE_DESCRIPTOR Device);


void
Dummy()
{
}


VOID
_KDNET_DBGPRINT(
     PCHAR pFmt,
     ...
     )
{
    ;
}


PVOID
_KDNET_MAP_PHYSICAL_MEMORY_64 (
    PHYSICAL_ADDRESS PhysicalAddress,
    ULONG NumberPages,
    BOOLEAN FlushCurrentTLB
    )
{
    return NULL;
}


VOID
_KDNET_UNMAP_VIRTUAL_ADDRESS(
    PVOID VirtualAddress,
    ULONG NumberPages,
    BOOLEAN FlushCurrentTLB
    )
{
    ;
}


#define _KeStallExecutionProcessor ImportTable_W10.StallExecutionProcessor




unsigned __int64 __rdtsc(void);
#pragma intrinsic(__rdtsc)

ULONG64
_KDNET_READ_CYCLE_COUNTER (
    PULONG64 Frequency
    )
{
    __int64 Start, End;

    if (Frequency) {
          Start = __rdtsc();
          _KeStallExecutionProcessor(4);
          End   = __rdtsc();
          *Frequency = ((End - Start) * 1000000i64) >> 2; // x/4
    }

    return __rdtsc();
}


// win81 KdInitializeLibrary
NTSTATUS
_KdInitializeLibrary (
   PKDNET_EXTENSIBILITY_IMPORTS_W81 ImportTable_W81,
   PCHAR LoaderOptions,
   PDEBUG_DEVICE_DESCRIPTOR Device
   )
{
    NTSTATUS    result;
    ULONG       HwContextSize;

    ImportTable_W10.FunctionCount       = KDNET_EXT_IMPORTS_W10;

    ExportTable_W10.FunctionCount       = KDNET_EXT_EXPORTS_W10;
    ImportTable_W10.Exports             = &ExportTable_W10;

    ImportTable_W10.GetPciDataByOffset      = ImportTable_W81->GetPciDataByOffset;
    ImportTable_W10.SetPciDataByOffset      = ImportTable_W81->SetPciDataByOffset;
    ImportTable_W10.GetPhysicalAddress      = ImportTable_W81->GetPhysicalAddress;
    ImportTable_W10.StallExecutionProcessor = ImportTable_W81->StallExecutionProcessor;
    
    ImportTable_W10.ReadRegisterUChar       = ImportTable_W81->ReadRegisterUChar;
    ImportTable_W10.ReadRegisterUShort      = ImportTable_W81->ReadRegisterUShort;
    ImportTable_W10.ReadRegisterULong       = ImportTable_W81->ReadRegisterULong;
    ImportTable_W10.ReadRegisterULong64     = ImportTable_W81->ReadRegisterULong64;

    ImportTable_W10.WriteRegisterUChar      = ImportTable_W81->WriteRegisterUChar;
    ImportTable_W10.WriteRegisterUShort     = ImportTable_W81->WriteRegisterUShort;
    ImportTable_W10.WriteRegisterULong      = ImportTable_W81->WriteRegisterULong;
    ImportTable_W10.WriteRegisterULong64    = ImportTable_W81->WriteRegisterULong64;

    ImportTable_W10.ReadPortUChar           = ImportTable_W81->ReadPortUChar;
    ImportTable_W10.ReadPortUShort          = ImportTable_W81->ReadPortUShort;
    ImportTable_W10.ReadPortULong           = ImportTable_W81->ReadPortULong;
    ImportTable_W10.ReadPortULong64         = ImportTable_W81->ReadPortULong64;

    ImportTable_W10.WritePortUChar          = ImportTable_W81->WritePortUChar;
    ImportTable_W10.WritePortUShort         = ImportTable_W81->WritePortUShort;
    ImportTable_W10.WritePortULong          = ImportTable_W81->WritePortULong;
    ImportTable_W10.WritePortULong64        = ImportTable_W81->WritePortULong64;

    ImportTable_W10.SetHiberRange           = ImportTable_W81->SetHiberRange;
    ImportTable_W10.BugCheckEx              = KeBugCheckEx;                     // ntoskrnl.exe
    ImportTable_W10.MapPhysicalMemory64     = _KDNET_MAP_PHYSICAL_MEMORY_64;
    ImportTable_W10.UnmapVirtualAddress     = _KDNET_UNMAP_VIRTUAL_ADDRESS;
    ImportTable_W10.ReadCycleCounter        = _KDNET_READ_CYCLE_COUNTER;
    ImportTable_W10.KdNetDbgPrintf          = _KDNET_DBGPRINT;

    ImportTable_W10._KdNetErrorStatus       = ImportTable_W81->_KdNetErrorStatus;
    ImportTable_W10._KdNetErrorString       = ImportTable_W81->_KdNetErrorString;
    ImportTable_W10._KdNetHardwareID        = ImportTable_W81->_KdNetHardwareID;

    result = KdInitializeLibrary(&ImportTable_W10, LoaderOptions, Device);

    KdInitializeController_tramp   = ExportTable_W10.KdInitializeController;
    KdShutdownController_tramp     = ExportTable_W10.KdShutdownController;
    KdSetHibernateRange_tramp      = ExportTable_W10.KdSetHibernateRange;
    KdGetRxPacket_tramp            = ExportTable_W10.KdGetRxPacket;
    KdReleaseRxPacket_tramp        = ExportTable_W10.KdReleaseRxPacket;
    KdGetTxPacket_tramp            = ExportTable_W10.KdGetTxPacket;
    KdSendTxPacket_tramp           = ExportTable_W10.KdSendTxPacket;
    KdGetPacketAddress_tramp       = ExportTable_W10.KdGetPacketAddress;
    KdGetPacketLength_tramp        = ExportTable_W10.KdGetPacketLength;

    //Device->Memory.Length = ExportTable_W10.KdGetHardwareContextSize(Device);

    return result;
}


// stubs
__declspec(naked) void KdShutdownController ()
{
    __asm jmp dword ptr [KdShutdownController_tramp];
}

__declspec(naked) void KdSetHibernateRange ()
{
    __asm jmp dword ptr [KdSetHibernateRange_tramp];
}

__declspec(naked) void KdGetRxPacket ()
{
    __asm jmp dword ptr [KdGetRxPacket_tramp];
}

__declspec(naked) void KdReleaseRxPacket ()
{
    __asm jmp dword ptr [KdReleaseRxPacket_tramp];
}

__declspec(naked) void KdGetTxPacket ()
{
    __asm jmp dword ptr [KdGetTxPacket_tramp];
}

__declspec(naked) void KdSendTxPacket ()
{
    __asm jmp dword ptr [KdSendTxPacket_tramp];
}

__declspec(naked) void KdGetPacketAddress ()
{
    __asm jmp dword ptr [KdGetPacketAddress_tramp];
}

__declspec(naked) void KdGetPacketLength ()
{
    __asm jmp dword ptr [KdGetPacketLength_tramp];
}

__declspec(naked) void KdInitializeController ()
{
    __asm jmp dword ptr [KdInitializeController_tramp];
}

/*
unsigned char hex2decimal (char ch) {
 if (ch >= '0' && ch <= '9') {
	return (ch - '0');
 } else if (ch >= 'a' && ch <= 'f') {
	return 10 + (ch - 'a');
 } else if (ch >= 'A' && ch <= 'F') {
	return 10 + (ch - 'A');
 }

 return 0;
}

unsigned long func(const char* ptr) {
    int             i;
    unsigned char   high, low, byte;
    unsigned long   dword = 0;

    for (i=0; i < 8; i=i+2) {
        high  = hex2decimal(ptr[i]);
        low   = hex2decimal(ptr[i+1]);
        byte  = (high << 4) | low; // 0-3*8-80, 2-2*8-86, 4-1*8-10, 6-0-0F
        dword |= (byte << (3 - i/2)*8);
    }

    return dword;
}


void dummy() 
{
    int a = func(NULL);
}
*/