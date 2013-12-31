#ifndef __lcd_h__
#define __lcd_h__

// CONTROL PORT adalah port yang mengontrol RS, EN,dan RW 
#define LCD_CONTROL_PORT	PORTC
#define LCD_CONTROL_DDR		DDRC
#define LCD_RS_BIT			5
#define LCD_EN_BIT			7
#define LCD_RW_BIT			6

// DATA PORT adalah port yang digunakan untuk perpindahan data 
// dari uC ke LCD maupun sebaliknya
#define LCD_DATA_PORT		PORTA
#define LCD_DATA_PIN		PINA
#define LCD_DATA_DDR		DDRA


unsigned char lcdSiap();

void lcdWait();

void kirim_perintah_lcd(unsigned char perintah);

void init_lcd (void);

void clear_lcd (void);

void tulis_data_ram_lcd(unsigned char data);


void set_cursor(int x,int y);

void tulis_data_at(unsigned char data, int x, int y);

void tulis_string(unsigned char* data, int x, int y);

#endif

