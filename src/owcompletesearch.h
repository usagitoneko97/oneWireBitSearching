#ifndef _OWCOMPLETESEARCH_H
#define _OWCOMPLETESEARCH_H

#define RESET_OW   1
#define BITSEARCH  2
typedef enum {
  DEVICE_AVAILABLE = 0,
  DEVICE_NA = 1
}deviceAvail;

deviceAvail resetOW();
#endif // _OWCOMPLETESEARCH_H
