#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H

#include <stdint.h>

// Define CAN message structure
typedef struct {
    uint32_t id;    // Message ID
    uint8_t data[8]; // Data field
    uint8_t len;    // Data length (up to 8 bytes)
} CAN_Message;

// Function prototypes
void CAN_Init();
void CAN_SendMessage(CAN_Message* msg);
void CAN_ReceiveMessage(CAN_Message* msg);

#endif /* CAN_PROTOCOL_H */
