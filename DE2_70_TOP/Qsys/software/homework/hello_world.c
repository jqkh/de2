#include <stdio.h>  /* ╰参夹非I/Oㄧボ畐 */
#include <system.h> /* ╰参夹非ㄧボ畐 */
#include <io.h>     /* ╰参I/Oㄧボ畐*/
#include <unistd.h> /* usleepㄧ计 */

extern int timer(void);
extern int isrbt(void);
extern int hexdecode(int num);

int main()
{
  IOWR(HEX0_BASE,0,hexdecode(0));
  IOWR(HEX1_BASE,0,hexdecode(0));
  IOWR(HEX2_BASE,0,hexdecode(0));
  IOWR(HEX3_BASE,0,hexdecode(0));
  printf("Hello from Nios II!\n");
  int i1=0,i2=0,i3=0,i4=0;
  while(1){
	  int sw=IORD(SWITCH_BASE,0);
	  int bt=IORD(BUTTOM_BASE,0);
	  switch(sw){
	  case 1:
		if(IORD(BUTTOM_BASE,3)){
		 if((bt&0x08)==0){
			IOWR(BUTTOM_BASE,3,0);
			if(i1>=15)i1=0;
			else i1++;
			IOWR(HEX0_BASE,0,hexdecode(i1));
			IOWR(HEX1_BASE,0,hexdecode(i2));
			IOWR(HEX2_BASE,0,hexdecode(i3));
			IOWR(HEX3_BASE,0,hexdecode(i4));
			i4=i3;
			i3=i2;
			i2=i1;
			printf("%d",i1);
		 }
		}
		break;
	  case 2:
		  IOWR(HEX0_BASE,0,hexdecode(0));
		  IOWR(HEX1_BASE,0,hexdecode(0));
		  IOWR(HEX2_BASE,0,hexdecode(0));
		  IOWR(HEX3_BASE,0,hexdecode(0));
		  printf("い耞计\n");
		  isrbt();
		  break;
	  case 4:
		  IOWR(HEX0_BASE,0,hexdecode(0));
		  IOWR(HEX1_BASE,0,hexdecode(0));
		  IOWR(HEX2_BASE,0,hexdecode(0));
		  IOWR(HEX3_BASE,0,hexdecode(0));
		  printf("璸竟\n");
		  timer();
		  break;
	  }


  }
  return 0;
}
