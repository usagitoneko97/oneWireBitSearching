#include <stdint.h>

#ifndef _ONEWIREIO_H
#define _ONEWIREIO_H

/*Global variables*/
#define TRUE  1
#define FALSE 0
#define MAX_OWDEVICE  64

unsigned char ROM_NO[8];
unsigned char RomDataBuffer[MAX_OWDEVICE][8];
int bufferByteNumber;
int bufferDeviceNumber;
int LastDiscrepancy;
int LastFamilyDiscrepancy;
int LastDeviceFlag;
unsigned char crc8;

int Read();
void Write(unsigned char byte);
uint8_t OW_TxRx (uint8_t* TxData);

#endif // _ONEWIREIO_H
