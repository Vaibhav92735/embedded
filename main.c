#include "stm32f4xx.h"
#include "can_protocol.h"
#include <stdio.h>
#include "uart.h"

int main(){
		printf("hello world");
		UART3Config();
	
	//UART3_SendString ("hello world12345\r\n");
		int state = 0;
		CAN_Init();
    // Create a CAN message
    CAN_Message message;
    message.id = 0x123; // Example ID
    message.len = 3;    // Example data length
    message.data[0] = 'A'; // Example data byte 1
    message.data[1] = 'B'; // Example data byte 2
    message.data[2] = 'C'; // Example data byte 3
		
		state = 1;

    // Send the CAN message
    CAN_SendMessage(&message);
		
		state = 2;
    
		// Receive a CAN message
    CAN_Message received_message;
    CAN_ReceiveMessage(&received_message);

		state = 3;
    // Print received message information
    printf("Received message:\n");
    printf("ID: 0x%X\n", received_message.id);
    printf("Length: %d\n", received_message.len);
    printf("Data:");
    for (int i = 0; i < received_message.len; i++) {
        printf(" 0x%X", received_message.data[i]);
    }
    printf("\n");

    return 0;
}
