/*
    ����2: �Q��Timer�ӭp��0.001���ͤ@�����_�C�p��500����NLED����C
*/
#include <stdio.h>
#include <system.h> /* �t�μзǨ�ܮw */
#include <io.h>     /* �t��I/O��ܮw*/
#include <unistd.h> /* �ŧiusleep��� */
#include "timer.h" /* �ŧitimer��� */

extern TimerISRInitial(sTimer_s *sTimer, float fIntTime ); /* decalare external initial timer ISR function */
extern DoEnableTimerInterrupt(sTimer_s *sTimer);  /* declare extern enable timer interrupt */

int main()/* C �D�{�� */
{
  sTimer_s stimer; /* �ŧitimer ���c�ܼ�*/
  printf("Timer interrupt experimental!\n");
  TimerISRInitial(&stimer, 0.001); /* ��l��ISR��ƻP�]�w���_�ɶ� */
  while(1){
      IOWR(LED_BASE, 0, stimer.ucLEDNiosLife);
  }
  return 0;
}
