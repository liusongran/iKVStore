#ifndef MSP430F5529_PROFILE_H_
#define MSP430F5529_PROFILE_H_
#include "mcu.h"
#include <stdio.h>

#define PRB_START(start)    \
        Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);\
        start = Timer_A_getCounterValue(TIMER_A0_BASE)

#define PRB_END(end)    \
        end =  Timer_A_getCounterValue(TIMER_A0_BASE);\
        Timer_A_stop(TIMER_A0_BASE);\
        Timer_A_clear(TIMER_A0_BASE)

int fputc(int _c, register FILE *_fp);
int fputs(const char *_ptr, register FILE *_fp);

#endif /* MSP430F5529_PROFILE_H_ */
