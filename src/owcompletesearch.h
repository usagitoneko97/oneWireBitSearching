#ifndef _OWCOMPLETESEARCH_H
#define _OWCOMPLETESEARCH_H
#include <stdint.h>

#define SEND_0    0x0
#define SEND_1    0xff
#define RESET     0
#define SENDF0_DATA  {SEND_1, SEND_1, SEND_1,SEND_1, SEND_0, SEND_0, SEND_0, SEND_0}
//uint8_t sendF0_txData[] = {SEND_1, SEND_1, SEND_1,SEND_1, SEND_0, SEND_0, SEND_0, SEND_0};
int state; //initiate to reset_OW

typedef enum {
  DEVICE_AVAILABLE = 0,
  DEVICE_NA = 1
}deviceAvail;

typedef enum{
  RESET_OW =0,
  REPLY = 1,
  SEND_F0 = 2,
  BITSEARCH = 3
}Event;
int search_SM();
void OW_Tx_SendArray(uint8_t* data, int length);
deviceAvail resetOW();
int completeSearch_OW();
#endif // _OWCOMPLETESEARCH_H
