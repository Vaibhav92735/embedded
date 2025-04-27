#include "can_protocol.h"
#include "stm32f4xx.h" // Include the appropriate header for your microcontroller

// Define CAN controller registers (example for STM32F4xx series)
#define CAN1_BASE_ADDRESS   0x40006400
#define CAN1               ((CAN_TypeDef *)CAN1_BASE_ADDRESS)

// Function prototypes for low-level CAN operations
static void CAN_SetBitRate(uint32_t bitrate);
static void CAN_ConfigFilter(uint8_t filterId, uint32_t id, uint32_t mask);

// Initialize CAN controller
void CAN_Init() {
    // Enable CAN clock
    RCC->APB1ENR &= ~RCC_APB1ENR_CAN1EN;
	    
		RCC -> APB1ENR |= RCC_APB1ENR_CAN1EN;


    // Reset CAN controller
    CAN1->MCR |= CAN_MCR_RESET;
    while (CAN1->MCR & CAN_MCR_RESET); // Wait for reset to complete

    // Enter initialization mode
    CAN1->MCR |= CAN_MCR_INRQ;
    while (!(CAN1->MSR & CAN_MSR_INAK)); // Wait until in initialization mode

    // Set CAN bit rate
    CAN_SetBitRate(500000); // Example bit rate: 500 kbps

    // Configure CAN filters (if needed)
    // CAN_ConfigFilter(0, 0x123, 0x7FF); // Example: accept messages with ID 0x123

    // Exit initialization mode
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while (CAN1->MSR & CAN_MSR_INAK); // Wait until exit initialization mode
}

// Send CAN message
void CAN_SendMessage(CAN_Message* msg) {
    // Wait until transmit mailbox is empty
    while (!(CAN1->TSR & CAN_TSR_TME0));

    // Set up CAN message
    CAN1->sTxMailBox[0].TIR = (msg->id << 21) | CAN_TI0R_TXRQ; // Standard ID, RTR = 0
    CAN1->sTxMailBox[0].TDTR = msg->len;
		CAN1->sTxMailBox[0].TDLR = msg->data[3] << 24 | msg->data[2] << 16 | msg->data[1] << 8 | msg->data[0] << 0;

		
}

// Receive CAN message
void CAN_ReceiveMessage(CAN_Message* msg) {
    // Check if message available in receive FIFO
    if (CAN1->RF0R & CAN_RF0R_FMP0) {
        // Read message ID
        msg->id = (CAN1->sFIFOMailBox[0].RIR >> 21) & 0x7FF; // Extract 11-bit standard ID

        // Read message data and length
        msg->len = CAN1->sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC;
        for (uint8_t i = 0; i < msg->len; i++) {
            msg->data[i] = (CAN1->sFIFOMailBox[0].RDLR >> (i * 8)) & 0xFF;
        }

        // Release receive FIFO
        CAN1->RF0R |= CAN_RF0R_RFOM0;
    }
}

// Set CAN bit rate (example for 42 MHz APB1 clock and 500 kbps bit rate)
static void CAN_SetBitRate(uint32_t bitrate) {
    uint32_t brp = 42 / bitrate; // Example: 42 MHz APB1 clock
    CAN1->BTR = (brp - 1) | (7 << 16); // Set BRP and TS1, TS2, SJW
}

// Configure CAN filter (example)
static void CAN_ConfigFilter(uint8_t filterId, uint32_t id, uint32_t mask) {
    CAN1->FA1R |= (1 << filterId); // Enable filter
    CAN1->sFilterRegister[filterId].FR1 = id << 5;
    CAN1->sFilterRegister[filterId].FR2 = mask << 5;
    CAN1->FM1R &= ~(1 << filterId); // Set filter mode to mask mode
}
