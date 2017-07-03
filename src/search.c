#include "search.h"

/*Global variables*/
unsigned char ROM_NO[8];
int LastDiscrepancy;
int LastFamilyDiscrepancy;
int LastDeviceFlag;
int byte_num;
int byte_mask;

int search(){
  int id_bit_number;
  int last_zero, rom_byte_number, search_result;
  int id_bit, cmp_id_bit;
  unsigned char rom_byte_mask, search_direction;

  if(LastDeviceFlag){

  }
  else{
    /*Initialize*/
    id_bit_number = 1;
    last_zero = 0;

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
            search_direction = ((ROM_NO[byte_num] & byte_mask)>0);  //if there is "1" on any bit, load 1 to search_direction
          }
          if(!search_direction){
            last_zero = id_bit_number;
            if(last_zero<9){
              LastFamilyDiscrepancy = last_zero;
            }

          }
        }
        ROM_NO[id_bit_number] = search_direction;
        //write()
        id_bit_number++;
      }

  }while(byte_num<8);
  }
}
