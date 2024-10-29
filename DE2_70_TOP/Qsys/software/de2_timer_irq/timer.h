#ifndef TIMER_H_
#define TIMER_H_
/* nios2 system library */
#include <system.h> /* declare nios Ii peripheral function */
#include <sys/alt_irq.h>
#include <priv/alt_legacy_irq.h> /* define alt_irq_register() */
#include <stdio.h> /* declare standard I/O function */
#include <io.h>    /* declare I/O function */

/* Timer structure variable */
typedef struct {

    // for timer parameters
    unsigned int  uiBase;        /* timer address base */
    unsigned int  uiIRQ;         /* timer irq */
    unsigned int  uiFREQ;        /* timer frequency */
    float         fInterruptTime;/* interrupt time */
    unsigned int  uiTICnt;       /* count interrupt times */
    unsigned char ucTimeout;     /* timeout flag */
    unsigned char ucOverBounded; /* over bounded flag */
    unsigned char ucBusy;        /* when controller busy , this flag set to 1,
                                    Timer ISR check the flag, if this flag equal 1, then send a error */
    void          *ISRFun_p;     /* ISR function pointer for timer */
    void          *sNios_p;      /* store sNios_p pointer, declare (void) only for store address  */
    unsigned char ucLEDNiosLife; /* indicated Nios Status, every 0.5 second light on and off*/
} sTimer_s;


#endif /* TIMER_H_ */
