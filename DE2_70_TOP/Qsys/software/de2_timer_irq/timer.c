/*
 * timer.c
 *
 *  Created on: 2021/5/17
 *      Author: lishyhan
 */

#include "Timer.h"

//Enable Timer Interrupt Routine
void DoEnableTimerInterrupt(sTimer_s *sTimer);  /* enable timer interrupt */
void DoDisableTimerInterrupt(sTimer_s *sTimer); /* disable timer interrupt */
void TimerISRInitial(sTimer_s *sTimer, float fIntTime ); /* initial timer ISR function */
// adjust Freq base on phase
void TimerISR(sTimer_s *sTimer, unsigned int id); /* timer ISR */

//Enable Timer Interrupt Routine
void DoEnableTimerInterrupt(sTimer_s *sTimer)
{
    unsigned int wCLKValue; // interrupt time counter value
    int i;
#ifdef TIMER_NAME
    printf("[MSG][Timer]Find Timer0 and start initial timer interrupt.\n");
    printf("[MSG][Timer]Interrupt Time = %1.3f (s)\n", sTimer->fInterruptTime);

    wCLKValue = (unsigned int ) (sTimer->uiFREQ * sTimer->fInterruptTime); /* set Interrupt time */

    i = alt_irq_register( sTimer->uiIRQ,    /* register IRQ number */
                          sTimer,           /* pointer to any passed context */
                          sTimer->ISRFun_p);/* register ISR function */
    /* Setting timer interrupt time register */
    IOWR(sTimer->uiBase, 2,(short)(wCLKValue & 0x0000ffff)); /* get low 16-bits */
    IOWR(sTimer->uiBase, 3,(short)((wCLKValue >> 16) & 0x0000ffff));/* get high 16-bits */

    IOWR(sTimer->uiBase, 1, 7);  /* Set timer running, looping and interrupting */
    IOWR(sTimer->uiBase, 0, 0);  /* Clear Timeout flag */
    printf("[MSG][Timer]Timer interrupt is activated.\n");
#else
    printf("[MSG][Timer]Can't find Timer peripheral. \n");
#endif
}

void DoDisableTimerInterrupt(sTimer_s *sTimer)/* disable timer interrupt */
{
     alt_irq_register(sTimer->uiIRQ, 0, 0); /* disable timer */
     IOWR(sTimer->uiBase, 1, 0); /* Stop Timer Interrupt */
     printf("[MSG][Timer]Timer interrupt is disabled.\n");
}


void TimerISRInitial(sTimer_s *sTimer, float fIntTime ){
    // initial timer0
    #ifdef TIMER_NAME
        printf("[MSG][Timer]Find Timer and start initial timer0 process.\n");
        sTimer->uiBase = TIMER_BASE;  /* store timer base */
        sTimer->uiIRQ  = TIMER_IRQ;   /* store timer IRQ number */
        sTimer->uiFREQ = TIMER_FREQ;  /* store timer frequency */
        sTimer->fInterruptTime = fIntTime;   /* set interrupt time */
        sTimer->uiTICnt = 0;          /* set interrupt time counter */
        sTimer->ucTimeout = 0;        /* clear timeout */
        sTimer->ISRFun_p = TimerISR;  /* set interrupt ISR function */
        DoEnableTimerInterrupt(sTimer);      /* Enable interrupt */
        sTimer->ucLEDNiosLife = 0;  /* close LED */
    #else
        printf("[MSG][Timer]Can't find Timer.\n");
    #endif
}

void SetLed(sTimer_s *sTimer){
    if(((sTimer->uiTICnt % 500) ==0 )&&(sTimer->uiTICnt > 0)){
        if(sTimer->ucLEDNiosLife){
            sTimer->ucLEDNiosLife = 0; /* 4th LED off */
        }else {
            sTimer->ucLEDNiosLife = 1; /* 4th LED on */
        }
    }
}
// adjust Freq base on phase
void TimerISR(sTimer_s *sTimer, unsigned int id) {
    IOWR(sTimer->uiBase, 0, 0); /* Clear Timeout flag */
    sTimer->uiTICnt++;          /* accumulate interrupt times */

    sTimer->ucTimeout = 1;      /* set timeout flag */
    SetLed(sTimer);
}
