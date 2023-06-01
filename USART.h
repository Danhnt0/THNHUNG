#ifndef _USART_H_
#define _USART_H_
#include <mega328p.h>
#define FOSC 16000000UL

void USART_init(unsigned long BAUD);
void USART_send_byte(unsigned char data);
void USART_print_str(const char *data);
void USART_print_float(float data);
void USART_print_int(int data);
int USART_received_num();
unsigned char USART_received_char();


#endif
