/*++

Copyright (c) 1990  Microsoft Corporation
Copyright (c) 1992, 1993  Digital Equipment Corporation

Module Name:

    ebintsup.c

Abstract:

    The module provides the interrupt support for EB66/Mustang, and 
    Avanti systems.

Author:

    Eric Rehm (DEC) 29-December-1993

Revision History:


--*/


#include "halp.h"
#include "eisa.h"
#include "ebsgdma.h"
#include "avantdef.h"
#include "pcrtc.h"
#include "pintolin.h"

//
// Reference the boolean variable for SIO-II vs. SIO, i.e., the
// Avanti pass 2 MLB, and the boolean for the APECS pass 2 on the
// Avanti pass 2 daughter card.
//
extern BOOLEAN SioCStep;
extern BOOLEAN ApecsPass2;

//
// Declare the interrupt handler for the PCI and ISA bus.
//
  
BOOLEAN
HalpSioDispatch(
    VOID
    );

//
// The following is the interrupt object used for DMA controller interrupts.
// DMA controller interrupts occur when a memory parity error occurs or a
// programming error occurs to the DMA controller.
//

KINTERRUPT HalpEisaNmiInterrupt;

//
// The following function initializes NMI handling.
//

VOID
HalpInitializeNMI( 
    VOID 
    );

//
// The following function is called when an ISA NMI occurs.
//

BOOLEAN
HalHandleNMI(
    IN PKINTERRUPT Interrupt,
    IN PVOID ServiceContext
    );


BOOLEAN
HalpInitializePCIInterrupts (
    VOID
    )

/*++

Routine Description:

    This routine initializes the structures necessary for EISA & PCI operations
    and connects the intermediate interrupt dispatcher. It also initializes the
    ISA interrupt controller; in the case of the SIO-II in Avanti, the 
    integral interrupt controller is compatible with the EISA interrupt 
    contoller used on Jensen.

Arguments:

    None.

Return Value:

    If the second level interrupt dispatcher is connected, then a value of
    TRUE is returned. Otherwise, a value of FALSE is returned.

--*/

{

    ULONG PciIsaBridgeHeaderOffset;
    KIRQL oldIrql;


    //
    // Initialize the EISA NMI interrupt.
    //

    HalpInitializeNMI();


    //
    // Directly connect the ISA interrupt dispatcher to the level for
    // ISA bus interrupt.
    //
    // N.B. This vector is reserved for exclusive use by the HAL (see
    //      interrupt initialization.
    //

    PCR->InterruptRoutine[PIC_VECTOR] = HalpSioDispatch;
    HalEnableSystemInterrupt(PIC_VECTOR, ISA_DEVICE_LEVEL, LevelSensitive);

    (PVOID) HalpPCIPinToLineTable = (PVOID) AvantiPCIPinToLineTable;

    //
    // Intitialize interrupt controller
    //

    KeRaiseIrql(ISA_DEVICE_LEVEL, &oldIrql);

    // 
    // We must declare the right form for the APECS PCI/ISA bridge 
    // configuration space device selector for use in the initialization
    // of the interrupts.
    //
    if (ApecsPass2) {
        PciIsaBridgeHeaderOffset = PCI_ISA_BRIDGE_HEADER_OFFSET_P2;
    } else {
        PciIsaBridgeHeaderOffset = PCI_ISA_BRIDGE_HEADER_OFFSET_P1;
    }

    if (SioCStep) {
        //
        // Setup the Avanti interrupt assignments for the C-Step SIO; this
        // is done with registers that are internal to the SIO-II, PIRQ0,
        // PIRQ1, PIRQ2, and PIRQ3.  They're at offsets 0x60, 0x61, 0x62,
        // and 0x63 of the SIO-II's configuration registers.  The effect
        // of these registers is to steer the PCI interrupts into the IRQx
        // inputs of the SIO's internal cascaded 82C59 interrupt controllers.
        // These controllers are then programmed in their usual fashion.
        //
        // Set up interrupts from PCI slots 0, 1, and 2, compatible with
        // Avanti pass 1 MLB, i.e., steer the interrupt from each PCI
        // slot or device into the default IRQL from pass 1 Avanti; enable 
        // the routings.
        //
        WRITE_CONFIG_UCHAR((PCHAR)(PCI_CONFIGURATION_BASE_QVA
                    | PciIsaBridgeHeaderOffset | PIRQ0_ROUTE_CONTROL),
                    PIRQX_ROUTE_IRQ10 | PIRQX_ROUTE_ENABLE,
                    PCI_CONFIG_CYCLE_TYPE_0);

        WRITE_CONFIG_UCHAR((PCHAR)(PCI_CONFIGURATION_BASE_QVA
                    | PciIsaBridgeHeaderOffset | PIRQ1_ROUTE_CONTROL),
                    PIRQX_ROUTE_IRQ15 | PIRQX_ROUTE_ENABLE,
                    PCI_CONFIG_CYCLE_TYPE_0);

        WRITE_CONFIG_UCHAR((PCHAR)(PCI_CONFIGURATION_BASE_QVA
                    | PciIsaBridgeHeaderOffset | PIRQ2_ROUTE_CONTROL),
                    PIRQX_ROUTE_IRQ9 | PIRQX_ROUTE_ENABLE,
                    PCI_CONFIG_CYCLE_TYPE_0);

        //
        // Set up interrupt from SCSI (IRQ11), and enable the routing.
        //
        WRITE_CONFIG_UCHAR((PCHAR)(PCI_CONFIGURATION_BASE_QVA
                    | PciIsaBridgeHeaderOffset | PIRQ3_ROUTE_CONTROL),
                    PIRQX_ROUTE_IRQ11 | PIRQX_ROUTE_ENABLE,
                    PCI_CONFIG_CYCLE_TYPE_0);

        //
        // Select "level" operation for PCI PIRQx interrupt lines, and
        // "edge" for ISA devices, e.g., mouse at IRQL12.  Default is "edge"
        // so no mention means "edge".
        //
        WRITE_REGISTER_UCHAR((PCHAR)(PCI_SPARSE_IO_BASE_QVA
                    | SIO_II_EDGE_LEVEL_CONTROL_2),
                    IRQ11_LEVEL_SENSITIVE | IRQ10_LEVEL_SENSITIVE |
                    IRQ9_LEVEL_SENSITIVE | IRQ15_LEVEL_SENSITIVE);

    } else {
        //
        //
        // Anything prior to a C-step SIO chip has a funny MUX register pair
        // for PCI interrupts.
        //
        // Avanti Pass1 only:  Intialize PCI Interrupt translation mux
        //
        // Select PCI Int Register.
        //

        WRITE_REGISTER_UCHAR((PUCHAR)(PCI_SPARSE_IO_BASE_QVA | PCI_INDEX),
                    PCI_INT_REGISTER);              // PCI Index Register 0x14
        //
        // Enable interrupts from PCI slots 0 and 1
        //

        WRITE_REGISTER_UCHAR((PUCHAR)(PCI_SPARSE_IO_BASE_QVA | PCI_DATA),
                    ENABLE_PINTA_0_AT_IRQ10 | 
                    ENABLE_PINTA_1_AT_IRQ15);

        //
        // Select PCI Int/Level Register.
        //

        WRITE_REGISTER_UCHAR((PUCHAR)(PCI_SPARSE_IO_BASE_QVA | PCI_INDEX),
                    PCI_INT_LEVEL_REGISTER);        // PCI Index Register 0x15
        //
        // Enable interrupts from PCI slot 2; select "level" operation for
        // all three interrupt lines.
        //

        WRITE_REGISTER_UCHAR((PUCHAR)(PCI_SPARSE_IO_BASE_QVA | PCI_DATA),
                    ENABLE_PINTA_2_AT_IRQ9 |
                    L2EEN0_LEVEL | L2EEN1_LEVEL | L2EEN1_LEVEL);
    
    }
// end AVANTI_PROTO1

    HalpInitializeSioInterrupts();

    //
    // Restore IRQL level.
    //

    KeLowerIrql(oldIrql);

    //
    // Initialize the DMA mode registers to a default value.
    // Disable all of the DMA channels except channel 4 which is the
    // cascade of channels 0-3.
    //

    WRITE_PORT_UCHAR(
        &((PEISA_CONTROL) HalpEisaControlBase)->Dma1BasePort.AllMask,
        0x0F
        );

    WRITE_PORT_UCHAR(
        &((PEISA_CONTROL) HalpEisaControlBase)->Dma2BasePort.AllMask,
        0x0E
        );

    return(TRUE);
}

VOID
HalpInitializeNMI( 
    VOID 
    )
/*++

Routine Description:

   This function is called to intialize SIO NMI interrupts.

Arguments:

    None.

Return Value:

    None.
--*/
{
    UCHAR DataByte;

    //
    // Initialize the SIO NMI interrupt.
    //

    KeInitializeInterrupt( &HalpEisaNmiInterrupt,
                           HalHandleNMI,
                           NULL,
                           NULL,
                           EISA_NMI_VECTOR,
                           EISA_NMI_LEVEL,
                           EISA_NMI_LEVEL,
                           LevelSensitive,
                           FALSE,
                           0,
                           FALSE
                         );

    //
    // Don't fail if the interrupt cannot be connected.
    //

    KeConnectInterrupt( &HalpEisaNmiInterrupt );

    //
    // Clear the Eisa NMI disable bit.  This bit is the high order of the
    // NMI enable register.
    //

    DataByte = 0;

    WRITE_PORT_UCHAR(
      &((PEISA_CONTROL) HalpEisaControlBase)->NmiEnable,
      DataByte
      );

}

BOOLEAN
HalHandleNMI(
    IN PKINTERRUPT Interrupt,
    IN PVOID ServiceContext
    )
/*++

Routine Description:

   This function is called when an EISA NMI occurs.  It print the appropriate
   status information and bugchecks.

Arguments:

   Interrupt - Supplies a pointer to the interrupt object

   ServiceContext - Bug number to call bugcheck with.

Return Value:

   Returns TRUE.

--*/
{
    UCHAR   StatusByte;
    
    StatusByte =
        READ_PORT_UCHAR(&((PEISA_CONTROL) HalpEisaControlBase)->NmiStatus);

    if (StatusByte & 0x80) {
        HalDisplayString ("NMI: Parity Check / Parity Error\n");
    }

    if (StatusByte & 0x40) {
        HalDisplayString ("NMI: Channel Check / IOCHK\n");
    }

     //
     // This is an Sio machine, no extnded nmi information, so just do it.
     //


    KeBugCheck(NMI_HARDWARE_FAILURE);
    return(TRUE);
}

UCHAR
HalpAcknowledgeEisaInterrupt(
    PVOID ServiceContext
    )
/*++

Routine Description:

    Acknowledge the EISA interrupt from the programmable interrupt controller.
    Return the vector number of the highest priority pending interrupt.

Arguments:

    ServiceContext - Service context of the interrupt service supplies
                     a pointer to the EISA interrupt acknowledge register.

Return Value:

    Return the value of the highest priority pending interrupt.

--*/
{
    UCHAR InterruptVector;

    //
    // Read the interrupt vector from the PIC.
    //

    InterruptVector = READ_PORT_UCHAR(ServiceContext);

    return( InterruptVector );

}

VOID
HalpAcknowledgeClockInterrupt(
    VOID
    )
/*++

Routine Description:

    Acknowledge the clock interrupt from the interval timer.  The interval
    timer for EB66 comes from the Dallas real-time clock.

Arguments:

    None.

Return Value:

    None.

--*/
{

    //
    // Acknowledge the clock interrupt by reading the control register C of
    // the Real Time Clock.
    //

    HalpReadClockRegister( RTC_CONTROL_REGISTERC );

    return;
}
