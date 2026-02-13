#include "xinu.h"

/* The base address for the QEMU 'virt' machine's primary UART */
#define UART0_BASE  0x09000000
#define UART0_DR    (*(volatile uint32 *)(UART0_BASE + 0x00))
#define UART0_FR    (*(volatile uint32 *)(UART0_BASE + 0x18))

/* Flag register bit: Transmit FIFO full */
#define UART_FR_TXFF 0x20 

/*------------------------------------------------------------------------
 * kputc - Write a single character to the serial console
 *------------------------------------------------------------------------
 */
void kputc(char c) {
    /* Wait for the UART to have space in its transmit buffer */
    while (UART0_FR & UART_FR_TXFF);

    /* Write the character to the Data Register */
    UART0_DR = c;
}

/*------------------------------------------------------------------------
 * kprintf - A tiny, limited version of printf for kernel debugging
 *------------------------------------------------------------------------
 */
void kprintf(char *str) {
    while (*str) {
        kputc(*str++);
    }
}


/*------------------------------------------------------------------------
 * kputn - Print an unsigned integer to the serial console
 *------------------------------------------------------------------------
 */
void kputn(uint32 n) {
    if (n / 10) {
        kputn(n / 10); /* Recursively handle higher digits */
    }
    kputc((n % 10) + '0'); /* Convert the remainder to ASCII */
}
