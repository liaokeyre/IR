/*--------------------------------------------------------------------------------
  ң�����������
  �㷨���������½��صĵ�ʱ�䣬���ж� 0��  1�������������룬������0

---------------------------------------------------------------------------------*/

#include<reg51.h>
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long	

extern show(uchar showdata); 

volatile ulong IRcode=0x00000000;  //32λ�ļ�����

volatile uint customcode=0x0000;      //16λ�û���
volatile uint time_us=0x0000;      //�����½���֮���ʱ��	

volatile uchar timeH,timeL;	 //����TH1 TL��ֵ

uchar Lcustomcode=0x00;      //��8�û���
uchar Hcustomcode=0x00;     //��8
uchar datacode=0x00;	   //8λ��������
uchar Rdatacode=0x00;      //8λ�����ݷ���
uchar T1overflow=0x00;   //T1������� 	������������

sbit IR=P3^2;	//����  
sbit power=P1^7;  //��Դ����
bit NewIRcode=0;  //ָʾ����������32λ��󣬾������µ�ң����
bit DataRight=0;   //Ϊ1ʱ��ȡ������ȷ 


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

  
  TMOD=0x11; //��ʱ��0��������ʽ1 ;  ��ʱ��1��������ʽ1
  TCON=0x01; //���ж�0�½��ش�����������TR1=0��TR0=0��
  TH0=0x3c;	 //��ʼ����ʱ��0,��ʱ50ms
  TL0=0xb0;
  TH1=0x00;	 //��ʼ����ʱ��1 
  TL1=0x00;	   

  EA=1;		  //��ȫ�ж�
  ET0=1;	  //����T0�ж�
  ET1=1;      //����T1�ж�
  EX0=1;	  //����INT0
  IP=0x01;   //?????  
}

//-------------------------------------interrupt0-------------------------------------
void IR_ISR() interrupt 0 using 1	//ң�����жϴ�����
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

  if(time_us>13300&&time_us<13700) {cn=1;IRcode=0;}  //����������,�Ͱ�cn��0��IRcode��0	
   
  if(cn<34)
  {
  if(time_us>1000&&time_us<1300) //0  
          {IRcode=IRcode|0x00000000; if(cn<33) IRcode=IRcode>>1;}

  else if(time_us>2000&&time_us<2450) //1	 
          {IRcode=IRcode|0x80000000;if(cn<33) IRcode=IRcode>>1;}
  }
		  
  else if(time_us>11000&&time_us<11500) {IRcode=IRcode;}; //�����ظ���
     																							
   cn++; 

  if(cn==34) {NewIRcode=1;TR1=0;}  //����32λ�룬����������� 

}

//--------------------------------------timer_ISR------------------------------
void Timer0_ISR() interrupt 1 using 2		//��ʱ��0�жϺ���
{
	//TR1=0;	
	//TR1=1; 
}
void Timer1_ISR() interrupt 3 using 3		//��ʱ��1�жϺ���
{
   T1overflow++;	 //��Ҫ�ǵ���0��������
   //������++���ű����������л�ʱ��ȵĵ�����������������������������������������������
   //TH1=0xff;   //��������Բ�Ҫ,��ʱ�����Բ��ϵ���������
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
       	
  if(NewIRcode==1)    //������µ�ң����Ͷ�
      {		
	   
	   customcode=IRcode;    //ȡ�������е��û���	 
	   //Lcustomcode=IRcode;   //ȡ��8λ�û���
	   //Hcustomcode=IRcode>>8;  //ȡ��8λ�û���
	   datacode=IRcode>>16;	   //ȡ������
	   Rdatacode=IRcode>>24; 	//ȡ������ķ��� 	  
	 
      if((customcode!=0xFF80)&&(~Rdatacode!=datacode)) {DataRight=0;IRcode=0;} //У���û��룬���� 
	    
		else DataRight=1;	    
	
	    NewIRcode=0; //����֮�����㣬��ʾ�����Ѷ� 
	   }
		
	  if(DataRight==1)  {show(datacode);}                     			
        
  }	

}








  