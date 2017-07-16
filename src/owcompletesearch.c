#include "owcompletesearch.h"
#include "onewireio.h"
#include "search.h"

int state = RESET_OW; //initiate to reset_OW
int result_reset;
uint8_t result ;
uint8_t txdata;
uint8_t sendF0_txData[] = {SEND_1, SEND_1, SEND_1,SEND_1, SEND_0, SEND_0, SEND_0, SEND_0};

deviceAvail resetOW(){
  //uart send F0 9600baud
  //expect receive 0x10 to 0x80
  txdata = 0xF0;
  result =  OW_TxRx(&txdata);
  if(result == 0xf0)
    return DEVICE_NA;
  else  //TODO add additional condition
    return DEVICE_AVAILABLE;
}

int search_OW(){
  switch (state) {
    case RESET_OW:

        result_reset = resetOW();
        if(result_reset == DEVICE_AVAILABLE){
          state = SEND_F0;
          return TRUE;
        }
          else if(result_reset == DEVICE_NA)
          return FALSE;
        break;

      case SEND_F0:
          OW_TxRx(sendF0_txData);
          state = BITSEARCH;
          break;
      case BITSEARCH:
          if(_firstSearch(1)== FALSE){
            return FALSE;
          }
          while(LastDeviceFlag != TRUE){
            if(_bitSearch(1) == FALSE)
              return FALSE;
          }
            return TRUE;
            state = RESET_OW;
  }
}

void completeSearch_OW(){
  int count = 0;
  while(count<3){

    if(search_OW()==FALSE)
      break;
    count++;
  }
}
