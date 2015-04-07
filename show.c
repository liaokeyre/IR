//MM74HC595 +数码管显示的程序模块，标号按MM74HC595.PDF文件标注
//提供show(showdata)显示接口  showdata:0~99



#include<reg51.h>  
#define uchar unsigned char

uchar code seg[10]={0xC0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

sbit SER=P0^4;	  //595串行数据输入
sbit SCK=P0^5;	  //595移位时钟
sbit RCK=P0^7;	  //595数据输出脉冲
sbit SQH=P0^6;	  //595串行数据输出
sbit gewei=P2^7;  //595数码管个位选通
sbit shiwei=P2^6; //595数码管十位选通 


sbit power=P1^7;  

void IN595(uchar segdata)	//输入一字节数码管数据进595
{
  uchar i,temp;
  //temp=seg[segdata];
  temp=segdata;		    //！！
  RCK=0;  
  for(i=0;i<8;i++)	//循环移入8位数据，
  {
     SCK=0;
  	 SER=temp&0x80;	// 取数码的最高位
	 temp=temp<<1;
	 SCK=1;	//上升沿把一位数据移入595
  }
  //由于电路上设计的不合理，这时在最低位QA的对应的是数码管右下角的点的管脚，所以需要再移一位，
  //使段码脚与数码对应起来，并把QA处的值置1，使数码管不显示点
  SCK=0;
  SER=segdata&0x80;			//
  SCK=1; 


  RCK=1;  //上升沿把8位数据并行输出 
}


void show(uchar showdata)	 //注意数码管应在不断循环中显示
{
   IN595(showdata);    //移入十位数据
   gewei=1;     //关个位
   shiwei=0;    //选通十位数码管
 

}









