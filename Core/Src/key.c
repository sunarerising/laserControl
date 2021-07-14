#include "key.h"




//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3����
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!

uint8_t KEY_Scan()
{
	static uint8_t key_up=1;
 
	if(key_up==0&&(KEY0==0&&KEY1==0&&KEY2==0&&KEY3==0))key_up=1;
	while(key_up)
	{ 
		 sm0(1);sm1(0);sm2(0);sm3(0);   //ɨ��
	    if(key_up&&(KEY0==1||KEY1==1||KEY2==1||KEY3==1))     //key_up Ϊ 1 
		{
				HAL_Delay(10);	//ȥ����
			    key_up=0;
				 if(KEY0==1)return KEY0_PRES;
				else if(KEY1==1)return KEY1_PRES;
				else if(KEY2==1)return KEY2_PRES;
				else if(KEY3==1)return KEY3_PRES;}
		 
		sm0(0);sm1(1);sm2(0);sm3(0);
		if(key_up&&(KEY0==1||KEY1==1||KEY2==1||KEY3==1))
	   {
				HAL_Delay(10);//ȥ����
				key_up=0;
				 if(KEY0==1)return KEY4_PRES;
				else if(KEY1==1)return KEY5_PRES;
				else if(KEY2==1)return KEY6_PRES;
				else if(KEY3==1)return KEY7_PRES;}
		 	
		sm0(0);sm1(0);sm2(1);sm3(0);
		if(key_up&&(KEY0==1||KEY1==1||KEY2==1||KEY3==1))
	   {
			HAL_Delay(10);//ȥ����
			key_up=0;
		    if(KEY0==1)return KEY8_PRES;
	     	else if(KEY1==1)return KEY9_PRES;
		    else if(KEY2==1)return KEY10_PRES;
		    else if(KEY3==1)return KEY11_PRES;}
     
		sm0(0);sm1(0);sm2(0);sm3(1);
		if(key_up&&(KEY0==1||KEY1==1||KEY2==1||KEY3==1))
		{
			HAL_Delay(10);//ȥ����
			key_up=0;
				if(KEY0==1)return KEY12_PRES;
				else if(KEY1==1)return KEY13_PRES;
				else if(KEY2==1)return KEY14_PRES;
				else if(KEY3==1)return KEY15_PRES;}
 
		return 0;
	}
 return 0;
}








