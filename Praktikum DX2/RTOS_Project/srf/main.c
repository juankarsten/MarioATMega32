/*
 * $Id: main.c,v 1.11 2007/08/02 01:24:42 bsd Exp $
 *
 */

/*
 * SRF08 - a complete test program using the SRF08 on the BDMICRO
 * MAVRIC and MAVRIC-II microcontroller boards.  This program is
 * intended to demonstrate how to use the BDMICRO MAVRIC or MAVRIC-II
 * microcontroller boards in conjunction with SRF08 Ultrasonic Ranger
 * Module.  Feel free to copy the relevent function definitions into
 * your own applications and to customize them for your needs.
 *
 * See: http://www.bdmicro.com/
 *
 * Configuration:
 *
 *   Simply attach your SRF08 Ultrasonic Ranger Module to your board's
 *   I2C bus, power, and ground.  It's OK to use one of the supplied
 *   VCC pins on your microcontroller board to supply power to the
 *   SRF08.
 *
 *   If desired, attach a terminal to UART0 for 9600, 8N1; the SRF08
 *   will be read once per second and the result in inches printed to
 *   UART0.
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/twi.h>

#include <stdio.h>

#include "i2c.h"
#include "srf08.h"

#define BAUD_RR ((CPU_FREQ/(16L*9600L) - 1))


#define CPU_FREQ 16000000L  /* set to clock frequency in Hz */

#if CPU_FREQ == 16000000L
#define OCR_1MS 125
#elif CPU_FREQ == 14745600L
#define OCR_1MS 115
#endif

volatile uint16_t ms_count;


/*
 * ms_sleep() - delay for specified number of milliseconds
 */
void ms_sleep(uint16_t ms)
{
  TCNT0  = 0;
  ms_count = 0;
  while (ms_count != ms)
    ;
}


/* 
 * millisecond counter interrupt vector 
 */
SIGNAL(SIG_OUTPUT_COMPARE0)
{
  ms_count++;
}


/*
 * Initialize timer0 to use the main crystal clock and the output
 * compare interrupt feature to generate an interrupt approximately
 * once per millisecond to use as a general purpose time base.
 */
void init_timer0(void)
{
  TCCR0 = 0;
  TIFR  |= _BV(OCIE0)|_BV(TOIE0);
  TIMSK |= _BV(TOIE0)|_BV(OCIE0);         /* enable output compare interrupt */
  TCCR0  = _BV(WGM01)|_BV(CS02)|_BV(CS00); /* CTC, prescale = 128 */
  TCNT0  = 0;
  OCR0   = OCR_1MS;                     /* match in aprox 1 ms */
}




int def_putc(char ch, FILE * f)
{
  /* output character to UART0 */
  while ((UCSR0A & _BV(UDRE)) == 0)
    ;
  UDR0 = ch;
  return 0;
}





int main(void)
{
  uint16_t distance;

  init_timer0();

  /* enable UART0 */
  UBRR0H = (BAUD_RR >> 8) & 0xff;
  UBRR0L = BAUD_RR & 0xff;
  UCSR0B = _BV(TXEN); /* enable transmitter only */

  /* enable interrupts */
  sei();

  /* initialize stdio */
  fdevopen(def_putc, NULL);

  /* set the I2C bit rate generator to 100 kb/s */
  TWSR &= ~0x03;
  TWBR  = 28;
  TWCR |= _BV(TWEN);

  while (1) {
    ms_sleep(1000);                  /* sleep for 1 second */
    srf08_ping(0x70, RANGE_CM);      /* initiate a ping, distance in cm */
    ms_sleep(70);                    /* wait for 70 ms */
    srf08_range(0x70, 0, &distance); /* read first echo */
    printf("range = %0.1f inches\n", ((double)distance) / 2.54);
  }
}

