/* ����@: LED������� */
#include <stdio.h>  /* �t�μзǿ�X�J��ܮw */
#include <system.h> /* �t�Ψ�ܮw */
#include <io.h>     /* �t��I/O��ܮw*/
#include <unistd.h> /* �ŧiusleep��� */
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
