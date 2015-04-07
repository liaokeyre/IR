/*--------------------------------------------------------------------------------
  遥控器处理程序
  算法：测两个下降沿的的时间，作判断 0？  1？，遇到引导码，计数清0

---------------------------------------------------------------------------------*/

#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long	

extern show(uchar showdata); 

volatile ulong IRcode=0x00000000;  //32位的键代码

volatile uint customcode=0x0000;      //16位用户码
volatile uint time_us=0x0000;      //两个下降沿之间的时间	

volatile uchar timeH,timeL;	 //保存TH1 TL的值

uchar Lcustomcode=0x00;      //低8用户码
uchar Hcustomcode=0x00;     //高8
uchar datacode=0x00;	   //8位键数据码
uchar Rdatacode=0x00;      //8位键数据反码
uchar T1overflow=0x00;   //T1溢出次数 	？？？？？？

sbit IR=P3^2;	//？？  
sbit power=P1^7;  //电源开关
bit NewIRcode=0;  //指示当处理完了32位码后，就有了新的遥控码
bit DataRight=0;   //为1时读取数据正确 


//-----------------------------delay()-------------------------
void delay()
{
 uchar i,j,k;
 for(i=200;i>0;i--)
    for(j=200;j>0;j--)
	   for(k=22;k>0;k--) ;	
}

void delay2()
{
 uchar i,j,k;
 for(i=200;i>0;i--)
    for(j=200;j>0;j--)
	   for(k=13;k>0;k--);
}
//------------------------------------------------------------------------------
void init()		
{

  
  TMOD=0x11; //定时器0，工作方式1 ;  定时器1，工作方式1
  TCON=0x01; //外中断0下降沿触发，（包括TR1=0，TR0=0）
  TH0=0x3c;	 //初始化定时器0,定时50ms
  TL0=0xb0;
  TH1=0x00;	 //初始化定时器1 
  TL1=0x00;	   

  EA=1;		  //开全中断
  ET0=1;	  //开放T0中断
  ET1=1;      //开放T1中断
  EX0=1;	  //开放INT0
  IP=0x01;   //?????  
}

//-------------------------------------interrupt0-------------------------------------
void IR_ISR() interrupt 0 using 1	//遥控器中断处理函数
{   
  static uchar cn=0;   
 
  TR1=0;	 
  timeH=TH1;
  timeL=TL1;
  TH1=0;
  TL1=0;
  TR1=1;
  
  time_us=(unsigned int)timeH;
  time_us=time_us<<8;
  time_us=time_us|timeL;

  if(time_us>13300&&time_us<13700) {cn=1;IRcode=0;}  //遇到引导码,就把cn清0，IRcode清0	
   
  if(cn<34)
  {
  if(time_us>1000&&time_us<1300) //0  
          {IRcode=IRcode|0x00000000; if(cn<33) IRcode=IRcode>>1;}

  else if(time_us>2000&&time_us<2450) //1	 
          {IRcode=IRcode|0x80000000;if(cn<33) IRcode=IRcode>>1;}
  }
		  
  else if(time_us>11000&&time_us<11500) {IRcode=IRcode;}; //遇到重复码
     																							
   cn++; 

  if(cn==34) {NewIRcode=1;TR1=0;}  //读完32位码，则有新码产生 

}

//--------------------------------------timer_ISR------------------------------
void Timer0_ISR() interrupt 1 using 2		//定时器0中断函数
{
	//TR1=0;	
	//TR1=1; 
}
void Timer1_ISR() interrupt 3 using 3		//定时器1中断函数
{
   T1overflow++;	 //还要记得清0？？？？
   //？？？++；放比如音量，切换时间等的递增量？？？？？？？？？？？？？？？？？？？？？
   //TH1=0xff;   //这三句可以不要,定时器可以不断的启动计数
   //TL1=0xff;
   //TR1=0;	//???
}
				   
//------------------------------------main()----------------------------------------------
void main()
{	
    
    init();	   
    power=1;
	
	
 while(1)	   
  { 
       	
  if(NewIRcode==1)    //如果有新的遥控码就读
      {		
	   
	   customcode=IRcode;    //取红外码中的用户码	 
	   //Lcustomcode=IRcode;   //取低8位用户码
	   //Hcustomcode=IRcode>>8;  //取高8位用户码
	   datacode=IRcode>>16;	   //取数据码
	   Rdatacode=IRcode>>24; 	//取数据码的反码 	  
	 
      if((customcode!=0xFF80)&&(~Rdatacode!=datacode)) {DataRight=0;IRcode=0;} //校验用户码，反码 
	    
		else DataRight=1;	    
	
	    NewIRcode=0; //读完之后清零，表示新码已读 
	   }
		
	  if(DataRight==1)  {show(datacode);}                     			
        
  }	

}








  