#include "search.h"
#include "onewireio.h"
/*Global variables*/
#define TRUE  1
#define FALSE 0



int bitSearch(){
  int id_bit_number;
  int last_zero, rom_byte_number, search_result;
  int id_bit, cmp_id_bit;
  unsigned char rom_byte_mask, search_direction;

  if(!LastDeviceFlag){

    /*Initialize*/
    id_bit_number = 1;
    last_zero = 0;
    rom_byte_number = 0;
    rom_byte_mask = 1;
    search_result = 0;
    crc8 = 0;

    Write(0xF0);
    do{
      id_bit = Read();
      cmp_id_bit = Read();
      if(id_bit == 1 && cmp_id_bit == 1)  //no devices
        break;
      else{
        if(id_bit != cmp_id_bit){
          search_direction = id_bit;
        }
        else{
          if(id_bit_number == LastDiscrepancy){
            search_direction = 1;
          }
          else if(id_bit_number > LastDiscrepancy){
            search_direction = 0;
          }
          else{
            search_direction = ((ROM_NO[byte_num] & rom_byte_mask)>0);  //if there is "1" on any bit, load 1 to search_direction
          }
          if(!search_direction){
            last_zero = id_bit_number;
            if(last_zero<9){
              LastFamilyDiscrepancy = last_zero;
            }

          }
        }
        if(search_direction == 1)
          ROM_NO[byte_num] |= rom_byte_mask; //set the current bit to be 1
        else
          ROM_NO[byte_num] &= ~rom_byte_mask; //set current bit to be 0
        //write()

        //preparation for next bit search
        id_bit_number++;
        rom_byte_mask <<=1;

        //checking of a complete byte

        if(rom_byte_mask == 0){
          rom_byte_mask = 1;
          byte_num++;
        }
      }

  }while(byte_num<8);
    //done searching
    //if successful
    if(id_bit_number == 65){
    LastDiscrepancy = last_zero;
    if(LastDiscrepancy == 0){
      LastDeviceFlag = TRUE;
    }
    search_result = TRUE;
    }
    //no device found (break from id_bit_number = cmp_id_bit =1)
  }
    /*last device flag is true*/
    if(!search_result){
      LastDiscrepancy = 0;
      LastDeviceFlag = FALSE;
      LastFamilyDiscrepancy = 0;
      search_result = FALSE;
    }
    return search_result;
}
