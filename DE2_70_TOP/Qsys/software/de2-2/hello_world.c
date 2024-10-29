#include <stdio.h>  /* 系統標準I/O函示庫 */
#include <system.h> /* 系統標準函示庫 */
#include <io.h>     /* 系統I/O函示庫*/
#include <unistd.h> /* 宣告usleep函數 */
#include <altera_avalon_timer_regs.h> /* 宣告timer函數 */

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

int main()/* C 主程式 */
{
  int i,bt=0,count=0;
  printf("Timer example!\n");
  SetupTimer(0.1); /* 呼叫設定時間函數 */
  while(1){
      for(i=0;i<27;i++){

          while((IORD_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE) & 0x01)==0x00){/* 讀取並判斷 TO位元 */
        	  bt=IORD(BUTTOM_BASE,0);
        	  	  if((bt&0x08)==0){

        	  		  IOWR_ALTERA_AVALON_TIMER_CONTROL( /* 對TIMER_CONTROL暫存器做寫入 */
        	  		            TIMER_BASE,                                /* timer的基底位址 */
        	  		            ALTERA_AVALON_TIMER_CONTROL_START_MSK |    /* 啟動位元 */
        	  		            ALTERA_AVALON_TIMER_CONTROL_CONT_MSK);     /* 連續位元 */
        	  		    IOWR(TIMER_BASE, 0, 0);   /* clear TO bit in status register */
        	  	  }else if((bt&0x04)==0){

        	  		  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE,
        	  				  ALTERA_AVALON_TIMER_CONTROL_STOP_MSK);
        	  	  }else if((bt&0x02)==0){

        	  		 count=0;
        	  	  }else if(count>=9999)count=0;

          }
          count++;
          printf("%d\n",count);
        	  IOWR_ALTERA_AVALON_TIMER_STATUS( /* 對TIMER_STATUS暫存器做寫入 */
                            TIMER_BASE, /* timer的基底位址 */
                            0);   /* clear TO bit in status register */
//          usleep(100000);           /* 延遲 */
      }
  }
  return 0;
}
