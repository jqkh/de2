/*
    龟喷2: ノTimerㄓ璸计0.001玻ネΩい耞璸计500Ω盢LEDは锣
*/
#include <stdio.h>
#include <system.h> /* ╰参夹非ㄧボ畐 */
#include <io.h>     /* ╰参I/Oㄧボ畐*/
#include <unistd.h> /* usleepㄧ计 */
#include "timer.h" /* timerㄧ计 */

extern TimerISRInitial(sTimer_s *sTimer, float fIntTime ); /* decalare external initial timer ISR function */
extern DoEnableTimerInterrupt(sTimer_s *sTimer);  /* declare extern enable timer interrupt */

int main()/* C 祘Α */
{
  sTimer_s stimer; /* timer 挡篶跑计*/
  printf("Timer interrupt experimental!\n");
  TimerISRInitial(&stimer, 0.001); /* ﹍てISRㄧ计籔砞﹚い耞丁 */
  while(1){
      IOWR(LED_BASE, 0, stimer.ucLEDNiosLife);
  }
  return 0;
}
