/*//////////////////
This is library for TWI comunication in AVR ATmega328p
Created by Tuan Do
*/////////////////
#ifndef _TWILIB_
#define _TWILIB_

#include <mega328p.h>
#include <delay.h>


#define fcpu 16000000
#define BR400   12   
#define BR200   32 
#define BR100   72



//Global variables

int i;

void TWI_Init(char sla,char gcall, char sclock){
    TWAR = (sla<<1) + gcall;
    TWSR &= 0b11111100;     //TWI prescale  = 1
    TWBR = (unsigned char)((fcpu/sclock -16)/2);
    TWCR = 0b01000101;
} 



void TWI_Interupt_Enable(void){
    TWCR |= 0b01000001;
}
///////////////////////////////////////////
void TWI_Interupt_Disable(void){
    TWCR &= 0b11111110;
}
/////////////////////////////////////////////
unsigned char TWI_Start(void){
    
    TWCR = 0b10100100;
    while (!(TWCR & 0b10000000));    
    return (TWSR & 0xF8);
    }     
/////////////////////////////////////////////
void TWI_Stop(void) {
    TWCR = 0b10010100;
    } 
/////////////////////////////////////////////////////    
unsigned char TWI_SLA_RW(unsigned char add, unsigned char rw){
    TWDR = add;
    TWCR = 0b10000100;
    while (!(TWCR & 0b10000000));
    return (TWSR & 0xF8); 

}
////////////////////////////////////////
unsigned char TWI_Send_Byte(unsigned char b){
    TWDR = b;
    TWCR = 0b10000100;
    while (!(TWCR & 0b10000000));
    return (TWSR & 0xF8); 
}
/////////////////////////////////////////
unsigned char TWI_Send_Array(unsigned char* arr, int length){
    for (i = 0; i < length; i++){        
        if (TWI_Send_Byte(arr[i]) != 0x28) 
        return (TWSR & 0xF8);
    }
    return (TWSR & 0xF8);  
}
/////////////////////////////////////////
//Process error when transmision
void TWI_Error(void){
    TWI_Stop();
    TWI_Interupt_Enable(); 
}
unsigned char TWI_Master_Send(unsigned char sla, unsigned char* arr, 
                                int length){
   TWI_Interupt_Disable();
   if (TWI_Start() != 0x08){
   
   return 1;                
   }
   if (TWI_SLA_RW(sla,0) != 0x18)
   return 1;
   if (TWI_Send_Array(arr, length) != 0x28)
   return 1;
   TWI_Stop();
   //Enable TWI interupt to jump in slave mode
   TWI_Interupt_Enable();
   return 0;
} 
/////////////////////////////////////////
unsigned char TWI_Master_Receive(unsigned char sla, unsigned char* arr, 
                                int length){
   //return 0: Receive successfully
   //return 1: Error
    //  uart_putstring("Read \n");
   TWI_Interupt_Disable();
   if (TWI_Start() != 0x08){
    return 1;               
    }
    
   if (TWI_SLA_RW(sla,1) != 0x40){
   
    return 1;                     
    }
   for (i = 0; i < (length-1); i++){
    TWCR = 0b11000100;               //Clear TWINT, set TWEA to start reading
    while (!(TWCR & 0b10000000));    //Wait for TWINT is set
    if ((TWSR & 0xF8)!= 0x50)        //Check the status code
        return 1;
    arr[i] = TWDR;
   }
   //Receive last byte
   TWCR = 0b10000100;   //Clear TWINT, clear TWEA to read last byte, not ack
   while (!(TWCR & 0b10000000));    //Wait for TWINT is set
   if ((TWSR & 0xF8)!= 0x58)        //Check the status code
        return 1;
   arr[length-1] = TWDR;
   //Send Stop condition to stop the communication 
   //uart_putstring("Read OK\n");
   TWI_Stop();
   //Enable TWI interupt to jump in slave mode
   TWI_Interupt_Enable();
   return 0;       
}




#endif 