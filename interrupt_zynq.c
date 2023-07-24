


#include "xparameters.h"
#include <xil_io.h>
#include "platform.h"
#include "ps7_init.h"
#include "xscugic.h"

XScuGic InterruptController;
static XScuGic_Config *GicConfig;


void InterruptHandler_send(){

}

void InterruptHandler_recieve(){

}




int SetUpInterruptSystem(XScuGic *XScuGicInstancePtr){

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) XScuGic_InterruptHandler, XScuGicInstancePtr);
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

int InitializeInterruptSystem(u8 deviceID, u8 DMA, void (*func_ptr)(void) ){
	int Status;

	GicConfig =  XScuGic_LookupConfig( deviceID );
	if ( NULL==GicConfig){
		return XST_FAILURE;
	}
	Status = XScuGic_CfgInitialize( &InterruptController, GicConfig, GicConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS){
		return XST_FAILURE;
	}
	Status = SetUpInterruptSystem(&InterruptController);
	if (Status != XST_SUCCESS){
		return XST_FAILURE;
	}
	Status = XScuGic_Connect( &InterruptController, DMA, (Xil_ExceptionHandler) func_ptr, NULL);
	if (Status != XST_SUCCESS){
		return XST_FAILURE;
	}

	XScuGic_Enable ( &InterruptController, DMA );

	return XST_SUCCESS;

}



int main(){

	init_platform();

	//initializing interrupts system
	xil_printf("Initializing Interrupts.....\n\r");

    
// Here interrupts for DMA are being initialized as an example
	InitializeInterruptSystem(XPAR_PS7_SCUGIC_0_DEVICE_ID, XPAR_FABRIC_AXI_DMA_1_MM2S_INTROUT_INTR, InterruptHandler_send );
	InitializeInterruptSystem(XPAR_PS7_SCUGIC_0_DEVICE_ID, XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR, InterruptHandler_recieve );
	xil_printf("Interrupts Initialized.....\n\r");

	xil_printf("Interrupts Initialized.....\n\r");


	while(1);

	cleanup_platform();

	return 0;
}
