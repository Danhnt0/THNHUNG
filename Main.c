/*
 * Main.c
 *
 * Created: 5/24/2023 6:56:19 PM
 * Author: Admin
 */

// #include "USART.h"
#include "TWI_Lib.h"
#include <mega328p.h>
#include <delay.h>
#include <alcd_twi.h>
#include <Servo.h>


#define SENSOR1_PIN 2 // Ch�n k?t n?i c?m bi?n 1
#define SENSOR2_PIN 3 // Ch�n k?t n?i c?m bi?n 2
#define LCD_ADDRESS 0x27

unsigned char IR_1 = 0;
unsigned char IR_2 = 0;


unsigned int count = 0;
unsigned int open = 0;

unsigned char regit[1];

interrupt[EXT_INT0] void ext_int1_isr(void)
{
	 IR_1 = 1;
	 if((open == 0) && (count == 0))
		  {
		  open = 1;
		  servo_set_angle(180);
		  }
}

interrupt[EXT_INT1] void ext_int2_isr(void)
{
	 IR_2 = 1;
	 if((open == 0) && (count == 10))
		  {
		  open = 1;
		  servo_set_angle(180);
		  }
}

void DS1307_get_all(unsigned char *time)
{

	 regit[0] = 0;
	 TWI_Master_Send(208, regit, 1);
	 TWI_Master_Receive(209, time, 7);


}

unsigned int BCD2DEC(unsigned int bcd)
{
	 return (bcd >> 4) * 10 + (bcd & 0x0F);
}



void main(void)
{
	 unsigned char time[7];
	 unsigned char i;
	 DDRD &= ~(1 << SENSOR1_PIN);
	 DDRD &= ~(1 << SENSOR2_PIN);

	 EICRA |= (1 << ISC01);
	 EICRA |= (1 << ISC11);
	 EIMSK |= (1 << INT0); // Cho ph�p interrupt INT0
	 EIMSK |= (1 << INT1); // Cho ph�p interrupt INT1
#asm("sei")

	 TWI_Init(1, 1, 100000);
	 twi_master_init(100);
	 servo_init();
	 lcd_twi_init(LCD_ADDRESS, 16);
	 lcd_clear();
	 lcd_printfxy(0, 0, "Room empty!");
	 DDRB.5 = 1;
	 PORTB.5 = 0;

	 while (1)
		  {
		  DS1307_get_all(time);
		  for(i = 0; i < 7; i++)
			   time[i] = BCD2DEC(time[i]);

		  lcd_printfxy(0, 1, "%02u:%02u", time[2], time[1], time[0]);
		  // display date
		  lcd_printfxy(6, 1, "%02u/%02u/%02u", time[4], time[5], time[6]);
		  if (IR_1)
			   {
			   delay_ms(800);
			   if (IR_2)
					{
					count++;
					lcd_clear();
					lcd_printfxy(0, 0, "Sum people:");
					lcd_printfxy(12, 0, "%u", count);
					IR_1 = 0;
					IR_2 = 0;
					}
			   else
					IR_1 = 0;
			   }

		  if (IR_2)
			   {
			   delay_ms(800);
			   if (IR_1)
					{
					count--;
					lcd_clear();
					lcd_printfxy(0, 0, "Sum people:");
					lcd_printfxy(12, 0, "%u", count);
					IR_1 = 0;
					IR_2 = 0;
					}
			   else
					IR_2 = 0;
			   }

		  if (((count == 0) || (count > 9)) && (open == 1))
			   {
			   delay_ms(2000);
			   open = 0;
			   servo_set_angle(0);
			   if (count > 9)
					{
					lcd_clear();
					lcd_printfxy(0, 0, "Full people:");
					}
			   }

		  if(count == 0)
			   PORTB.5 = 1;
		  else
			   PORTB.5 = 1;

		  lcd_printfxy(0, 1, "%02u:%02u", time[2], time[1], time[0]);
		  // display date
		  lcd_printfxy(6, 1, "%02u/%02u/%02u", time[4], time[5], time[6]);

		  }

}
