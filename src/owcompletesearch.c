#include "owcompletesearch.h"
#include "onewireio.h"

deviceAvail resetOW(){
  //uart send F0 9600baud
  //expect receive 0x10 to 0x80
  uint8_t* result ;
  uint8_t txdata = 0xF0;
  result =  OW_TxRx(&txdata);
  return DEVICE_AVAILABLE;
}
