#include "unity.h"
#include "search.h"
#include "mock_onewireio.h"

#define TRUE  1
#define FALSE 0
#define NO_OF_DEVICE  4

/*constants to use with fake*/
uint8_t ROM [6][16] = {
{0x40, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},   //first group
{0x80, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},   //second group
{0xA0, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},   //third group
{0xA8, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},    //forth group
{0x94, 0xa6, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa},   //more complex first group
};


int result_bit;
int bytePos = 0;
unsigned char bitPos = 0x01;
int groupNum = 0;


int fake_Read(int numOfCalls){
  if(!LastDeviceFlag){
  if(bytePos<16){
    result_bit = ((ROM[groupNum][bytePos] & bitPos)>0);
    bitPos <<= 1;

    if(bitPos == 0){
      if(bytePos == 15){ //reset byte position
        bytePos = 0;
        groupNum++; //to next group of data
        bitPos = 1;
      }
      else{
        bytePos++;
        bitPos =1;
      }
    }

    return result_bit;
  }
    return -1;
  }
}

int fake_Write(int numOfCalls){
  printf("OW Write function is being called..\n");
}


void setUp(void)
{
  Read_StubWithCallback(fake_Read);
  Write_StubWithCallback(fake_Write);
}

void tearDown(void)
{
}


/*Given these data
 *
 *|0*61 (assume rest of 61 bit is 0) 1 0 0 0  (0x08)    -first data.                                        0000...1 0 0 0   <---- This path is chosen, data read is 0x08
 *                                   0 1 0 0            -second data                ->                      0000...0 1 0 0
 *                                   0 0 1 0            -third data      (after running through search)     0000...0 0 1 0
 *                                   0 0 0 1            -forth data                                         0000...0 0 0 1
                                                                                                                     ^
 *                                            LastDiscrepancy = 0                                                LastDiscrepancy
                                                                                              points to here so that next search will take the path with "1" (second path)
 *should read these data <true>:<compliment>
 *00 00 00 10 |01 01 01 01....
 *should put in these to return in fake
 *01000000  10101010  10101010  10101010  10101010   will send first bit(lsb) of first byte of ROM
 *ROM[0]     ROM[1]     ROM[2]    ROM[3]    ROM[4]  ...
 */

/*
 *----------------------------------------------------
 *FIRST
 *-----
 *Data retrieved: 0x08 (1.)
 *LastDiscrepancy = 3
 *LastDeviceFlag = FALSE
*/
void test_search_bit_expect_firstdata_LastDisprecancy_3(void)
{
  TEST_ASSERT_EQUAL(TRUE, bitSearch());
  TEST_ASSERT_EQUAL_INT64(0x08, ROM_NO[0]);
  TEST_ASSERT_EQUAL(3, LastDiscrepancy);
  TEST_ASSERT_EQUAL(FALSE, LastDeviceFlag);
}




/*Given these data
 *
 *|0*61 (assume rest of 61 bit is 0) 1 0 0 0  (0x08)    -first data.   (READ)                               0000...1 0 0 0
 *                                   0 1 0 0            -second data                ->                      0000...0 1 0 0    <---- This path is chosen, data read is 0x04
 *                                   0 0 1 0            -third data      (after running through search)     0000...0 0 1 0
 *                                   0 0 0 1            -forth data                                         0000...0 0 0 1
                                       ^                                                                               ^
 *                              LastDiscrepancy                                                                  LastDiscrepancy
                                                                                              points to here so that next search will take the path with "1" (second path)
 *
 *should read these data <true>:<compliment>
 *00 00 00 01 |01 01 01 01....
 *should put in these to return in fake
 *10000000  10101010  10101010  10101010  10101010   will send first bit(lsb) of first byte of ROM
 *ROM[0]     ROM[1]     ROM[2]    ROM[3]    ROM[4]  ...
 */

/*
 *----------------------------------------------------
 *NEXT
 *-----
 *Data to be retrieved: 0x04 (second data)
 *LastDiscrepancy = 2
 *LastDeviceFlag = FALSE
*/
void test_search_bit_expect_SecondData_LastDisprecancy_2(void)
{
  /*Test Initialize*/
  LastDiscrepancy = 3;
  LastDeviceFlag = FALSE;
  ROM_NO[0] = 0x08;
  TEST_ASSERT_EQUAL(TRUE, bitSearch());
  TEST_ASSERT_EQUAL_INT64(0x04, ROM_NO[0]);
  TEST_ASSERT_EQUAL(2, LastDiscrepancy);
  TEST_ASSERT_EQUAL(FALSE, LastDeviceFlag);
}


/*Given these data
 *
 *|0*61 (assume rest of 61 bit is 0) 1 0 0 0  (0x08)    -first data.(READ)                                  0000...1 0 0 0
 *                                   0 1 0 0            -second data(READ)          ->                      0000...0 1 0 0
 *                                   0 0 1 0            -third data        (after running through search)   0000...0 0 1 0    <---- This path is chosen, data read is 0x04
 *                                   0 0 0 1            -forth data                                         0000...0 0 0 1
                                         ^                                                                               ^
 *                                LastDiscrepancy                                                                  LastDiscrepancy
                                                                                              points to here so that next search will take the path with "1" (second path)
 *
 *should read these data <true>:<compliment>
 *00 00 01 01 |01 01 01 01....
 *should put in these to return in fake
 *10100000  10101010  10101010  10101010  10101010  will send first bit(lsb) of first byte of ROM
 *ROM[0]     ROM[1]     ROM[2]    ROM[3]    ROM[4]  ...
 */

/*
 *----------------------------------------------------
 *NEXT
 *-----
 *Data to be retrieved: 0x02 (third data)
 *LastDiscrepancy = 2
 *LastDeviceFlag = FALSE
*/
void test_search_bit_expect_ThirdData_LastDisprecancy_1(void)
{
  /*Test Initialize*/
  LastDiscrepancy = 2;
  LastDeviceFlag = FALSE;
  ROM_NO[0] = 0x04;
  TEST_ASSERT_EQUAL(TRUE, bitSearch());
  TEST_ASSERT_EQUAL_INT64(0x02, ROM_NO[0]);
  TEST_ASSERT_EQUAL(1, LastDiscrepancy);
  TEST_ASSERT_EQUAL(FALSE, LastDeviceFlag);
}


/*Given these data
 *
 *|0*61 (assume rest of 61 bit is 0) 1 0 0 0  (0x08)    -first data.(READ)                                  0000...1 0 0 0
 *                                   0 1 0 0            -second data.(READ)           ->                    0000...0 1 0 0
 *                                   0 0 1 0            -third data (READ) (after running through search)   0000...0 0 1 0
 *                                   0 0 0 1            -forth data                                         0000...0 0 0 1    <---- This path is chosen, data read is 0x04
                                           ^
 *                                  LastDiscrepancy                                                                LastDiscrepancy  = 0
                                                                                              points to zero so that it will return TRUE to LastDeviceFlag
 *
 *should read these data <true>:<compliment>
 *00 01 01 01 |01 01 01 01.... *Note:Lsb comes first
 *should put in these to return in fake
 *10101000  10101010  10101010  10101010  10101010  ...  will send first bit(lsb) of first byte of ROM
 *ROM[0]     ROM[1]     ROM[2]    ROM[3]    ROM[4]  ...
 */
/*
 *----------------------------------------------------
 *NEXT (LAST)
 *-----
 *Data to be retrieved: 0x02 (third data)
 *LastDiscrepancy = 2
 *LastDeviceFlag = FALSE
*/
void test_search_bit_expect_ForthData_LastDisprecancy_0(void)
{
  /*Test Initialize*/
  LastDiscrepancy = 1;
  LastDeviceFlag = FALSE;
  ROM_NO[0] = 0x02;
  TEST_ASSERT_EQUAL(TRUE, bitSearch());
  TEST_ASSERT_EQUAL_INT64(0x01, ROM_NO[0]);
  TEST_ASSERT_EQUAL(0, LastDiscrepancy);
  TEST_ASSERT_EQUAL(TRUE, LastDeviceFlag);
}

/* More complex data:
 * 000...0 1 1 0 1 0 1  --> firstData
 * 000...1 0 1 1 0 0 1  --> SecondData
 * 000...0 1 0 0 1 1 0  --> ThirdData
 *
 *expected OW read return: <true><Compliment>
 *00 10 10 01 01 10 01 (thirdData)
 *value to be put into array:
 *10010100 10100110 10101010 10101010 10101010...
 *{0x94, 0xC6, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};
 */

 void test_search_OW_expect_first_ThirdData(void){
   /*reset the variables*/
   LastDeviceFlag = FALSE;
   LastDiscrepancy=0;
   ROM_NO[0] = 0;
   TEST_ASSERT_EQUAL(TRUE, bitSearch());
   TEST_ASSERT_EQUAL_INT8(0x26, ROM_NO[0]); //0010 0110
   TEST_ASSERT_EQUAL(1, LastDiscrepancy);
   TEST_ASSERT_EQUAL(FALSE, LastDeviceFlag);
 }
