
#include<mega328p.h>
#include"TWI_Lib.h"
#include"delay.h"

// use I2C to communicate with LCD 16x2

#define LCD_ADDR 0x27
#define LCD_RS 0x01
#define LCD_RW 0x02
#define LCD_EN 0x04
#define LCD_BL 0x08

#define LCD_D4 0x10
#define LCD_D5 0x20
#define LCD_D6 0x40
#define LCD_D7 0x80

#define LCD_LINE1 0x00
#define LCD_LINE2 0x40

#define LCD_BACKLIGHT 0x08




void LCD_send4bit(unsigned char data){
    unsigned char data_u, data_l;
    data_u = data&0xf0;
    data_l = (data<<4)&0xf0;
    TWI_Master_Send(LCD_ADDR, &data_u, 1);
    TWI_Master_Send(LCD_ADDR, &data_l, 1);
}

void LCD_send_command(unsigned char command){
    unsigned char data_u, data_l;
    data_u = command&0xf0;
    data_l = (command<<4)&0xf0;
    TWI_Master_Send(LCD_ADDR, &data_u, 1);
    TWI_Master_Send(LCD_ADDR, &data_l, 1);
}

void LCD_clear(void){
    LCD_send_command(0x01);
    delay_ms(2);
}

void LCD_gotoxy(unsigned char x, unsigned char y){
    unsigned char address;
    if (y==0) address = LCD_LINE1;
    else address = LCD_LINE2;
    address += x;
    LCD_send_command(address);
}


void LCD_init(void){
    LCD_send4bit(0x00);
    delay_ms(20);
    LCD_send4bit(0x03);
    delay_ms(5);
    LCD_send4bit(0x03);
    delay_ms(11);
    LCD_send4bit(0x03);
    LCD_send4bit(0x02);
    LCD_send_command(0x28);
    LCD_send_command(0x08);
    LCD_send_command(0x01);
    LCD_send_command(0x06);
    LCD_send_command(0x0C);
}

void LCD_send_data(unsigned char data){
    unsigned char data_u, data_l;
    data_u = data&0xf0;
    data_l = (data<<4)&0xf0;
    TWI_Master_Send(LCD_ADDR, &data_u, 1);
    TWI_Master_Send(LCD_ADDR, &data_l, 1);
}

void LCD_send_String(char *str){
    while (*str) LCD_send_data(*str++);
}


void LCD_define_char(unsigned char *pattern, unsigned char location){
    unsigned char i;
    if (location<8){
        LCD_send_command(0x40+(location*8));
        for (i=0;i<8;i++) LCD_send_data(pattern[i]);
    }
}

void LCD_send_Int(int val){
    char str[10];
    
}