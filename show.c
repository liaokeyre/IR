//MM74HC595 +�������ʾ�ĳ���ģ�飬��Ű�MM74HC595.PDF�ļ���ע
//�ṩshow(showdata)��ʾ�ӿ�  showdata:0~99



#include<reg51.h>  
#define uchar unsigned char

uchar code seg[10]={0xC0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

sbit SER=P0^4;	  //595������������
sbit SCK=P0^5;	  //595��λʱ��
sbit RCK=P0^7;	  //595�����������
sbit SQH=P0^6;	  //595�����������
sbit gewei=P2^7;  //595����ܸ�λѡͨ
sbit shiwei=P2^6; //595�����ʮλѡͨ 


sbit power=P1^7;  

void IN595(uchar segdata)	//����һ�ֽ���������ݽ�595
{
  uchar i,temp;
  //temp=seg[segdata];
  temp=segdata;		    //����
  RCK=0;  
  for(i=0;i<8;i++)	//ѭ������8λ���ݣ�
  {
     SCK=0;
  	 SER=temp&0x80;	// ȡ��������λ
	 temp=temp<<1;
	 SCK=1;	//�����ذ�һλ��������595
  }
  //���ڵ�·����ƵĲ�������ʱ�����λQA�Ķ�Ӧ������������½ǵĵ�Ĺܽţ�������Ҫ����һλ��
  //ʹ������������Ӧ����������QA����ֵ��1��ʹ����ܲ���ʾ��
  SCK=0;
  SER=segdata&0x80;			//
  SCK=1; 


  RCK=1;  //�����ذ�8λ���ݲ������ 
}


void show(uchar showdata)	 //ע�������Ӧ�ڲ���ѭ������ʾ
{
   IN595(showdata);    //����ʮλ����
   gewei=1;     //�ظ�λ
   shiwei=0;    //ѡͨʮλ�����
 

}









