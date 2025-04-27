#ifndef UART_H
#define UART_H

#include "stm32f4xx.h"
#include <stdio.h>

/**
 * @brief  Configures UART3: GPIO pins, USART parameters, and NVIC for interrupts if needed.
 */
void UART3Config(void);

/**
 * @brief  Sends a single character over UART3 (blocking).
 * @param  c: character to send
 */
void UART3_SendChar(uint8_t c);

/**
 * @brief  Sends a null-terminated string over UART3 (blocking).
 * @param  str: pointer to the string to send
 */
void UART3_SendString(const char *str);

/**
 * @brief  Retargets the C library printf to the UART3.
 * @param  ch: character to send
 * @param  f: file pointer (unused)
 * @return ch
 */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar(). */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

#endif /* UART_H */
