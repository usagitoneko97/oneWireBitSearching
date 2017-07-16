#include <stdint.h>

#ifndef _ONEWIREIO_H
#define _ONEWIREIO_H

/*Global variables*/
#define TRUE  1
#define FALSE 0


unsigned char ROM_NO[8];
int LastDiscrepancy;
int LastFamilyDiscrepancy;
int LastDeviceFlag;
unsigned char crc8;

int Read();
void Write(unsigned char byte);
uint8_t OW_TxRx (uint8_t* TxData);

#endif // _ONEWIREIO_H
