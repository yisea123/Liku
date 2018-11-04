
#include "Key.h"

bool Key_press_flag = 0;
bool KEY1_flag = 0;
bool KEY2_flag = 0;
bool KEY3_flag = 0;
bool KEY4_flag = 0;
bool KEY5_flag = 0;
bool KEY6_flag = 0;
bool KEY7_flag = 0;
bool KEY8_flag = 0;

bool Bottle_Judge_Record; //��Сƿ��¼ģʽ����(KEY3 + ��λ��)����	
u8 TEST_MODE;           //����ģʽ��ǣ���(KEY4 + ��λ��)����	
u8 TEST_MODE_SAVE = 1;
u8 Bottle_Judge_Mode = 0;	//��Сƿ�ж������¼ģʽ����¼�߰�ƿģʽ�л�
//		char buf0[] = {'+','+','+','\0'};
//		char buf1[] = {'A','T','+','C','W','M','O','D','E','_','D','E','F','=','1','\r','\n','\0'};
//		char buf2[] = {'A','T','+','C','W','J','A','P','_','D','E','F','=','"','J','S','-','L','I','K','U','"',',','"','j','e','s','o','o','1','2','3','"','\r','\n','\0'};
//		char buf3[] = {'A','T','+','C','I','P','S','T','A','R','T','=','"','T','C','P','"',',','"','1','9','2','.','1','6','8','.','0','.','5','3','"',',','6','0','0','0','\r','\n','\0'};
//		char buf4[] = {'A','T','+','C','I','P','M','O','D','E','=','1','\r','\n','\0'};
//		char buf5[] = {'A','T','+','C','I','P','S','T','A','_','D','E','F','=',
//									'"','1','9','2','.','1','6','8','.','0','.','1','1','7','"',',',
//									'"','1','9','2','.','1','6','8','.','0','.','1','1','6','"',',',
//									'"','2','5','5','.','2','5','5','.','2','5','5','.','0','"','\r','\n','\0'};
//		char buf6[] = {'A','T','+','C','I','P','S','E','N','D','\r','\n','\0'};

/*-----------*/
u16 Time_PushRod_Ctrl;   

void KEY_Init(void) //����IO��ʼ��
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;//PD0,PD2,PD4,PD6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOD	
}

/************�������***********/
void Key_Scan()
{
	if(KEY1 == 0)
	{
		if(Key_press_flag == 0)
		{
			delay_ms(3);
			if(KEY1 == 0)
			{
				Key_press_flag = 1;
				KEY1_flag = 1;
			}
		}
	}
	else if(KEY2 == 0)
	{
		if(Key_press_flag == 0)
		{
			delay_ms(3);
			if(KEY2 == 0)
			{
				Key_press_flag = 1;
				KEY2_flag = 1;
			}
		}
	}
	else if(KEY3 == 0)
	{
		if(Key_press_flag == 0)
		{
			delay_ms(3);
			if(KEY3 == 0)
			{
				Key_press_flag = 1;
				KEY3_flag = 1;
			}
		}
	}
	else if(KEY4 == 0)
	{
		if(Key_press_flag == 0)
		{
			delay_ms(3);
			if(KEY4 == 0)
			{
				Key_press_flag = 1;
				KEY4_flag = 1;
			}
		}
	}
	else if(KEY5 == 0)
	{
		if(Key_press_flag == 0)
		{
			delay_ms(3);
			if(KEY5 == 0)
			{
				Key_press_flag = 1;
				KEY5_flag = 1;
			}
		}
	}
	else if(KEY6 == 0)
	{
		if(Key_press_flag == 0)
		{
			delay_ms(3);
			if(KEY6 == 0)
			{
				Key_press_flag = 1;
				KEY6_flag = 1;
			}
		}		
	}
	else
	{
		Key_press_flag = 0;
	}
}

/************��������***********/
void Key_Deal()
{
	u8 test;
	u16 result;
	Key_Scan();
	/***********����1����********************/
	if(KEY1_flag == 1)
	{
		KEY1_flag = 0;
//		result = (u16)test<<8;//Ŀ��X������
//		test = test;
		Beep(200,200,1);
//		PE7OUTPUT();
//		Start_I2C();
//		
//		
//		
//		test = bytRec_Byte(1);
//		test =test;
		Axis_Move_Alone(AXIS_X,X_DIR_FRONT);
	}
	/***********����2����********************/
	if(KEY2_flag == 1)
	{
		KEY2_flag = 0;
		Axis_Move_Alone(AXIS_X,X_DIR_BACK);
		Beep(200,200,1);

	}
	/***********����3����********************/
	if(KEY3_flag == 1)
	{
//		u8 CheckAdd =0;
		KEY3_flag = 0;		
		Axis_Move_Alone(AXIS_Z,Z_DIR_FRONT);
		Beep(200,100,1);      //��������һ��

//		send_char(buf3,0);//AT+CIPSTART="TCP","192.168.0.53",6000
//		delay_ms(1000);
//		send_char(buf4,0);//AT+CIPMODE=1
//		delay_ms(1000);
//		send_char(buf5,0);//AT+CIPSTA_DEF="192.168.0.117","192.168.0.116","255.255.255.0"
//		delay_ms(1000);
//		send_char(buf6,0);//AT+CIPSEND
	}
	/***********����4����********************/
	if(KEY4_flag == 1)
	{
		KEY4_flag = 0;
		Axis_Move_Alone(AXIS_Z,Z_DIR_BACK);
		Beep(200,100,1);      //��������һ��
//		DIR_Z = Z_DIR_FRONT;
//								Axis_Z.Dir = Z_DIR_FRONT;
//								Axis_Z.NowPulse = 0;
//								Axis_Z.Target_Pulse = 200;
//								Axis_Z.InCtrl_Flag = 1;
//		char buf3[] = {'A','T','+','C','I','P','S','T','A','R','T','=','"','T','C','P','"',',','"','1','9','2','.','1','6','8','.','0','.','5','3','"',',','6','0','0','0','\r','\n','\0'};
//		char buf4[] = {'A','T','+','C','I','P','M','O','D','E','=','1','\r','\n','\0'};
//		char buf5[] = {'A','T','+','C','I','P','S','T','A','_','D','E','F','=',
//									'"','1','9','2','.','1','6','8','.','0','.','1','1','7','"',',',
//									'"','1','9','2','.','1','6','8','.','0','.','1','1','6','"',',',
//									'"','2','5','5','.','2','5','5','.','2','5','5','.','0','"','\r','\n','\0'};
//		char buf6[] = {'A','T','+','C','I','P','S','E','N','D','\r','\n','\0'};
//		send_char(buf3,0);//AT+CIPSTART="TCP","192.168.0.53",6000
//		delay_ms(1500);
//		send_char(buf4,0);//AT+CIPMODE=1
//		delay_ms(500);
//		send_char(buf5,0);//AT+CIPSTA_DEF="192.168.0.117","192.168.0.116","255.255.255.0"
//		delay_ms(500);
//		send_char(buf6,0);//AT+CIPSEND
//send_char(buf3,0);//AT+CIPSTART="TCP","192.168.0.53",6000
	}
	/***********����5����********************/
	if(KEY5_flag == 1)
	{
		KEY5_flag = 0;
//		DIR_Z = Z_DIR_BACK;
//		Axis_Z.Dir = Z_DIR_BACK;
//		Axis_Z.NowPulse = 0;
//		Axis_Z.Target_Pulse = 10;
//		Axis_Z.InCtrl_Flag = 1;
//		PWM_Set_Z(60000,PWM_PCS);
//		TIM_Cmd(TIM4, ENABLE);
//		send_char(buf4,0);//AT+CIPMODE=1
		Beep(50,50,1);      //��������һ��
//		Change_X_MPP.X_MPP_Data = 0.4400;
//		I2C_BufferWrite(Change_X_MPP.X_MPP_CharData,8,10);
//		I2C_ReadByte(Change_X_MPP.X_MPP_CharData,8,10);
//		Change_X_MPP.X_MPP_Data = Change_X_MPP.X_MPP_Data;
//		X_MPP = Change_X_MPP.X_MPP_Data;
	}
	/***********����6����********************/
	if(KEY6_flag == 1)
	{
		KEY6_flag = 0;
		Beep(50,50,1);      //��������һ��		
//		DIR_Z = Z_DIR_FRONT;
//	  Axis_Z.Dir = Z_DIR_FRONT;
//		Axis_Z.NowPulse = 0;
//		Axis_Z.Target_Pulse = 10;
//		Axis_Z.InCtrl_Flag = 1;
//		PWM_Set_Z(60000,PWM_PCS);
//		TIM_Cmd(TIM4, ENABLE);
//		send_char(buf5,0);//AT+CIPSTA_DEF="192.168.0.117","192.168.0.116","255.255.255.0"
//		delay_ms(500);
//		send_char(buf6,0);//AT+CIPSEND
//		Axis_Move_Alone(AXIS_Y,Y_DIR_RIGHT);
//		PCout(0) = 1;
//		PCout(1) = 1;
//		PCout(3) = 1;
//		if(Length_High_LimCtl() == 1)
//		{
//			Beep(50,50,5);
//		}			
//		Change_X_Zero_Pluse[0] = 270;
//		I2C_BufferWrite(Change_X_Zero_Pluse,4,1);
//		I2C_ReadByte(Change_X_Zero_Pluse,4,1);
//		X_Zero_Pluse = Change_X_Zero_Pluse[0];
						Change_X_Zero_Pluse.X_Zero_Pluse_Data = 320;
						I2C_PageWrite(Change_X_Zero_Pluse.X_Zero_Pluse_CharData,4,1);
						X_Zero_Pluse = Change_X_Zero_Pluse.X_Zero_Pluse_Data;
						
						Change_X_MPP.X_MPP_Data = 0.4400;
		        I2C_BufferWrite(Change_X_MPP.X_MPP_CharData,8,10);
		        X_MPP = Change_X_MPP.X_MPP_Data;
						
						Change_First_Goods_Height.First_Goods_Height_Data = 30;
						I2C_BufferWrite(Change_First_Goods_Height.First_Goods_Height_CharData,4,25);
						First_Goods_Height = Change_First_Goods_Height.First_Goods_Height_Data;
						
						Change_Z_MPP.Z_MPP_Data = 0.3395;
						I2C_BufferWrite(Change_Z_MPP.Z_MPP_CharData,8,35);
						Z_MPP = Change_Z_MPP.Z_MPP_Data;
						
						Change_Plat_Axis_Z.Plat_Axis_Z_Data = 212;
						I2C_BufferWrite(Change_Plat_Axis_Z.Plat_Axis_Z_CharData,4,50);
						Plat_Axis_Z = Change_Plat_Axis_Z.Plat_Axis_Z_Data;
						
						Change_Plat_Axis_X.Plat_Axis_X_Data = 10;
						I2C_BufferWrite(Change_Plat_Axis_X.Plat_Axis_X_CharData,4,60);
						Plat_Axis_X = Change_Plat_Axis_X.Plat_Axis_X_Data;
						
						Change_Y_RightZero_Pluse.Y_RightZero_Pluse_Data = 60;
						I2C_BufferWrite(Change_Y_RightZero_Pluse.Y_RightZero_Pluse_CharData,4,70);
						Y_RightZero_Pluse = Change_Y_RightZero_Pluse.Y_RightZero_Pluse_Data;

						Change_Y_LeftZero_Pluse.Y_LeftZero_Pluse_Data = 80;
						I2C_BufferWrite(Change_Y_LeftZero_Pluse.Y_LeftZero_Pluse_CharData,4,80);
						Y_LeftZero_Pluse = Change_Y_LeftZero_Pluse.Y_LeftZero_Pluse_Data;

						Change_Y_LeftTrayPluse.Y_LeftTrayPluse_Data = 2250;
						I2C_BufferWrite(Change_Y_LeftTrayPluse.Y_LeftTrayPluse_CharData,4,90);
						Y_LeftTrayPluse = Change_Y_LeftTrayPluse.Y_LeftTrayPluse_Data;

						Change_Y_LeftLim_MorePluse.Y_LeftLim_MorePluse_Data = 2200;
						I2C_BufferWrite(Change_Y_LeftLim_MorePluse.Y_LeftLim_MorePluse_CharData,4,100);
						Y_LeftLim_MorePluse = Change_Y_LeftLim_MorePluse.Y_LeftLim_MorePluse_Data;

						Change_Y_LeftLim_LessPluse.Y_LeftLim_LessPluse_Data = 2165;
						I2C_BufferWrite(Change_Y_LeftLim_LessPluse.Y_LeftLim_LessPluse_CharData,4,110);
						Y_LeftLim_LessPluse = Change_Y_LeftLim_LessPluse.Y_LeftLim_LessPluse_Data;

						Change_Y_RightLim_MorePluse.Y_RightLim_MorePluse_Data = 2250;
						I2C_BufferWrite(Change_Y_RightLim_MorePluse.Y_RightLim_MorePluse_CharData,4,120);
						Y_RightLim_MorePluse = Change_Y_RightLim_MorePluse.Y_RightLim_MorePluse_Data;

						Change_Y_RightLim_LessPluse.Y_RightLim_LessPluse_Data = 2190;
						I2C_BufferWrite(Change_Y_RightLim_LessPluse.Y_RightLim_LessPluse_CharData,4,130);
						Y_RightLim_LessPluse = Change_Y_RightLim_LessPluse.Y_RightLim_LessPluse_Data;

						Change_AxisZ_UpDown_Pluse.AxisZ_UpDown_Pluse_Data = 110;
						I2C_BufferWrite(Change_AxisZ_UpDown_Pluse.AxisZ_UpDown_Pluse_CharData,4,140);
						AxisZ_UpDown_Pluse = Change_AxisZ_UpDown_Pluse.AxisZ_UpDown_Pluse_Data;

						Change_AxisZ_UpDownMore_Pluse.AxisZ_UpDownMore_Pluse_Data = 190;
						I2C_BufferWrite(Change_AxisZ_UpDownMore_Pluse.AxisZ_UpDownMore_Pluse_CharData,4,150);
						AxisZ_UpDownMore_Pluse = Change_AxisZ_UpDownMore_Pluse.AxisZ_UpDownMore_Pluse_Data;
						
	}
}



