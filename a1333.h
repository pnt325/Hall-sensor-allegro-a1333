/*
 * a1333.h
 *
 *  Created on: Dec 29, 2022
 *      Author: Phat.N
 */

#ifndef _A1333_H_
#define _A1333_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

uint16_t a1333_extended_read(uint16_t addr, uint32_t* value);
uint16_t a1333_extended_write(uint16_t addr, uint32_t value);

#endif /*_A1333_H_*/

