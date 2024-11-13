#include <stdio.h>  /* 系統標準I/O函示庫 */
#include <system.h> /* 系統標準函示庫 */
#include <io.h>     /* 系統I/O函示庫*/
#include <unistd.h> /* 宣告usleep函數 */
#include <altera_avalon_timer_regs.h> /* 宣告timer函數 */
#include "pio.h"
extern void PIOISRInitial(sPIO_s *sPIO ); /* decalare external initial PIO ISR function */
int hexdecode(int num);
extern void DoDisablePIOInterrupt(sPIO_s *spio);
extern void DoEnablePIOInterrupt(sPIO_s *spio);
void SetupTimer(float fSec){
    int CntValue;
    /* 設定Timer計數時間 */
    CntValue = (int )(TIMER_FREQ*fSec);
    printf("cnt=%d\n",CntValue );
    IOWR_ALTERA_AVALON_TIMER_PERIODL(  /* 對TIMER_PERIODL暫存器做寫入 */
                          TIMER_BASE,  /* timer的基底位址 */
                         (CntValue) & 0xffff);     /* 取出CntValue的低16bit, 寫入Timeout periodh register */
    IOWR_ALTERA_AVALON_TIMER_PERIODH(  /* 對TIMER_PERIODH暫存器做寫入 */
                          TIMER_BASE,  /* timer的基底位址 */
                          (CntValue>>16) & 0xffff); /* 取出CntValue的高16bit, 寫入Timeout periodh register */
}

int timer(void)
{
  int i=0, bt=0;
  printf("Timer example!\n");
  SetupTimer(0.001); /* 呼叫設定時間函數 */

  while(1){
	while((IORD_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE) & 0x01)==0x00){
		  int sw=IORD(SWITCH_BASE,0);//判斷switch是否撥動
			  if(sw!=4){return 0;}

	  if(IORD(BUTTOM_BASE,3)){
		 IOWR(BUTTOM_BASE,3,0);
		 bt=IORD(BUTTOM_BASE,0);

	  if((bt&8)==0){//開始
		  IOWR_ALTERA_AVALON_TIMER_CONTROL( /* 對TIMER_CONTROL暫存器做寫入 */
		          TIMER_BASE,                                /* timer的基底位址 */
		          ALTERA_AVALON_TIMER_CONTROL_START_MSK |    /* 啟動位元 */
		          ALTERA_AVALON_TIMER_CONTROL_CONT_MSK);     /* 連續位元 */
		  IOWR(TIMER_BASE, 0, 0);   /* clear TO bit in status register */
          /* 讀取並判斷 TO位元 */

      }else if((bt&4)==0){//暫停
    	  printf("stop");
    	  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE,
    		ALTERA_AVALON_TIMER_CONTROL_STOP_MSK);
      }else if((bt&2)==0){//歸零
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
    IOWR_ALTERA_AVALON_TIMER_STATUS( /* 對TIMER_STATUS暫存器做寫入 */
					 TIMER_BASE, /* timer的基底位址 */
					 0);   /* clear TO bit in status register */
	IOWR(HEX0_BASE,0,hexdecode( i      %10));
	IOWR(HEX1_BASE,0,hexdecode((i/10)  %10));
	IOWR(HEX2_BASE,0,hexdecode((i/100) %10));
	IOWR(HEX3_BASE,0,hexdecode((i/1000)%10));
  }
  return 0;
}

int isrbt(void)/* C 主程式 */
{
	int i=0;
  sPIO_s sPIO; /* 宣告PIO 結構變數*/
  printf("PIO interrupt experimental!\n");
  PIOISRInitial(&sPIO); /* 初始化ISR函數 */
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
