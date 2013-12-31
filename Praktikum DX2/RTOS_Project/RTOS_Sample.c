/*
Contoh program penggunaan RTOS.


Device	: ATmega32
Clock 	: 12 MHz
Koneksi	: - PORTA 		<-> LCD_DATA
		  - PORTC 5,6,7 <-> LCD_CONTROL
		  - PORTC 0,1	<-> I2C BUS
		  - PORTC 3		<-> Speaker
		  - PORTB		<-> LED
		  - PORTD 		<-> SWITCH

Library	: 	AVR-Libc
			FreeRTOS
			I2C Library

Notes	: 

Tim Asisten Sistem Tertanam 2011
(M. Sakti Alvissalim, Faris Al Afif, Big Zaman)
@ver: ESAT 2013
*/
#include <avr/eeprom.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "FreeRTOS.h"
#include "task.h"
#include "i2c.h"
#include "lcd.h"
#include "sample.c"
#include "util/delay.h"
#include "uas_constant.h"
#include "game.h"

#define TICKS_PER_MS 5


//###################STATE##########################
static int current_lcd_state = 3;
static int current_state = 1;
static int led_current_state = 0;
static int switchinput = 0;
static int second = 0;
static int firstsrand = 0;





/** 	
This task measures distance using the SRF08 every 100 ms 
and display the result in cm to LCD
*/
int global_sonar_cm = 0;
void vSonarTask( void * pvParameters )
{
    char lcd[16];
	int i = 0;
	init_lcd();
	
	while(1){
		unsigned int data = 0;
		// Send command to start measurement
 		i2c_transmit(0xE0, 0, 81);
		// Wait for the measurement to be done
		vTaskDelay(70 * TICKS_PER_MS);
		// Read the 16 bit result
		data = i2cRead(0xE0, 2) << 8;
		data |= i2cRead(0xE0, 3);
		global_sonar_cm = data;
		// Convert the data to string
		//ltoa(data, lcd, 10);

		/*
		// Clear the LCD
		init_lcd();
		
		// Display the distance to LCD
		for(i=0; i<16; i++){
			if(lcd[i]==0){
				break;
			}
			//tulis_data_ram_lcd('E');

			//tulis_data_ram_lcd(lcd[i]);
		}
		*/
		
		// Delay for another 100 ms
		vTaskDelay(20 * TICKS_PER_MS);
	}
}



void vLCDTask( void * pvParameters) {
	init_lcd();

	/*	
	tulis_data_ram_lcd(0x20); 		// Tulis spasi
	*/
	
	int ii = 0;
	while (1){
		if (current_lcd_state == LCD_EMPTY){
			kirim_perintah_lcd(0x01);
		}else if(current_state == HISCORESCREEN){
			tulis_string("HISCORE",0,0);
			int highscore = eeprom_read_word((uint16_t*)46);
			if(highscore<0){
				highscore=0;
			}
			char lcdstr[16];
			ltoa(highscore, lcdstr, 10);
			tulis_string(lcdstr,3,1);
		}else if(current_state == STARTSCREEN){
			tulis_string("0.START",0,0);
			tulis_string("1.HISKOR",0,1);
		}else if(current_state == LOADING){
			if(current_lcd_state == LCD_START){	
				current_lcd_state = LCD_WRITE;
			}else if (current_lcd_state == LCD_WRITE){
				tulis_string("LOADING.",0,0);
				tulis_string("GAME...",1,1);
			}
		}else if(current_state == GAME){
			clear_lcd();
			draw_game();
			//tulis_string("MULAI",0,0);
		}else if(current_state == RESULT){
			clear_lcd();
			
			int highscore = eeprom_read_word((uint16_t*)46);
			int pas = getpass();
			if(pas>=highscore){
				tulis_string("HISCORE: ",0,0);
				eeprom_update_word((uint16_t*)46,pas);
			}else{
				tulis_string("SCORE: ",1,0);
			}		
			char lcdstr[16];
			ltoa(pas, lcdstr, 10);
			tulis_string(lcdstr,3,1);

			
		}	
		
		vTaskDelay(20 * TICKS_PER_MS);
		
		
	}
}



void vLEDTask ( void * pvParameters ) {
	int current = 0;
	// Set PORTB as LED output //
	DDRB = 0xFF;
	PORTB = current;
	led_current_state = LED_START;
	while (1) {
		if(led_current_state == LED_STOP){
			current = 0;	
		}else if(led_current_state == LED_START){
			current = 0b11100000;
			led_current_state = LED_MOVE_LEFT;
		}else if(led_current_state == LED_MOVE_LEFT){
			if (current == 0b00000111){
				led_current_state = LED_MOVE_RIGHT;
			}else{
				current = current >> 1;
			}
		}else if(led_current_state == LED_MOVE_RIGHT){
			if (current == 0b11100000){
				led_current_state = LED_MOVE_LEFT;
			}else{
				current = current << 1;
			}
		}
		PORTB = current ^ 0b11111111;
		vTaskDelay(20 * TICKS_PER_MS);
	}
}


void vInputTask(void *pvParameters){
	DDRD = 0x11111100;
	while(1){
		int input = PIND;
		if(current_state == STARTSCREEN){
			if(input == 0b11111110){
				clear_lcd();
				current_state = LOADING;
				second = 0;
				
			}else if(input == 0b11111101){
				clear_lcd();
				current_state = HISCORESCREEN;
			}
		}else if(current_state == HISCORESCREEN){
			if(input == 0b11111110){
				clear_lcd();
				current_state = STARTSCREEN;
			}
		}else if(current_state == GAME){
			if(input == 0b11111110){
				set_mario(1);
			}else if(input == 0b11111101){
				set_mario(0);
			}
		}else if(current_state==RESULT && input == 0b11111110){
			clear_lcd();
			led_current_state = LED_STOP;
			current_lcd_state = LCD_EMPTY;
			current_state = LOADING;
			second = 0;
		}
		
		vTaskDelay(3 * TICKS_PER_MS);
	}

}


void vServoTask(void *pvParameters){
	int i, j, k;
	while (1) {
		if (current_state == GAME) {
			i = 1;
			for (k = 0; k < 40; k++) {
				PORTB |= 0b00000001;
				for (j = 0; j <= i; j++) {
					vTaskDelay(1);
				}
				PORTB &= 0b11111110;
		
				vTaskDelay(10 * TICKS_PER_MS);
			}
			vTaskDelay(1);
		
			i = 45;
			for (k = 0; k < 40; k++) {
				PORTB |= 0b00000001;
				for (j = 0; j <= i; j++) {
					vTaskDelay(1);
				}
				PORTB &= 0b11111110;
			
				vTaskDelay(10 * TICKS_PER_MS);
			}
			vTaskDelay(1);
		}
		vTaskDelay(1);		
	}	
}


void vMainGameTask(void *pvParameters){
    second = 0;
	current_state = STARTSCREEN;
	int firstloading = 1;
	
	while(1){
		if(current_state == STARTSCREEN){
			if(firstloading){
				led_current_state = LED_START;
				current_lcd_state = LCD_START;
				firstloading = 0;
				second = 0;
			}
		}else if(current_state == LOADING){
			if(firstloading){
				led_current_state = LED_START;
				current_lcd_state = LCD_START;
				firstloading = 0;
				second = 0;
			}

			if(second > LOADING_INTERVAL){
				second = 0;
				firstloading = 1;
				led_current_state = LED_STOP;
				current_lcd_state = LCD_EMPTY;
				current_state = GAME;
			}
		}else if(current_state == GAME){
			current_lcd_state = LCD_WRITE;
			if(firstloading){
				init_game();
				firstloading = 0;
				second = 0;
			}

			//switchinput = PORTD;
			update_game();
			
			if (global_sonar_cm<10){
				set_mode(FAST_MODE);
				led_current_state = LED_START;
			}else{
				set_mode(SLOW_MODE);
				led_current_state = LED_STOP;
			}
			if(!firstsrand){
				srand(global_sonar_cm*17*13);
				firstsrand=1;
			}

			if(get_state() == GAME_FINAL){
				second = 0;
				firstloading = 1;
				led_current_state = LED_STOP;
				current_lcd_state = LCD_WRITE;
				current_state = RESULT;
			}
		}
		second++;
		vTaskDelay(20 * TICKS_PER_MS);
	}	
	
}




int main(){
	// Task Handlers
	xTaskHandle xSonarTaskHandle, xMusicTaskHandle, xSoundTaskHandle, xLEDTaskHandle, xLCDTaskHandle, xMainGameTaskHandle;
	xTaskHandle xInputTaskHandle;
	
	/* set the I2C bit rate generator to 100 kb/s */
	
	TWSR &= ~0x03;
	TWBR  = 28;
	TWCR |= _BV(TWEN);

	LCD_CONTROL_DDR	 = _BV(LCD_RS_BIT) | _BV(LCD_EN_BIT) | _BV(LCD_RW_BIT);
	DDRA = 0xFF;
	
	

	/*
	Create the tasks
	*/
	xTaskCreate( vMainGameTask, "GAME", 100, NULL, tskIDLE_PRIORITY, &xMainGameTaskHandle);
	xTaskCreate( vLCDTask, "LCD", 100, NULL, tskIDLE_PRIORITY, &xLCDTaskHandle);
	xTaskCreate( vLEDTask, "LED", 100, NULL, tskIDLE_PRIORITY, &xLEDTaskHandle);
	xTaskCreate( vSonarTask, "Sonar", 100, NULL , tskIDLE_PRIORITY, &xSonarTaskHandle );
	xTaskCreate( vInputTask, "Input", 100, NULL , tskIDLE_PRIORITY, &xInputTaskHandle );


	// Start the scheduler
	vTaskStartScheduler();
}
