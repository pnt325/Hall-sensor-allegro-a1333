/*
 * a1333.c
 *
 *  Created on: Dec 29, 2022
 *      Author: Phat.N
 */

#include "a1333.h"

#define A1333_CMD_NOP   (0x0000)

#define kNOERROR 0
#define kPRIMARYREADERROR 1
#define kEXTENDEDREADTIMEOUTERROR 2
#define kPRIMARYWRITEERROR 3
#define kEXTENDEDWRITETIMEOUTERROR 4

//const uint16_t ChipSelectPin = 10;
const uint16_t LEDPin       = 13;
const uint32_t WRITE        = 0x40;
const uint32_t READ         = 0x00;
const uint32_t COMMAND_MASK = 0xC0;
const uint32_t ADDRESS_MASK = 0x3F;

unsigned long nextTime;
bool ledOn = false;

bool includeCRC = false;


static uint8_t spi_transfer(uint8_t value);
static uint16_t spi_transfer16(uint16_t value);
static void spi_cs_ctrl(uint8_t state);

static uint16_t PrimaryRead(uint16_t addr, uint16_t* value);
static uint16_t PrimaryWrite(uint16_t addr, uint16_t value);

uint16_t a1333_extended_read(uint16_t addr, uint32_t* value)
{
   uint16_t results;
    uint16_t readFlags;
    uint32_t timeout;
    uint16_t valueMSW;
    uint16_t valueLSW;
    uint32_t currentTime;

    // Write the address to the Extended Read Address register
    results = PrimaryWrite(0x0A, addr & 0xFFFF);

    if (results != kNOERROR)
    {
        return results;
    }

    // Initiate the extended read
    results = PrimaryWrite(0x0C, 0x8000);
        
    if (results != kNOERROR)
    {
        return results;
    }

    timeout = millis() + 100L;

    do  // Wait for the read to be complete
    {
        results = PrimaryRead(0x0C, readFlags);
    
        if (results != kNOERROR)
        {
            return results;
        }

        // Make sure the read is not taking too long
        currentTime = millis();
        if (timeout < currentTime)
        {
            return kEXTENDEDREADTIMEOUTERROR;
        }
    } while ((readFlags & 0x0001) != 0x0001);
    
    // Read the most significant word from the extended read data
    results = PrimaryRead(0x0E, valueMSW);

    if (results != kNOERROR)
    {
        return results;
    }

    // Read the least significant word from the extended read data
    results = PrimaryRead(0x10, valueLSW);

    // Combine them
    value = ((uint32_t)valueMSW << 16) + valueLSW;

    return results;
}

uint16_t a1333_extended_write(uint16_t addr, uint32_t value)
{
   uint16_t results;
    uint16_t writeFlags;
    uint32_t timeout;

    // Write into the extended address register
    results = PrimaryWrite(0x02, addr & 0xFFFF);
    
    if (results != kNOERROR)
    {
        return results;
    }

    // Write the MSW (Most significant word) into the high order write data register
    results = PrimaryWrite(0x04, (value >> 16) & 0xFFFF);
        
    if (results != kNOERROR)
    {
        return results;
    }

    // Write the LSW (Least significant word) into the low order write data register
    results = PrimaryWrite(0x06, value & 0xFFFF);
        
    if (results != kNOERROR)
    {
        return results;
    }

    // Start the write process
    results = PrimaryWrite(0x08, 0x8000);
        
    if (results != kNOERROR)
    {
        return results;
    }

    timeout = millis() + 100;

    // Wait for the write to complete
    do
    {
        results = PrimaryRead(0x08, writeFlags);
    
        if (results != kNOERROR)
        {
            return results;
        }

        if (timeout < millis())
        {
            return kEXTENDEDWRITETIMEOUTERROR;
        }
    } while ((writeFlags & 0x0001) != 0x0001);

    return results;
}

uint8_t spi_transfer(uint8_t value)
{

}

static uint16_t PrimaryRead(uint16_t addr, uint16_t *value)
{
    // Combine the register address and the command into one byte
    uint16_t command = ((addr & ADDRESS_MASK) | READ) << 8;

    // take the chip select low to select the device
    spi_cs_ctrl(0)

    // send the device the register you want to read
    //SPI.transfer16(command);
    spi_transfer16(command);

    spi_cs_ctrl(1);


    spi_cs_ctrl(0)

    // send the command again to read the contents
    value[0] = spi_transfer16(command);

    // take the chip select high to de-select
    spi_cs_ctrl(1);

    return 0;
}

static uint16_t PrimaryWrite(uint16_t addr, uint16_t value)
{
    // Combine the register address and the command into one byte
    uint16_t command = ((addr & ADDRESS_MASK) | WRITE) << 8;

    // take the chip select low to select the device:
    // digitalWrite(cs, LOW);
    spi_cs_ctrl(0);

    spi_transfer16(command | ((value >> 8) & 0x0FF));
    // SPI.transfer16(command | ((value >> 8) & 0x0FF)); // Send most significant
                                                      // byte of register data

    // take the chip select high to de-select:
    // digitalWrite(cs, HIGH);
    spi_cs_ctrl(1);

    // GPIO_HIGH(PIN_A1333_CS);
    command = (((addr + 1) & ADDRESS_MASK) | WRITE) << 8;
    // take the chip select low to select the device:
    // digitalWrite(cs, LOW);
    //GPIO_LOW(PIN_A1333_CS);

    spi_cs_ctrl(0);
    //SPI.transfer16(command | (value & 0x0FF)); // Send least significant byte
                                               // of register data
    spi_transfer16(command | (value & 0x0FF));

    // take the chip select high to de-select:
    // digitalWrite(cs, HIGH);
    spi_cs_ctrl(1);
    //GPIO_HIGH(PIN_A1333_CS);
    //SPI.endTransaction();
}

static void spi_cs_ctrl(uint8_t state)
{

}

static uint16_t spi_transfer16(uint16_t value)
{

}