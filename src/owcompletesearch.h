#ifndef _OWCOMPLETESEARCH_H
#define _OWCOMPLETESEARCH_H

#define RESET_OW   1
#define SEND_F0    2
#define BITSEARCH  3

#define SEND_0    0x0
#define SEND_1    0xff
typedef enum {
  DEVICE_AVAILABLE = 0,
  DEVICE_NA = 1
}deviceAvail;

deviceAvail resetOW();
#endif // _OWCOMPLETESEARCH_H
