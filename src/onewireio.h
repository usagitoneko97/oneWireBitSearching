#ifndef _ONEWIREIO_H
#define _ONEWIREIO_H

unsigned char ROM_NO[8];
int LastDiscrepancy;
int LastFamilyDiscrepancy;
int LastDeviceFlag;
int byte_num;
unsigned char crc8;

int Read();
void Write(unsigned char byte);

#endif // _ONEWIREIO_H
