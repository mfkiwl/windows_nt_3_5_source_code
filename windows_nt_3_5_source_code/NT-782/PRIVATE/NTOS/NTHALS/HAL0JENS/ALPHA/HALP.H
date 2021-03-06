/*++ BUILD Version: 0003    // Increment this if a change has global effects

Copyright (c) 1991  Microsoft Corporation
Copyright (c) 1992  Digital Equipment Corporation

Module Name:

    halp.h

Abstract:

    This header file defines the private Hardware Architecture Layer (HAL)
    interfaces.

Author:

    David N. Cutler (davec) 25-Apr-1991
    Miche Baker-Harvey (miche) 22-Apr-1992


Revision History:

    09-Jul-1992 Jeff McLeman (mcleman)
      If processor is an Alpha, include XXHALP.C for Alpha.

--*/

#ifndef _HALP_
#define _HALP_
#include "nthal.h"

//
// Define function prototypes.
//

BOOLEAN
HalpCalibrateStall (
    VOID
    );

VOID
HalpClockInterrupt (
    VOID
    );

BOOLEAN
HalpCreateDmaStructures (
    VOID
    );

BOOLEAN
HalpDmaDispatch(
    IN PKINTERRUPT Interrupt,
    IN PVOID ServiceContext
    );

BOOLEAN
HalpInitializeDisplay (
    IN PLOADER_PARAMETER_BLOCK LoaderBlock
    );

BOOLEAN
HalpInitializeInterrupts (
    VOID
    );

BOOLEAN
HalpMapFixedTbEntries (
    VOID
    );

BOOLEAN
HalpMapIoSpace (
    VOID
    );

VOID
HalpProfileInterrupt (
    PKSERVICE_ROUTINE InterruptRoutine,
    PVOID ServiceContext,
    PKTRAP_FRAME TrapFrame
    );

PVOID
HalpMapPhysicalMemory(
    IN PVOID PhysicalAddress,
    IN ULONG NumberOfPages
    );

PVOID
HalpRemapVirtualAddress(
    IN PVOID VirtualAddress,
    IN PVOID PhysicalAddress
    );

ULONG
HalpAllocPhysicalMemory(
    IN PLOADER_PARAMETER_BLOCK LoaderBlock,
    IN ULONG MaxPhysicalAddress,
    IN ULONG NumberOfPages,
    IN BOOLEAN bAlignOn64k
    );

VOID
HalpProgramIntervalTimer(
    IN ULONG RateSelect
    );

VOID
HalpStallInterrupt (
    VOID
    );

VOID
HalpVideoReboot(
    VOID
    );

#if defined(JENSEN)

#include "jxhalp.h"

#endif

#if defined(FLAMINGO)

#include "fxhalp.h"

#endif

//
// Include alpha processor interfaces
//

#include "xxhalp.h"

//
// Define external references.
//

extern ULONG HalpCurrentTimeIncrement;
extern ULONG HalpNextRateSelect;
extern ULONG HalpNextTimeIncrement;
extern ULONG HalpNewTimeIncrement;

extern ULONG HalpClockFrequency;
extern ULONG HalpClockMegaHertz;

extern ULONG HalpProfileCountRate;

extern PADAPTER_OBJECT MasterAdapterObject;

extern BOOLEAN LessThan16Mb;

//
// Map buffer prameters.  These are initialized in HalInitSystem
//

extern PHYSICAL_ADDRESS HalpMapBufferPhysicalAddress;
extern ULONG HalpMapBufferSize;

extern ULONG HalpBusType;
extern ULONG HalpCpuType;

#endif // _HALP_
