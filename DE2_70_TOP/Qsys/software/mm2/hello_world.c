/* ����@: LED������� */
#include <stdio.h>  /* �t�μзǿ�X�J��ܮw */
#include <system.h> /* �t�Ψ�ܮw */
#include <io.h>     /* �t��I/O��ܮw*/
#include <unistd.h> /* �ŧiusleep��� */

int main()
{
  long long int i=1;
  printf("Hello LED test!\n");
  while(1){
	 // if(IORD(BUTTOM_BASE,1)==1){
	  i++;
	  IOWR_32DIRECT(LED_BASE,0,0x01<<i); /* LED ���� */
     // }else{

         // IOWR(LED_BASE,0,0x01>>i); /* LED ���� */


     // }
	 usleep(100000);
      }
  return 0;
}
