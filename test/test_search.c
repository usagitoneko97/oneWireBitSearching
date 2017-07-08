#include "unity.h"
#include "search.h"
#include "mock_onewireio.h"

#define TRUE  1
#define FALSE 0
// #define NO_OF_DEVICE  4

unsigned char bitPos = 0x01;
int state = 0;
uint8_t *fake_id_bits = NULL;
uint8_t *fake_cmp_id_bits = NULL;

#define getBytePos(x)    ((x) >> 3)
#define getBitPos(x)     ((x) & 0x7)

void init64BitId(uint8_t *id,uint8_t *cmp_id, uint8_t startBit) {
  fake_id_bits = id;
  fake_cmp_id_bits = cmp_id;
  bitPos = startBit;
}

int fake_Read(int numOfCalls){
    uint8_t result_bit;
    if(!LastDeviceFlag){
      while(bitPos < 64){
        switch (state) {
          case 0: result_bit = fake_id_bits[bitPos];
                  state = 1;
                  return result_bit;
                  break;
          case 1: result_bit = fake_cmp_id_bits[bitPos++];
                  state = 0;
                  return result_bit;
                  break;
        }
      }
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

void tearDown(void) {
  fake_id_bits = NULL;
  fake_cmp_id_bits = NULL;
}

InnerVAR_OW initSearchTest(InnerVAR_OW innerVAR_OW){
  LastDiscrepancy = 0;
  LastDeviceFlag=FALSE;
  LastFamilyDiscrepancy = 0;
  int i = 0;
  while(i<8){
    ROM_NO[i++] = 0;
  }
  innerVAR_OW.id_bit_number = 1;
  innerVAR_OW.last_zero = 0;
  innerVAR_OW.rom_byte_num = 0;
  innerVAR_OW.rom_byte_mask = 1;
  innerVAR_OW.search_result = 0;
  innerVAR_OW.id_bit = -1;
  innerVAR_OW.cmp_id_bit = -1;
  innerVAR_OW.search_direction = 0;
  return innerVAR_OW;
}
/*Testing return value of fake_read is correct*/
void test_fake_read_return_idBit_cmpIdBit(void){
//  char id[]
  uint8_t fake_id_bit_VAL []=       {1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1};
  uint8_t fake_cmp_id_bit_VAL[] =   {0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);

  TEST_ASSERT_EQUAL(1, fake_id_bits[0]);
  TEST_ASSERT_EQUAL(0, fake_cmp_id_bits[0]);
  LastDeviceFlag = FALSE;
  TEST_ASSERT_EQUAL(1, fake_Read(NULL));
  TEST_ASSERT_EQUAL(0, fake_Read(NULL));

  TEST_ASSERT_EQUAL(1, fake_Read(NULL));
  TEST_ASSERT_EQUAL(1, fake_Read(NULL));

  TEST_ASSERT_EQUAL(0, fake_Read(NULL));
  TEST_ASSERT_EQUAL(1, fake_Read(NULL));
}

/********************************************************
 * Id_bit     = 0                                       *
 * cmp_id_bit = 0                                       *
 *                                                      *
 * EXPECTED:                                            *
 * last_zero = 1                                        *
 * id_bit_number ++                                     *
 * search_direction = id_bit                            *
 * first bit of first byte of ROM_NO = search_direction *
 ********************************************************/
void test_processOWData_IdBit_cmpBit_00(void){
  /*initialize test*/
  uint8_t fake_id_bit_VAL []=       {0};
  uint8_t fake_cmp_id_bit_VAL[] =   {0};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);

  InnerVAR_OW innerVAR_OW;
  innerVAR_OW = initSearchTest(innerVAR_OW);
  /*initialize condition for test*/
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);
  innerVAR_OW = processOWData(innerVAR_OW);

  /*checking results*/
  int ROM_bit_val = ROM_NO[0] &0x01; //the 0th bit
  TEST_ASSERT_EQUAL(1, innerVAR_OW.last_zero);
  TEST_ASSERT_EQUAL(2, innerVAR_OW.id_bit_number);
  TEST_ASSERT_EQUAL(0, innerVAR_OW.search_direction);
  TEST_ASSERT_EQUAL(0, ROM_bit_val);

}

/********************************************************
 * Id_bit     = 0                                       *
 * cmp_id_bit = 1                                       *
 *                                                      *
 * EXPECTED:                                            *
 * last_zero = 0                                        *
 * id_bit_number ++                                     *
 * search_direction = id_bit                            *
 * first bit of first byte of ROM_NO = search_direction *
 ********************************************************/
void test_processOWData_IdBit_cmpBit_01(void){
  /*initialize test*/
  uint8_t fake_id_bit_VAL []=       {0};
  uint8_t fake_cmp_id_bit_VAL[] =   {1};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);
  InnerVAR_OW innerVAR_OW;
  innerVAR_OW = initSearchTest(innerVAR_OW);
  /*initialize condition for test*/
  LastDiscrepancy = 0;
  innerVAR_OW = processOWData(innerVAR_OW);
  int ROM_bit_val = ROM_NO[0] &0x01; //the 0th bit
  /*checking results*/
  TEST_ASSERT_EQUAL(0, innerVAR_OW.last_zero);
  TEST_ASSERT_EQUAL(2, innerVAR_OW.id_bit_number);
  TEST_ASSERT_EQUAL(0, innerVAR_OW.search_direction);
  TEST_ASSERT_EQUAL(0, ROM_bit_val);

}

/********************************************************
 * Id_bit     = 1                                       *
 * cmp_id_bit = 0                                       *
 *                                                      *
 * EXPECTED:                                            *
 * last_zero = 0                                        *
 * id_bit_number ++                                     *
 * search_direction = id_bit                            *
 * first bit of first byte of ROM_NO = search_direction *
 ********************************************************/
void test_processOWData_IdBit_cmpBit_10(void){
  /*initialize test*/
  uint8_t fake_id_bit_VAL []=       {1};
  uint8_t fake_cmp_id_bit_VAL[] =   {0};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);
  InnerVAR_OW innerVAR_OW;
  innerVAR_OW = initSearchTest(innerVAR_OW);
  /*initialize condition for test*/
  LastDiscrepancy = 0;
  innerVAR_OW = processOWData(innerVAR_OW);
  /*checking results*/
  int ROM_bit_val = ROM_NO[0] &0x01;
  TEST_ASSERT_EQUAL(0, innerVAR_OW.last_zero);
  TEST_ASSERT_EQUAL(2, innerVAR_OW.id_bit_number);
  TEST_ASSERT_EQUAL(1, innerVAR_OW.search_direction);
  TEST_ASSERT_EQUAL(1, ROM_bit_val);
}

/********************************************************
 * Id_bit     = 0                                       *
 * cmp_id_bit = 0                                       *
 *                                                      *
 * EXPECTED:                                            *
 * last_zero = remains                                  *
 * id_bit_number = remains                              *
 * search_direction = remains                           *
 * search_result = FALSE                                *
 ********************************************************/
void test_processOWData_IdBit_cmpBit_11(void){
    /*initialize test*/
  uint8_t fake_id_bit_VAL []=       {1};
  uint8_t fake_cmp_id_bit_VAL[] =   {1};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);
  InnerVAR_OW innerVAR_OW;
  innerVAR_OW = initSearchTest(innerVAR_OW);
  /*initialize condition for test*/
  LastDiscrepancy = 0;
  innerVAR_OW = processOWData(innerVAR_OW);
    /*checking results*/
  int ROM_bit_val = ROM_NO[0] &0x01;
  TEST_ASSERT_EQUAL(0, innerVAR_OW.last_zero);
  TEST_ASSERT_EQUAL(1, innerVAR_OW.id_bit_number);
  TEST_ASSERT_EQUAL(0, innerVAR_OW.search_direction);
  TEST_ASSERT_EQUAL(FALSE, innerVAR_OW.search_result);
  TEST_ASSERT_EQUAL(0, ROM_bit_val);

}

/********************************************************
 * Id_bit     = 0                                       *
 * cmp_id_bit = 0                                       *
 * LastDiscrepancy = 1                                  *
 * id_bit_number = 1                                    *
 *                                                      *
 * EXPECT:                                              *
 * last_zero = 0                                        *
 * id_bit_number++                                      *
 * search_direction = 1                                 *
 * first bit of first byte of ROM_NO = search_direction *
 ********************************************************/
void test_processOWData_given_00_lastDiscrepency_sameAs_IDBitNumber_expect_searchDir_1(void){
  /*initialize test*/
  uint8_t fake_id_bit_VAL []=       {0};
  uint8_t fake_cmp_id_bit_VAL[] =   {0};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);
  InnerVAR_OW innerVAR_OW;
  innerVAR_OW = initSearchTest(innerVAR_OW);
  /*Initialize condition of test*/
  //id_bit = cmp_id_bit = 0
  LastDiscrepancy = 1;
  innerVAR_OW.id_bit_number = 1;
  /*checking results*/
  innerVAR_OW = processOWData(innerVAR_OW);
  int ROM_bit_val = ROM_NO[0] &0x01;
  TEST_ASSERT_EQUAL(1, ROM_bit_val);
  TEST_ASSERT_EQUAL(0, innerVAR_OW.last_zero);
  TEST_ASSERT_EQUAL(2, innerVAR_OW.id_bit_number);
  TEST_ASSERT_EQUAL(1, innerVAR_OW.search_direction);
}

/********************************************************
 * id_bit = 1                                           *
 * cmp_id_bit = 0                                       *
 * LastDiscrepancy = 3                                  *
 * id_bit_number = 1                                    *
 * ROM[0] |= 0x01 (set bi0 if ROM[0] to 1)              *
 *                                                      *
 * expect:                                              *
 * last_zero = 0                                        *
 * id_bit_number++                                      *
 * search_direction = 1                                 *
 * first bit of first byte of ROM_NO = search_direction *
 ********************************************************/
void test_processOWData_given_00_lastDiscrepency_biggerThan_IDBitNumber_expect_followBack_value_eq_1(void){
  /*initialize test*/
  uint8_t fake_id_bit_VAL []=       {1};
  uint8_t fake_cmp_id_bit_VAL[] =   {0};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);
  InnerVAR_OW innerVAR_OW;
  innerVAR_OW = initSearchTest(innerVAR_OW);
  /*Initialize condition of test*/
  //id_bit = cmp_id_bit = 0
  LastDiscrepancy = 3;
  innerVAR_OW.id_bit_number = 1;
  ROM_NO[0] |= 0x01;  //set bit 0 to '1'
  /*checking results*/
  innerVAR_OW = processOWData(innerVAR_OW);
  int ROM_bit_val = ROM_NO[0] &0x01;
  TEST_ASSERT_EQUAL(1, ROM_bit_val);
  TEST_ASSERT_EQUAL(0, innerVAR_OW.last_zero);
  TEST_ASSERT_EQUAL(2, innerVAR_OW.id_bit_number);
  TEST_ASSERT_EQUAL(1, innerVAR_OW.search_direction);

}

/********************************************************
 * id_bit = 1                                           *
 * cmp_id_bit = 0                                       *
 * LastDiscrepancy = 3                                  *
 * id_bit_number = 1                                    *
 * ROM[0] |= 0x01 (set bi0 if ROM[0] to 0)              *
 *                                                      *
 * expect:                                              *
 * last_zero = 0                                        *
 * id_bit_number++                                      *
 * search_direction = 0                                 *
 * first bit of first byte of ROM_NO = search_direction *
 ********************************************************/
void test_processOWData_given_00_lastDiscrepency_biggerThan_IDBitNumber_expect_followBack_ROM_NO_value_eq_0(void){
  /*initialize test*/
  uint8_t fake_id_bit_VAL []=       {0};
  uint8_t fake_cmp_id_bit_VAL[] =   {0};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);
  InnerVAR_OW innerVAR_OW;
  innerVAR_OW = initSearchTest(innerVAR_OW);
  /*Initialize condition of test*/
  //id_bit = cmp_id_bit = 0
  LastDiscrepancy = 3;
  innerVAR_OW.id_bit_number = 1;
  ROM_NO[0] &= 0xfe;  //set bit 0 to '0'
  /*checking results*/
  innerVAR_OW = processOWData(innerVAR_OW);
  int ROM_bit_val = ROM_NO[0] &0x01;
  TEST_ASSERT_EQUAL(0, ROM_bit_val);
  TEST_ASSERT_EQUAL(1, innerVAR_OW.last_zero);
  TEST_ASSERT_EQUAL(2, innerVAR_OW.id_bit_number);
  TEST_ASSERT_EQUAL(0, innerVAR_OW.search_direction);

}

/**
 * id_bit = 1
 * cmp_id_bit = 1
 *
 * expected:
 * _firstSearch(1) return FALSE
 */
void test_search_bit_given_idBit_cmp_idBit_11_expect_SearchFail(void)
{
  /*reset bit and byte pos in return value of OW  */
  uint8_t fake_id_bit_VAL []=       {1, 0, 1, 0, 1, 1, 0};
  uint8_t fake_cmp_id_bit_VAL[] =   {1, 0, 0, 1, 0, 0, 1};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);
  TEST_ASSERT_EQUAL(FALSE, _firstSearch(1));
}

/*Given these data
 *
 * 1 0 0 0  (0x08)    -first data.
 * 0 1 0 0            -second data
 * 0 0 1 0            -third data
 * 0 0 0 1            -forth data

 * LastDiscrepancy = 0

 *(after running through search)...

 *0000...1 0 0 0   <---- This path is chosen, data read is 0x08
 *0000...0 1 0 0
 *0000...0 0 1 0
 *0000...0 0 0 1
           ^
     LastDiscrepancy
points to here so that next search will take the path with "1" (second path)
 *should read these data <true>:<compliment>
 *00 00 00 10 |01 01 01 01....

 *----------------------------------------------------
 *FIRST
 *-----
 *Data retrieved: 0x08 (1.)
 *LastDiscrepancy = 3
 *LastDeviceFlag = FALSE
*/
void test_search_bit_expect_firstdata_LastDisprecancy_3(void)
{
  /*reset bit and byte pos in return value of OW  */
  uint8_t fake_id_bit_VAL []=       {0, 0, 0, 1, 0, 0, 0, 0};
  uint8_t fake_cmp_id_bit_VAL[] =   {0, 0, 0, 0, 1, 1, 1, 1};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);
  TEST_ASSERT_EQUAL(TRUE, _bitSearch(1));
  TEST_ASSERT_EQUAL_INT64(0x08, ROM_NO[0]);
  TEST_ASSERT_EQUAL(3, LastDiscrepancy);
  TEST_ASSERT_EQUAL(FALSE, LastDeviceFlag);
}




/*Given these data
 *
 *1 0 0 0  (0x08)    -first data.   (READ)
 *0 1 0 0            -second data
 *0 0 1 0            -third data
 *0 0 0 1            -forth data
 *  ^
*LastDiscrepancy
 *
*(after running through search).....

 0000...1 0 0 0
 0000...0 1 0 0    <---- This path is chosen, data read is 0x04
 0000...0 0 1 0
 0000...0 0 0 1
            ^
     LastDiscrepancy
points to here so that next search will take the path with "1" (second path)
 *
 *should read these data <true>:<compliment>
 *00 00 00 01 |01 01 01 01....
 *should put in these to return in fake
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
  uint8_t fake_id_bit_VAL []=       {0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t fake_cmp_id_bit_VAL[] =   {0, 0, 0, 1, 1, 1, 1, 1};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);

  LastDiscrepancy = 3;
  LastDeviceFlag = FALSE;
  ROM_NO[0] = 0x08;
  TEST_ASSERT_EQUAL(TRUE, _bitSearch(1));
  TEST_ASSERT_EQUAL_INT64(0x04, ROM_NO[0]);
  TEST_ASSERT_EQUAL(2, LastDiscrepancy);
  TEST_ASSERT_EQUAL(FALSE, LastDeviceFlag);
}


/*Given these data
 *
 *1 0 0 0  (0x08)    -first data.(READ)
 *0 1 0 0            -second data(READ)
 *0 0 1 0            -third data
 *0 0 0 1            -forth data
      ^
 * LastDiscrepancy
 *
 *(after running through search)  ...

 *0000...1 0 0 0
 *0000...0 1 0 0
 *0000...0 0 1 0    <---- This path is chosen, data read is 0x04
 *0000...0 0 0 1
               ^
       LastDiscrepancy
 *points to here so that next search will take the path with "1" (second path)
 *should read these data <true>:<compliment>
 *00 00 01 01 |01 01 01 01....
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
  uint8_t fake_id_bit_VAL []=       {0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t fake_cmp_id_bit_VAL[] =   {0, 0, 1, 1, 1, 1, 1, 1};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);

  LastDiscrepancy = 2;
  LastDeviceFlag = FALSE;
  ROM_NO[0] = 0x04;
  TEST_ASSERT_EQUAL(TRUE, _bitSearch(1));
  TEST_ASSERT_EQUAL_INT64(0x02, ROM_NO[0]);
  TEST_ASSERT_EQUAL(1, LastDiscrepancy);
  TEST_ASSERT_EQUAL(FALSE, LastDeviceFlag);
}


/*Given these data
 *
 *|0*61 (assume rest of 61 bit is 0) 1 0 0 0  (0x08)    -first data.(READ)
 *                                   0 1 0 0            -second data.(READ)
 *                                   0 0 1 0            -third data (READ)
 *                                   0 0 0 1            -forth data
                                           ^
 *                                  LastDiscrepancy

 *(after running through search)  ...
 *
 *0000...1 0 0 0
 *0000...0 1 0 0
 *0000...0 0 1 0
 *0000...0 0 0 1    <---- This path is chosen, data read is 0x04

       LastDiscrepancy  = 0
points to zero so that it will return TRUE to LastDeviceFlag
 *should read these data <true>:<compliment>
 *00 01 01 01 |01 01 01 01.... *Note:Lsb comes first
 *----------------------------------------------------
 *NEXT (LAST)
 *-----
 *Data to be retrieved: 0x02 (third data)
 *Expected:
 *LastDiscrepancy = 2
 *LastDeviceFlag = FALSE
*/
void test_search_bit_expect_ForthData_LastDisprecancy_0(void)
{
  /*Test Initialize*/
  uint8_t fake_id_bit_VAL []=       {0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t fake_cmp_id_bit_VAL[] =   {0, 1, 1, 1, 1, 1, 1, 1};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);

  LastDiscrepancy = 1;
  LastDeviceFlag = FALSE;
  ROM_NO[0] = 0x02;
  TEST_ASSERT_EQUAL(TRUE, _bitSearch(1));
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
 *00 10 10 01   01 10 01 (thirdData)
 *value to be put into array:
 */

 void test_search_OW_expect_first_dataThree(void){
   /*reset the variables*/
   uint8_t fake_id_bit_VAL []=       {0, 1, 1, 0, 0, 1, 0, 0};
   uint8_t fake_cmp_id_bit_VAL[] =   {0, 0, 0, 1, 1, 0, 1, 1};
   init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);

   TEST_ASSERT_EQUAL(TRUE, _firstSearch(1));
   TEST_ASSERT_EQUAL_INT8(0x26, ROM_NO[0]); //0010 0110
   TEST_ASSERT_EQUAL(1, LastDiscrepancy);
   TEST_ASSERT_EQUAL(FALSE, LastDeviceFlag);
 }

 /* More complex data:
  * 000...0 1 1 0 1 0 1  --> dataOne
  * 000...1 0 1 1 0 0 1  --> dataTwo
  * 000...0 1 0 0 1 1 0  --> dataThree
  *
  *expected OW read return: <true><Compliment>
  *00 01 00 10   10 01 10 (thirdData)
  *value to be put into array:
  */

  void test_search_OW_expect_second_dataTwo(void){
    /*reset the variables*/
    uint8_t fake_id_bit_VAL []=       {0, 0, 0, 1,  1, 0, 1, 0};
    uint8_t fake_cmp_id_bit_VAL[] =   {0, 1, 0, 0,  0, 1, 0, 1};
    init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);

    LastDeviceFlag = FALSE;
    LastDiscrepancy=1;
    ROM_NO[0] = 0x26;
    TEST_ASSERT_EQUAL(TRUE, _bitSearch(1));
    TEST_ASSERT_EQUAL_INT8(0x59, ROM_NO[0]);
    TEST_ASSERT_EQUAL(3, LastDiscrepancy);
    TEST_ASSERT_EQUAL(FALSE, LastDeviceFlag);
  }

  /* More complex data:
   * 000...0 1 1 0 1 0 1  --> dataOne
   * 000...1 0 1 1 0 0 1  --> dataTwo
   * 000...0 1 0 0 1 1 0  --> dataThree
   *
   *expected OW read return: <true><Compliment>
   *00 01 00 01  10 10 01 (thirdData)
   */

   void test_search_OW_expect_third_dataOne(void){
     /*reset the variables*/
     uint8_t fake_id_bit_VAL []=       {0, 0, 0, 0,  1, 1, 0, 0};
     uint8_t fake_cmp_id_bit_VAL[] =   {0, 1, 0, 1,  0, 0, 1, 1};
     init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);

     LastDeviceFlag = FALSE;
     LastDiscrepancy=3;
     ROM_NO[0] = 0x59;
     TEST_ASSERT_EQUAL(TRUE, _bitSearch(1));
     TEST_ASSERT_EQUAL_INT8(0x35, ROM_NO[0]);
     TEST_ASSERT_EQUAL(0, LastDiscrepancy);
     TEST_ASSERT_EQUAL(TRUE, LastDeviceFlag);
   }

   /*Target Setup search
   *Given these data
   * 000...0 1 0 1  1 1 0 0 0 1 0 1 --> dataOne   <=====choosen first
   * 000...1 0 1 1  1 1 0 0 0 1 0 1--> dataTwo
   * 000...1 0 0 1  0 0 1 0 0 1 1 0 --> dataThree
   * The only difference is the first 4 bit
   *10 01 10 01  00 01 10 10   10 00 10 01
   */

   void test_targetSetupSearch_givenAboveData_expect_dataOne(void){
     uint8_t fake_id_bit_VAL []=       {1, 0, 1, 0,  0, 0, 1, 1,  1, 0, 1, 0,  0, 0, 0, 0};
     uint8_t fake_cmp_id_bit_VAL[] =   {0, 1, 0, 1,  0, 1, 0, 0,  0, 0, 0, 1,  1, 1, 1, 1};
     init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);

     LastDiscrepancy = 64;
     LastFamilyDiscrepancy = 0;
     LastDeviceFlag = FALSE;
     ROM_NO[0] = 0xc5; //family code
     TEST_ASSERT_EQUAL(TRUE, _firstSearch(2));
     TEST_ASSERT_EQUAL(10, LastDiscrepancy);
     TEST_ASSERT_EQUAL(FALSE, LastDeviceFlag);
     TEST_ASSERT_EQUAL(0xc5, ROM_NO[0]);
     TEST_ASSERT_EQUAL(0x05, ROM_NO[1]);
   }

   //TODO the rest of targetSetup search
   //TODO test on familyskipSetup and verify
