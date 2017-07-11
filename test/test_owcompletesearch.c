#include "unity.h"
#include "owcompletesearch.h"
#include "mock_onewireio.h"

uint8_t* fake_OW_TxRx(uint8_t* Txdata, int numOfCalls){
  uint8_t* Rxdata;
  *Rxdata = 0xf0;
  return Rxdata;
}


void setUp(void){
  OW_TxRx_StubWithCallBack(fake_OW_TxRx);
}

void tearDown(void){}


void test_owcompletesearch_given_RX_F0_expect_DeviceNA(void)
{
  resetOW();
}
