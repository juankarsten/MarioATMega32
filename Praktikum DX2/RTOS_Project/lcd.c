#include <avr/io.h>
#include <avr/delay.h>
#include "lcd.h"


unsigned char lcdSiap(){
	unsigned char busyFlag;

	LCD_DATA_DDR = 0x00;					// Set PORT DATA sebagai input
	LCD_DATA_PORT = 0x00;					// Matikan pull-up
	LCD_CONTROL_PORT &= ~_BV(LCD_RS_BIT);	// RS = 0
	LCD_CONTROL_PORT |= _BV(LCD_RW_BIT);	// RW = 1
	LCD_CONTROL_PORT |= _BV(LCD_EN_BIT);	// EN = 1

	_delay_us(1);							// Beri jeda untuk menunggu input stabil

	busyFlag = (LCD_DATA_PIN & _BV(7));		// Check busy flag bit
	
	LCD_CONTROL_PORT &= ~_BV(LCD_EN_BIT);	// EN = 0

	return busyFlag;
}

void lcdWait(){
	unsigned char busyFlag = 1;

	while(busyFlag != 0){					// Tunggu hingga busy flag == 0
		busyFlag = lcdSiap();
	}
}

void kirim_perintah_lcd(unsigned char perintah){
	lcdWait();								// Tunggu hingga lcd siap

	LCD_DATA_DDR = 0xFF;					// Set Data sebagai output
	LCD_CONTROL_PORT &= ~_BV(LCD_RS_BIT);	// RS = 0
	LCD_CONTROL_PORT &= ~_BV(LCD_RW_BIT);	// RW = 0
	LCD_CONTROL_PORT |= _BV(LCD_EN_BIT);	// EN = 1
	LCD_DATA_PORT = perintah;				// Set perintah yang ingin dikirim	
	
	LCD_CONTROL_PORT &= ~_BV(LCD_EN_BIT);	// EN = 0		
}

//Inisialisasi LCD
void init_lcd (void)
{
    _delay_ms(20);  						//Wait for more than 15 ms
    							
    kirim_perintah_lcd(0x38);  				//Function set: Interface is 8 bit long,
    _delay_us(50);  						// 2lines, 5x7 dots (N=1, F=0)
											//Execution time 39 us
    
    kirim_perintah_lcd(0x04);				//Display Off
    
    kirim_perintah_lcd(0x01);				//Display Clear
    
    kirim_perintah_lcd(0x06);				//Entry mode set: increment & no display shift
    
    kirim_perintah_lcd(0x0E);				//Display ON,Cursor OFF & Blink OFF
}

//Clear LCD
void clear_lcd (void)
{
    _delay_ms(20);  						//Wait for more than 15 ms
    							  
    kirim_perintah_lcd(0x01);				//Display Clear
    
}

void set_cursor(int x,int y){
	int address = 0b10000000;
	char ddram = address ^ x;
	if (y==1){
		ddram = ddram ^ 0b01000000;
	}
	kirim_perintah_lcd(ddram);
}

void tulis_data_at(unsigned char data, int x, int y){
	set_cursor(x,y);
	tulis_data_ram_lcd(data);
}

void tulis_string(unsigned char* data, int x, int y){
	set_cursor(x,y);
	for(int ii=0; ii<strlen(data); ii++){
		tulis_data_ram_lcd(data[ii]);
	}

}

void tulis_data_ram_lcd(unsigned char data){
	lcdWait();								// Tunggu hingga lcd siap

	LCD_DATA_DDR = 0xFF;					// Set Data sebagai output
	LCD_CONTROL_PORT |= _BV(LCD_RS_BIT);	// RS = 1
	LCD_CONTROL_PORT &= ~_BV(LCD_RW_BIT);	// RW = 0
	LCD_CONTROL_PORT |= _BV(LCD_EN_BIT);	// EN = 1
	
	LCD_DATA_PORT = data;					// Set data yang ingin dikirim
	LCD_CONTROL_PORT &= ~_BV(LCD_EN_BIT);	// EN = 0		
}
