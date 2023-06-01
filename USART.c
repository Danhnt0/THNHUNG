#include <mega328p.h>
#include "USART.h"

// volatile static unsigned char check_busy = 1;
volatile static unsigned char data_rx;

interrupt[USART_RXC] void rxc_isr(void)
{
    data_rx = UDR0;
}
/*
interrupt [USART_TXC] void txc_isr (void)
{
    check_busy =  1;
}
*/
void USART_init(unsigned long BAUD)
{
    /*  Set baud rate   */
    unsigned long ubrr = FOSC / (16 * BAUD) - 1;
    UBRR0H = ubrr >> 8;
    UBRR0L = ubrr;
    /*  Enable Transmitter and Receiver  */
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    /* Enable RX_UART interrupt  */
    UCSR0B |= (1 << RXCIE0) | (0 << TXCIE0);
    /*  Set frame format: 8-bit data, 1stop bit   */
    /*  Asynchronous USART, non-parity mode   */
    UCSR0C |= (0 << UMSEL01) | (0 << UMSEL00);
    UCSR0C |= (0 << UPM01) | (0 << UPM00);
    UCSR0C |= (0 << USBS0);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_send_byte(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    // while (check_busy == 0);
    // check_busy = 0;
    UDR0 = data;
}

void USART_print_str(const char *data)
{
    while (*data)
        USART_send_byte(*data++);
}

void USART_print_int(int data)
{
	 unsigned int buf[8];
	 int index = 0, i, j;
	 j = data;

	 do
		  {
		  buf[index] = j % 10 + 48;
		  j = j / 10;
		  index ++;
		  }
	 while(j);

	 for(i = index; i > 0; i--)
		  {
          USART_send_byte(buf[i-1]);
          }
}

int USART_received_num()
{
    return data_rx - 48;
}

unsigned char USART_received_char()
{
    return data_rx;
}

void USART_print_float(float data)
{
    long num  = data * 1000;
    long  tp = num % 1000;
    
    USART_print_int(num/1000);
    USART_send_byte('.');
    USART_print_int(tp);
}
