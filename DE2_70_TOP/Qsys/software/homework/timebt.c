#include <stdio.h>  /* �t�μз�I/O��ܮw */
#include <system.h> /* �t�μзǨ�ܮw */
#include <io.h>     /* �t��I/O��ܮw*/
#include <unistd.h> /* �ŧiusleep��� */
#include <altera_avalon_timer_regs.h> /* �ŧitimer��� */
#include "pio.h"
extern void PIOISRInitial(sPIO_s *sPIO ); /* decalare external initial PIO ISR function */
int hexdecode(int num);
extern void DoDisablePIOInterrupt(sPIO_s *spio);
extern void DoEnablePIOInterrupt(sPIO_s *spio);
void SetupTimer(float fSec){
    int CntValue;
    /* �]�wTimer�p�Ʈɶ� */
    CntValue = (int )(TIMER_FREQ*fSec);
    printf("cnt=%d\n",CntValue );
    IOWR_ALTERA_AVALON_TIMER_PERIODL(  /* ��TIMER_PERIODL�Ȧs�����g�J */
                          TIMER_BASE,  /* timer���򩳦�} */
                         (CntValue) & 0xffff);     /* ���XCntValue���C16bit, �g�JTimeout periodh register */
    IOWR_ALTERA_AVALON_TIMER_PERIODH(  /* ��TIMER_PERIODH�Ȧs�����g�J */
                          TIMER_BASE,  /* timer���򩳦�} */
                          (CntValue>>16) & 0xffff); /* ���XCntValue����16bit, �g�JTimeout periodh register */
}

int timer(void)
{
  int i=0, bt=0;
  printf("Timer example!\n");
  SetupTimer(0.001); /* �I�s�]�w�ɶ���� */

  while(1){
	while((IORD_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE) & 0x01)==0x00){
		  int sw=IORD(SWITCH_BASE,0);//�P�_switch�O�_����
			  if(sw!=4){return 0;}

	  if(IORD(BUTTOM_BASE,3)){
		 IOWR(BUTTOM_BASE,3,0);
		 bt=IORD(BUTTOM_BASE,0);

	  if((bt&8)==0){//�}�l
		  IOWR_ALTERA_AVALON_TIMER_CONTROL( /* ��TIMER_CONTROL�Ȧs�����g�J */
		          TIMER_BASE,                                /* timer���򩳦�} */
		          ALTERA_AVALON_TIMER_CONTROL_START_MSK |    /* �Ұʦ줸 */
		          ALTERA_AVALON_TIMER_CONTROL_CONT_MSK);     /* �s��줸 */
		  IOWR(TIMER_BASE, 0, 0);   /* clear TO bit in status register */
          /* Ū���çP�_ TO�줸 */

      }else if((bt&4)==0){//�Ȱ�
    	  printf("stop");
    	  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE,
    		ALTERA_AVALON_TIMER_CONTROL_STOP_MSK);
      }else if((bt&2)==0){//�k�s
    	i=0;
    	SetupTimer(0.001);
    	IOWR(HEX0_BASE,0,hexdecode(0));
	    IOWR(HEX1_BASE,0,hexdecode(0));
	    IOWR(HEX2_BASE,0,hexdecode(0));
	    IOWR(HEX3_BASE,0,hexdecode(0));
      	  }
	  }
	}
    i++;
    IOWR_ALTERA_AVALON_TIMER_STATUS( /* ��TIMER_STATUS�Ȧs�����g�J */
					 TIMER_BASE, /* timer���򩳦�} */
					 0);   /* clear TO bit in status register */
	IOWR(HEX0_BASE,0,hexdecode( i      %10));
	IOWR(HEX1_BASE,0,hexdecode((i/10)  %10));
	IOWR(HEX2_BASE,0,hexdecode((i/100) %10));
	IOWR(HEX3_BASE,0,hexdecode((i/1000)%10));
  }
  return 0;
}

int isrbt(void)/* C �D�{�� */
{
	int i=0;
  sPIO_s sPIO; /* �ŧiPIO ���c�ܼ�*/
  printf("PIO interrupt experimental!\n");
  PIOISRInitial(&sPIO); /* ��l��ISR��� */
  DoEnablePIOInterrupt(&sPIO);
  while(1){
		int sw=IORD(SWITCH_BASE,0);
		 if(sw!=2){
			 DoDisablePIOInterrupt(&sPIO);
			 return 0;
		 }
   if((sPIO.uiPressCnt&8)==0){
	   if(i==9)i=0;
	   else i++;
	   sPIO.uiPressCnt=0x0f;
   }else if((sPIO.uiPressCnt&4)==0){
	   if(i<=0)i=9;
	   else i--;
	   sPIO.uiPressCnt=0x0f;
   }else if((sPIO.uiPressCnt&2)==0){
	   i=0;
	   sPIO.uiPressCnt=0x0f;
   }
   IOWR(HEX3_BASE,0,hexdecode(i));
  }
  return 0;
}
int hexdecode(int num){
	 int data=0;
     if(num==0x0)data=0x3F;
else if(num==0x1)data=0x06;
else if(num==0x2)data=0x5B;
else if(num==0x3)data=0x4F;
else if(num==0x4)data=0x66;
else if(num==0x5)data=0x6D;
else if(num==0x6)data=0x7D;
else if(num==0x7)data=0x07;
else if(num==0x8)data=0x7F;
else if(num==0x9)data=0x6F;
else if(num==0xa)data=0x77;
else if(num==0xb)data=0x7C;
else if(num==0xc)data=0x39;
else if(num==0xd)data=0x5E;
else if(num==0xe)data=0x79;
else if(num==0xf)data=0x71;
     return 0xff-data;
}
