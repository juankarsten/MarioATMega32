#include <stdlib.h>
#include "lcd.h"
#include "FreeRTOS.h"
#include <avr/io.h>

#include "game.h"

// lokasi mario
static int mario = 0;
// letak obstacle
static int obstacle[2][8];
// banyak obstacle dipass
static int pass = 0;
// speed of mario
static int speed = 0;
//summon time
static int summon_time=3;
// state game,win,lose
static int play_state=0;
//state mode
static int mode = SLOW_MODE;


void setup_seed(){
	 unsigned char oldADMUX = ADMUX;
	 ADMUX |=  _BV(MUX0); //choose ADC1 on PB2
	 ADCSRA |= _BV(ADPS2) |_BV(ADPS1) |_BV(ADPS0); //set prescaler to max value, 128
	 ADCSRA |= _BV(ADEN); //enable the ADC
	 ADCSRA |= _BV(ADSC);//start conversion
	 while (ADCSRA & _BV(ADSC)); //wait until the hardware clears the flag. Note semicolon!
	 unsigned char byte1 = ADCL;
	 ADCSRA |= _BV(ADSC);//start conversion
	 while (ADCSRA & _BV(ADSC)); //wait again note semicolon!
	 unsigned char byte2 = ADCL;
	 unsigned int seed = byte1 << 8 | byte2;
	 srand(seed);
	 ADCSRA &= ~_BV(ADEN); //disable ADC
	 ADMUX = oldADMUX;
}



void init_game(){
	setup_seed();
	mario = 0;
	pass = 0;
	speed = 0;
	summon_time = 2*TIME_INTERVAL;
	play_state = GAME_PLAY;
	for (int ii=0; ii<2; ii++){
		for (int jj=0; jj<8; jj++){
			obstacle[ii][jj] = 0;
		}
	}
	obstacle[rand()%2][7] = 1;
}


void update_game(){
	if(play_state==GAME_PLAY){
		//menggerakan mario ketika time> time_interval
		if(speed>TIME_INTERVAL-(mode-1)){
			speed = 0;
			for (int ii=0; ii<2; ii++){
				for (int jj=0; jj<8; jj++){
					if(obstacle[ii][jj]){
						obstacle[ii][jj] = 0;
						if(jj>=1) {
							obstacle[ii][jj-1] = 1;
						}else{
							pass+=mode;
						}				
					}

				}
			}
			summon_time--;	
		}	

		

		if(obstacle[mario][0]){
			play_state = GAME_FINAL;
		}


		if(summon_time<=0){
			int yy =  rand()%2;
			obstacle[yy][7] = 1;
			summon_time = rand()%(3-(mode-1))+2;	
		}

		speed++;
	}
}



void draw_game(){
	tulis_data_at('M',0,mario);
	if(mario==1){
		tulis_data_at(' ',0,0);
	}else{
		tulis_data_at(' ',0,1);
	}
	for (int ii=0; ii<2; ii++){
		for (int jj=0; jj<8; jj++){
			if(obstacle[ii][jj]){
				tulis_data_at('#',jj,ii);			
			}
		}
	}
	
}

void set_mario(int loc){
	if(loc==0){
		mario = 0;
	}else{
		mario = 1;
	}
}

int get_mario(){
	return mario;
}


int get_state(){
	return play_state;
}

int getpass(){
	return pass;
}

void set_mode(int mod){
	mode = mod;
}
