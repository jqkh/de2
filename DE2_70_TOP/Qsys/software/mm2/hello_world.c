/* 實驗一: LED旋轉測試 */
#include <stdio.h>  /* 系統標準輸出入函示庫 */
#include <system.h> /* 系統函示庫 */
#include <io.h>     /* 系統I/O函示庫*/
#include <unistd.h> /* 宣告usleep函數 */

int main()
{
  long long int i=1;
  printf("Hello LED test!\n");
  while(1){
	 // if(IORD(BUTTOM_BASE,1)==1){
	  i++;
	  IOWR_32DIRECT(LED_BASE,0,0x01<<i); /* LED 左移 */
     // }else{

         // IOWR(LED_BASE,0,0x01>>i); /* LED 左移 */


     // }
	 usleep(100000);
      }
  return 0;
}
