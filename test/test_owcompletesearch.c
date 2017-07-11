#include "unity.h"
#include "owcompletesearch.h"
#include "search.h"
#include "mock_onewireio.h"

uint8_t fake_OW_TxRx(uint8_t* Txdata, int numOfCalls){
  return 0xf0;
}


void setUp(void){
  OW_TxRx_StubWithCallback(fake_OW_TxRx);
}

void tearDown(void){}


void test_owcompletesearch_given_RX_F0_expect_DeviceNA(void)
{
  TEST_ASSERT_EQUAL(DEVICE_NA, resetOW());
}
