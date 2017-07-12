#include "owcompletesearch.h"
#include "onewireio.h"

#define VALIDATE_DEV  0
#define SEND_F0       1
#define 64BYTE_SEARCH 2

int state = 0;

deviceAvail resetOW(){
  //uart send F0 9600baud
  //expect receive 0x10 to 0x80
  uint8_t result ;
  uint8_t txdata = 0xF0;
  result =  OW_TxRx(&txdata);
  if(result == 0xf0)
    return DEVICE_NA;
  else  //TODO add additional condition
    return DEVICE_AVAILABLE;
}

void sendF0(){
  //f0
}

void mainFSM(){
  switch (state) {
    case VALIDATE_DEV:if(resetOW() == DEVICE_AVAILABLE){
                        state = SEND_F0
                      }
                      else{
                        //throw?
                        state = VALIDATE_DEV;
                      }
                      break;
    case SEND_F0:sendF0();
                 break;
  }
}
