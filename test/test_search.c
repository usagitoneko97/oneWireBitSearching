#include "unity.h"
#include "search.h"
#include "mock_onewireio.h"

#define TRUE  1
#define FALSE 0

/*constants to use with fake*/
uint8_t ROM_1[16] = {
  0x40, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
};

uint8_t ROM_2[16] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,
};

int result_bit;
int bytePos = 0;
unsigned char bitPos = 0x01;


int fake_Read(int numOfCalls){
  printf("OW Read function is being called..\n");

  if(!LastDeviceFlag){
  if(bytePos<16){
    printf("ROM_1[bytePos] bitpos = %d\n",(ROM_1[bytePos] & bitPos));
    result_bit = ((ROM_1[bytePos] & bitPos)>0);
    bitPos <<= 1;
    if(bitPos == 0){
      bytePos++;
      bitPos =1;
    }
    return result_bit;
  }
    return -1;
  }
  printf("Read function is been called for %d times\n",numOfCalls );
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
 *|0*61 (assume rest of 61 bit is 0) 1 0 0 0  (0x08)
 *                                   0 1 0 0
 *                                   0 0 1 0
 *                                   0 0 0 1

 *should read these data <true>:<compliment>
 *00 00 00 10 |01*61
 *should put in these to return in fake
 *01000000  10101010  10101010  10101010  10101010  ...
 *ROM_1[0]  ROM_1[1]  ROM_1[2]  ROM_1[3]  ROM_1[4]  ...
*/
void test_search_bit_expect_0x08_LastDisprecancy_3(void)
{
  TEST_ASSERT_EQUAL(TRUE, bitSearch());
  printf("last discrepancy = %d\n",LastDiscrepancy );
  TEST_ASSERT_EQUAL_INT64(0x08, ROM_NO[0]);
  TEST_ASSERT_EQUAL(3, LastDiscrepancy);
}
