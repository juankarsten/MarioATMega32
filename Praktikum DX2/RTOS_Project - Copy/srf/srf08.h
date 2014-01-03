/*
 * $Id: srf08.h,v 1.1 2003/11/15 22:42:54 bsd Exp $
 *
 */

#ifndef __srf08_h__
#define __srf08_h__

#include <inttypes.h>

#define RANGE_IN 0x50
#define RANGE_CM 0x51
#define RANGE_US 0x52

int8_t srf08_ping(uint8_t device, uint8_t cmd);

int8_t srf08_range(uint8_t device, uint8_t echo, uint16_t * range);

#endif
