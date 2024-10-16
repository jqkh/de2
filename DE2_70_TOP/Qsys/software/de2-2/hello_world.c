/* 實驗一: LED旋轉測試 */
#include <stdio.h>  /* 系統標準輸出入函示庫 */
#include <system.h> /* 系統函示庫 */
#include <io.h>     /* 系統I/O函示庫*/
#include <unistd.h> /* 宣告usleep函數 */
int sw=0,i=1;
long long int led=1;
int main()
{
  printf("Hello LED test!\n");
  while(1){

	  sw=IORD(SWITCH_BASE,0);
	  usleep(2000);
	  if((sw&0x02)==0)i++;
	  if(sw&0x02){
		 if(led==0x4000000)led=0x01;
		 led=led<<1;
	 }else{
		if(led==0x01)led=0x4000000;
		led=led>>1;
	 }
	 IOWR(LED_BASE,0,led);
	 usleep(50000);
  }
  return 0;
}
