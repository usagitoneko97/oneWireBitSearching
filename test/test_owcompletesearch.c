#include <stdint.h>
#include "unity.h"
#include "owcompletesearch.h"
#include "search.h"
#include "mock_onewireio.h"



uint8_t OW_TxRx_val;
unsigned char bitPos = 0;
uint8_t *fake_id_bits = NULL;
uint8_t *fake_cmp_id_bits = NULL;
int state_fakeRead = 0;

void init64BitId(uint8_t *id,uint8_t *cmp_id, uint8_t startBit) {
  fake_id_bits = id;
  fake_cmp_id_bits = cmp_id;
  bitPos = startBit;
}

int fake_Read(int numOfCalls){
    uint8_t result_bit;
    if(!LastDeviceFlag){
      while(bitPos < 64){
        switch (state_fakeRead) {
          case 0: result_bit = fake_id_bits[bitPos];
                  state_fakeRead = 1;
                  return result_bit;
                  break;
          case 1: result_bit = fake_cmp_id_bits[bitPos++];
                  state_fakeRead = 0;
                  return result_bit;
                  break;
        }
      }
  }
}

int fake_Write(int numOfCalls){
}

uint8_t fake_OW_TxRx(uint8_t* Txdata, int numOfCalls){
  return OW_TxRx_val;
}


void setUp(void){
  OW_TxRx_StubWithCallback(fake_OW_TxRx);
  Read_StubWithCallback(fake_Read);
  Write_StubWithCallback(fake_Write);
}

void tearDown(void){
  fake_id_bits = NULL;
  fake_cmp_id_bits = NULL;
}


void test_owcompletesearch_given_RX_F0_expect_DeviceNA(void)
{
  OW_TxRx_val = 0xf0;
  TEST_ASSERT_EQUAL(DEVICE_NA, resetOW());
}

/**
 * Expected 1byte return result
 * ---------------------------------
 * 0 1 0 0 1 0 1 1
 *       4b
 * 1 1 1 0 0 0 1 0
 *      e2
 * 0 1 1 0 0 1 1 1
 *       67fi
 */
void test_owcompletesearch_given_RX_10_given_above_number(void){
  uint8_t fake_id_bit_VAL []=       {0, 1, 0, 0, 0, 1, 1, 1,  0, 1, 0, 1, 0, 0, 1, 0,  0, 1, 0, 0, 0, 1, 1, 0};
  uint8_t fake_cmp_id_bit_VAL[] =   {0, 0, 1, 1, 1, 0, 0, 0,  0, 0, 0, 0, 1, 1, 0, 1,  0, 0, 0, 1, 1, 0, 0, 1};
  init64BitId(fake_id_bit_VAL, fake_cmp_id_bit_VAL, 0);
  OW_TxRx_val = 0x10;
  completeSearch_OW();
  TEST_ASSERT_EQUAL(0xe2, RomDataBuffer[0][0]);
  TEST_ASSERT_EQUAL(0x4b, RomDataBuffer[1][0]);
  TEST_ASSERT_EQUAL(TRUE, LastDeviceFlag);
  TEST_ASSERT_EQUAL(0, LastDiscrepancy);
  TEST_ASSERT_EQUAL_INT8(0x67, ROM_NO[0]);

}
