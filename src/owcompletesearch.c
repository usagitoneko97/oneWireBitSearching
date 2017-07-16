#include "owcompletesearch.h"
#include "onewireio.h"
#include "search.h"

int state = RESET_OW; //initiate to reset_OW
int result_reset;

#define VALIDATE_DEV  0
#define SEND_F0       1
#define BYTE64_SEARCH 2

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

int search_OW(){
  switch (state) {
    case RESET_OW:

        result_reset = resetOW();
        if(result_reset == DEVICE_AVAILABLE){
          state = BITSEARCH;
          return TRUE;
        }
          else if(result_reset == DEVICE_NA)
          return FALSE;
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
  while(count<2){

    if(search_OW()==FALSE)
      break;
    count++;
  }
}
