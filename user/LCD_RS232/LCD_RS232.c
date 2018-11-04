
//#include "stm32f10x.h"
#include "LCD_RS232.h"

unsigned char R_right;                                  //���ձ�����ȷ��־
volatile struct_USART2 Uart2Buff[5];                    //��stm32f10x_it.h�ﶨ��	  //����2��������CCU-TO-TCU
unsigned int X_Row;   
unsigned int Z_Column;  
unsigned int X_Run;   
unsigned int Z_Run;  
void PE7OUTPUT(void);
void PE7INPUT(void);
u8 Tray_Property = 0xff;
u8 Box_Property = 0xff;

Typedef_X_Zero_Pluse Change_X_Zero_Pluse;
Typedef_X_MPP Change_X_MPP;
Typedef_First_Goods_Height Change_First_Goods_Height;
Typedef_Z_MPP Change_Z_MPP;
Typedef_Plat_Axis_Z Change_Plat_Axis_Z;
Typedef_Plat_Axis_X Change_Plat_Axis_X;
Typedef_Y_RightZero_Pluse Change_Y_RightZero_Pluse;
Typedef_Y_LeftZero_Pluse Change_Y_LeftZero_Pluse;
Typedef_Y_LeftTrayPluse Change_Y_LeftTrayPluse;
Typedef_Y_LeftLim_MorePluse Change_Y_LeftLim_MorePluse;
Typedef_Y_LeftLim_LessPluse Change_Y_LeftLim_LessPluse;
Typedef_Y_RightLim_MorePluse Change_Y_RightLim_MorePluse;
Typedef_Y_RightLim_LessPluse Change_Y_RightLim_LessPluse;
Typedef_AxisZ_UpDownMore_Pluse Change_AxisZ_UpDownMore_Pluse;
Typedef_AxisZ_UpDown_Pluse Change_AxisZ_UpDown_Pluse;

//����2��ʼ��
void USART2_Init(u32 baud)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);					//USART2ʱ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	USART_DeInit(USART2);  //��λ����2
	
	/* Configure USART2 Tx (PA.2) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Configure USART2 Rx (PA.3) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate =baud;						    				//������115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//8Ϊ����λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;					//1λֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;							//����żУ��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  /********************USART2�жϳ�ʼ��***********************/
  //Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  

  USART_Init(USART2, &USART_InitStructure); 		//��ʼ������
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���
	USART_ClearFlag(USART2, USART_FLAG_TXE);			//�巢����ɱ�־
}

void USART2_printf_str(u8 len, u8*put_str)
{
	u8 i;
	for(i=0; i<len; i++)
	{
		USART_SendData(USART2,put_str[i]);
	}
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char recflag=0; //���ձ�־
unsigned int reclenth=0;//���ճ��ȼ�¼���ֶΣ�
unsigned char recbuff;	 //���ջ���
unsigned char check_temp=0;//У��λ

void USART2_IRQHandler(void)
{
  USART_ClearITPendingBit(USART2,USART_IT_RXNE); 

	recbuff=USART_ReceiveData(USART2);	//���������ݴ�
	
	switch(recflag)//��ʼֵΪ0
	{
		case 0: //֡ͷ1
		{
			 if(recbuff==0xa5)
			 {
			 	Uart2Buff[R_right].Star0=recbuff;
			 	recflag=1;
			 }
       else 
			 {
			 	recflag=0;//���ճ�ʼ�����½���
				reclenth=0;
				check_temp=0;
			 }
    }break;
		case 1: //֡ͷ2
		{
			 if(recbuff==0x5a)
			 {
			 	Uart2Buff[R_right].Star1=recbuff;
			 	recflag=2;
			 }
       else 
			 {
			 	recflag=0;//���ճ�ʼ�����½���
				reclenth=0;
				check_temp=0;
			 }
    }break;
		case 2: //���ݳ���
		{
			 Uart2Buff[R_right].Len=recbuff;
			 check_temp+=recbuff;
			 recflag=3;
		}break;
		case 3: //ָ��
		{
			 Uart2Buff[R_right].Comend=recbuff;
			 check_temp+=recbuff;
			 recflag=4;
		}break;
		case 4: //����
		{
			 Uart2Buff[R_right].ComData[reclenth]=recbuff;
			 reclenth++;
			 check_temp+=recbuff;
			 if(reclenth==(Uart2Buff[R_right].Len-1))
			 {
					 recflag=0;//���ճ�ʼ�����½���
					 check_temp=0;
					 reclenth=0;
					 R_right++;//���ճɹ���־��ʹ�ܡ�
					 if(R_right>=4)R_right=1;
			 }
		}break;
	} 
}

/***********************************************
* �������ƣ�void SURE_REC_UART2_Analyze(void)
* �����������ж��Ƿ�Ҫ�������ڽ��յ�������
* �����������
* ���ú�������
* ���ؽ������
***********************************************/
void SURE_REC_UART2_Analyze(void)
{
	unsigned char i;
	if(R_right)			
	{	
		if(Uart2Buff[4].Comend==0)
		{
			//ȡ�����������н���
			Uart2Buff[4].check      = Uart2Buff[R_right-1].check;
			Uart2Buff[4].Star0      = Uart2Buff[R_right-1].Star0;
			Uart2Buff[4].Star1      = Uart2Buff[R_right-1].Star1;
			Uart2Buff[4].Len		    = Uart2Buff[R_right-1].Len;
			Uart2Buff[4].Comend     = Uart2Buff[R_right-1].Comend;
			for(i=0;i<Uart2Buff[4].Len;i++)
			{
				Uart2Buff[4].ComData[i]=Uart2Buff[R_right-1].ComData[i];
			}
			R_right--;
			REC_UART2_Analyze(4);
			Uart2Buff[4].Comend=0;
		}
	}
}

/***********************************************
* �������ƣ�void REC_UART2_Analyze(void)
* �����������������ڽ��յ�������
* ������������ĺ�right_num
* ���ú�������
* ���ؽ������
***********************************************/
void REC_UART2_Analyze(unsigned char right_num)//����������SCREEN-TO-ACU��
{			
   char test;	
	switch(Uart2Buff[right_num].Comend)
	{
		case 0x81:
		{
		}break;
		case 0x83:	 //�����ڷ���ָ��
		{
			switch(Uart2Buff[right_num].ComData[1])//ָ��֡�����ֽ� ��������ַ��λ��ַ
			{
				case 0x00:
				{
				}break;
				case 0x01:
				{
					switch(Uart2Buff[right_num].ComData[4])
					{
						case 0x01:    //����1
						{
							Action.Step_Grade = 0;
							
				
////							PE7OUTPUT();
//							Start_I2C();
//							
////							PE7INPUT();
//							
//							test = bytRec_Byte(0);
//							test =test;
						}break;
						case 0x02:    //����2
						{
							Action.Step_Grade = 1;
						}break;
						case 0x03:    //����3
						{
							Action.Step_Grade = 2;
						}break;
						case 0x04:    //����4
						{
							Action.Step_Grade = 3;
							//StepMotorCtrl_Pulse(2500,AXIS_Y,Y_DIR_LEFT);//�궨��Y_DIR_LEFT
						}break;
						case 0x05:    //����5
						{
							Action.Step_Grade = 4;
							//StepMotorCtrl_Pulse(2500,AXIS_Y,Y_DIR_RIGHT);//�궨��
						}break;
						case 0x06:    //����6
						{
							Action.Step_Grade = 5;
							//StepMotorCtrl_Pulse(2150,AXIS_Y,Y_DIR_LEFT);//2200
						}break;
						case 0x07:    //����7
						{
							Action.Step_Grade = 6;
							//StepMotorCtrl_Pulse(2180,AXIS_Y,Y_DIR_RIGHT);//�궨��
						}break;
            case 0x08:    //�ٶȵ�λ1
						{
							Action.Speed_Grade = 0;
						}break;
						case 0x09:    //�ٶȵ�λ2
						{
							Action.Speed_Grade = 1;
						}break;
						case 0x0a:    //�ٶȵ�λ3
						{
							Action.Speed_Grade = 2;
						}break;
						case 0x0b:    //�ٶȵ�λ4
						{
							Action.Speed_Grade = 3;
						}break;
						case 0x0c:    //�ٶȵ�λ5
						{
							Action.Speed_Grade = 4;
						}break;
						case 0x0d:    //�ٶȵ�λ6
						{
							Action.Speed_Grade = 5;
						}break;
						case 0x0e:    //�ٶȵ�λ7
						{
							Action.Speed_Grade = 6;
						}break;
						case 0x0f:    // x��ǰ��
						{
							 Axis_Move_Alone(AXIS_X,X_DIR_FRONT);
						}break;
						case 0x10:    // x�����,��λ����
						{						
							 Axis_Move_Alone(AXIS_X,X_DIR_BACK);
						}break;
						case 0x11:    // y������
						{
							 Axis_Move_Alone(AXIS_Y,Y_DIR_LEFT);
						}break;
						case 0x12:    // y������
						{
							 Axis_Move_Alone(AXIS_Y,Y_DIR_RIGHT);
						}break;
						case 0x13:    // z������
						{
							 Axis_Move_Alone(AXIS_Z,Z_DIR_FRONT);
						}break;
						case 0x14:    // z���½�
						{
							 Axis_Move_Alone(AXIS_Z,Z_DIR_BACK);
						}break;  
						case 0x15:  // z���λ
						{
							Motor_BackToZero(AXIS_Z);
						}break; 
						case 0x16:   // x���λ
						{
							Motor_BackToZero(AXIS_X);
						}break; 
						case 0x17:   // �����λ
						{
							XYZ_To_Zero.Start_Flag = 1;
							XYZ_To_Zero.XYZ_Ret_Stage = 1;
							XYZ_To_Zero.XYZRET_Status_Change = 0;
						}break; 
						case 0x18:  //ֹͣ
						{
							Action_Stop();
						}break; 
						case 0x19:  //����
						{
							Get_PutBox_Analysis(Arrive_Analysis,Keep,Arrive,X_Run,Z_Run);
							Beep(50,50,1);
						}break;
						case 0x1a:  //ץ��
						{
							if(Box_Property == 0xff || Tray_Property == 0xff)
							{
								Beep(50,50,3);
								return;
							}
							else
							{
								Get_Box_Analysis(Tray_Property,Box_Property,X_Run,Z_Run);
								ChangePage(PAGE_POSITION);
							  Box_Property = 0xff;
							  Tray_Property =  0xff;
							}
						}break;
						case 0x1b:  //����
						{
							if(Box_Property == 0xff || Tray_Property == 0xff)
							{
								Beep(50,50,3);
								return;
							}
							else
							{
								Put_Box_Analysis(Tray_Property,Box_Property,X_Run,Z_Run);
								ChangePage(PAGE_POSITION);
							  Box_Property = 0xff;
							  Tray_Property =  0xff;
							}							
						}break;	
						case 0x1c:   //ץ�����������
						{
							Box_Property = Get_Left_Box; 
						}break;
						case 0x1d:   //ץ�����������
						{
							Box_Property = Get_Right_Box; 
						}break;			
            case 0x1e:   //�������������
						{
							Box_Property = Return_Left_Box; 
						}break;						
						case 0x1f:   //�������������
						{
							Box_Property = Return_Right_Box; 
						}break;						
						case 0x20:   //����������
						{
							Tray_Property = Left_Tray; 
						}break;			
						case 0x21:   //����������
						{
							Tray_Property = Right_Tray; 
						}break;			
						case 0x22:   //�޸�X�ᵽ����λ��ǰ���������
						{

						}break;					
						case 0x23:
						{

						}break;		
						case 0x24:
						{
							
						}break;							
					}
				}break;
        case 0x02:
				{
					X_Row = Uart2Buff[right_num].ComData[4];
					if(X_Row>=12)
					{
						return;
					}
					X_Run= 412 * X_Row;
				}break;
				case 0x03:
				{
					Z_Column = Uart2Buff[right_num].ComData[4];
					if((Z_Column>=10)||(Z_Column==0))
					{
						return;
					}
					if(Z_Column <= 4)
					{
						Z_Run = 270*Z_Column - Z_Difference;
					}
					else
					{
						Z_Run = 270*Z_Column - (Z_Difference-70);//170
					}
				}break;		
				case 0x04:
				{
						Change_X_Zero_Pluse.X_Zero_Pluse_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_X_Zero_Pluse.X_Zero_Pluse_CharData,4,1);
						X_Zero_Pluse = Change_X_Zero_Pluse.X_Zero_Pluse_Data;
				}break;	
				case 0x05:
				{
					  Change_X_MPP.X_MPP_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_X_MPP.X_MPP_CharData,8,10);
						X_MPP = Change_X_MPP.X_MPP_Data;
				}break;
				case 0x06:
				{
						Change_First_Goods_Height.First_Goods_Height_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_First_Goods_Height.First_Goods_Height_CharData,4,25);
						First_Goods_Height = Change_First_Goods_Height.First_Goods_Height_Data;
				}break;
				case 0x07:
				{
					  Change_Z_MPP.Z_MPP_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_Z_MPP.Z_MPP_CharData,8,35);
						Z_MPP = Change_Z_MPP.Z_MPP_Data;
				}break;
				case 0x08:
				{
						Change_Plat_Axis_Z.Plat_Axis_Z_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_Plat_Axis_Z.Plat_Axis_Z_CharData,4,50);
						Plat_Axis_Z = Change_Plat_Axis_Z.Plat_Axis_Z_Data;
				}break;
				case 0x09:
				{
						Change_Plat_Axis_X.Plat_Axis_X_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_Plat_Axis_X.Plat_Axis_X_CharData,4,60);
						Plat_Axis_X = Change_Plat_Axis_X.Plat_Axis_X_Data;
				}break;
				case 0x0A:
				{
						Change_Y_RightZero_Pluse.Y_RightZero_Pluse_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_Y_RightZero_Pluse.Y_RightZero_Pluse_CharData,4,70);
						Y_RightZero_Pluse = Change_Y_RightZero_Pluse.Y_RightZero_Pluse_Data;
				}break;
				case 0x0B:
				{
						Change_Y_LeftZero_Pluse.Y_LeftZero_Pluse_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_Y_LeftZero_Pluse.Y_LeftZero_Pluse_CharData,4,80);
						Y_LeftZero_Pluse = Change_Y_LeftZero_Pluse.Y_LeftZero_Pluse_Data;
				}break;
				case 0x0C:
				{
						Change_Y_LeftTrayPluse.Y_LeftTrayPluse_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_Y_LeftTrayPluse.Y_LeftTrayPluse_CharData,4,90);
						Y_LeftTrayPluse = Change_Y_LeftTrayPluse.Y_LeftTrayPluse_Data;
				}break;
				case 0x0D:
				{
						Change_Y_LeftLim_MorePluse.Y_LeftLim_MorePluse_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_Y_LeftLim_MorePluse.Y_LeftLim_MorePluse_CharData,4,100);
						Y_LeftLim_MorePluse = Change_Y_LeftLim_MorePluse.Y_LeftLim_MorePluse_Data;
				}break;
				case 0x0E:
				{
						Change_Y_LeftLim_LessPluse.Y_LeftLim_LessPluse_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_Y_LeftLim_LessPluse.Y_LeftLim_LessPluse_CharData,4,110);
						Y_LeftLim_LessPluse = Change_Y_LeftLim_LessPluse.Y_LeftLim_LessPluse_Data;
				}break;
				case 0x0F:
				{
						Change_Y_RightLim_MorePluse.Y_RightLim_MorePluse_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_Y_RightLim_MorePluse.Y_RightLim_MorePluse_CharData,4,120);
						Y_RightLim_MorePluse = Change_Y_RightLim_MorePluse.Y_RightLim_MorePluse_Data;
				}break;
				case 0x10:
				{
						Change_Y_RightLim_LessPluse.Y_RightLim_LessPluse_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_Y_RightLim_LessPluse.Y_RightLim_LessPluse_CharData,4,130);
						Y_RightLim_LessPluse = Change_Y_RightLim_LessPluse.Y_RightLim_LessPluse_Data;
				}break;				
				case 0x11:
				{
						Change_AxisZ_UpDown_Pluse.AxisZ_UpDown_Pluse_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_AxisZ_UpDown_Pluse.AxisZ_UpDown_Pluse_CharData,4,140);
						AxisZ_UpDown_Pluse = Change_AxisZ_UpDown_Pluse.AxisZ_UpDown_Pluse_Data;
				}break;				
				case 0x12:
				{
						Change_AxisZ_UpDownMore_Pluse.AxisZ_UpDownMore_Pluse_Data = Uart2Buff[right_num].ComData[4];
						I2C_BufferWrite(Change_AxisZ_UpDownMore_Pluse.AxisZ_UpDownMore_Pluse_CharData,4,150);
						AxisZ_UpDownMore_Pluse = Change_AxisZ_UpDownMore_Pluse.AxisZ_UpDownMore_Pluse_Data;
				}break;				
			}
		}break;
	}
}

//Ӧ���ӦSCREEN ��Ҫ����
void Uart2AskBytes(unsigned char *str,unsigned int Num) 
{
	unsigned int i;	
	for(i=0;i<Num;i++)
	{  
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  
		//�ȴ�������ɡ�   ��� USART_FLAG_TC �Ƿ���1��    //���⺯�� P359 ����
		USART_SendData(USART2,str[i]);	
		if((i+1)%4096==0)
		{
			DelayTimes_ms=10;
			while (DelayTimes_ms);
		}
	}		
}
/************************************************
* �������ƣ�void Set_Variable(unsigned char adress, unsigned char data)
* ��������������Һ����������ַ�е���ֵ
* �����������ַadress������data
* ���ؽ������
*************************************************/
void Set_Variable(unsigned char adressH,unsigned char adressL, 
									unsigned char data0, unsigned char data1, 
									unsigned char data2, unsigned char data3, 
									unsigned char length)
{
	unsigned char sendtemp[10];
	sendtemp[0]=0xA5;
	sendtemp[1]=0x5A;
	sendtemp[2]=0x03+length;
	sendtemp[3]=0x82;
	sendtemp[4]=adressH;
	sendtemp[5]=adressL;
	sendtemp[6]=data0;
	sendtemp[7]=data1;
	sendtemp[8]=data2;
	sendtemp[9]=data3;
	Uart2AskBytes(sendtemp,length+6);			
}

void ChangePage(unsigned char page)
{
  u8 sendbuff[7];
  sendbuff[0] = 0xA5;
  sendbuff[1] = 0x5A;
  sendbuff[2] = 0x04;
  sendbuff[3] = 0x80;
  sendbuff[4] = 0x03;
  sendbuff[5] = 0x00;
  sendbuff[6] = page;
  Uart2AskBytes(sendbuff,7);
}

void PE7OUTPUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);							
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_9;//PE0��P27_DIR2,PE2��P27_PUL2,PE4��P28_DIR1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				
	GPIO_Init(GPIOE, &GPIO_InitStructure);									
	GPIO_ResetBits(GPIOE, GPIO_Pin_9);
}

void PE7INPUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);							
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9; //IO		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void All_I2C_Read(void)
{
			I2C_ReadByte(Change_X_Zero_Pluse.X_Zero_Pluse_CharData,4,1);
	    I2C_ReadByte(Change_X_MPP.X_MPP_CharData,8,10);
	    I2C_ReadByte(Change_First_Goods_Height.First_Goods_Height_CharData,4,25);
		  I2C_ReadByte(Change_Z_MPP.Z_MPP_CharData,8,35);
			I2C_ReadByte(Change_Plat_Axis_Z.Plat_Axis_Z_CharData,4,50);
	    I2C_ReadByte(Change_Plat_Axis_X.Plat_Axis_X_CharData,4,60);
	    I2C_ReadByte(Change_Y_RightZero_Pluse.Y_RightZero_Pluse_CharData,4,70);
	    I2C_ReadByte(Change_Y_LeftZero_Pluse.Y_LeftZero_Pluse_CharData,4,80);
	    I2C_ReadByte(Change_Y_LeftTrayPluse.Y_LeftTrayPluse_CharData,4,90);
	    I2C_ReadByte(Change_Y_LeftLim_MorePluse.Y_LeftLim_MorePluse_CharData,4,100);
	    I2C_ReadByte(Change_Y_LeftLim_LessPluse.Y_LeftLim_LessPluse_CharData,4,110);
	    I2C_ReadByte(Change_Y_RightLim_MorePluse.Y_RightLim_MorePluse_CharData,4,120);
	    I2C_ReadByte(Change_Y_RightLim_LessPluse.Y_RightLim_LessPluse_CharData,4,130);
	    I2C_ReadByte(Change_AxisZ_UpDown_Pluse.AxisZ_UpDown_Pluse_CharData,4,140);
	    I2C_ReadByte(Change_AxisZ_UpDownMore_Pluse.AxisZ_UpDownMore_Pluse_CharData,4,150);
}
/**********������λ����Ϣ����ʱ����**************/
void Uart2_Send_test(u8 Length,u8 cmd,u8 net_num,u8 Port_cfg,u8 comdata,u8 Buf[])
{
	u8 send_buf[10+8*Length];
	memset(&send_buf,0,sizeof(send_buf));
	send_buf[0]=0xaa;
	send_buf[1]=0x55;
	send_buf[2]=Length;
	send_buf[3]=cmd;
	send_buf[4]=net_num;
	send_buf[5]=Port_cfg;
	send_buf[6]=comdata;
	if(Length == 1)
	{
		send_buf[8]=Buf[0];
		send_buf[9]=Buf[1];
		send_buf[10]=Buf[2];
		send_buf[11]=Buf[3];
		send_buf[12]=Buf[4];
		send_buf[13]=Buf[5];
		send_buf[14]=Buf[6];
		send_buf[15]=Buf[7];
		send_buf[8+8*Length]=0xa0;
		send_buf[9+8*Length]=0x5f;
	}
	else
	{
		send_buf[8+8*Length]=0xa0;
		send_buf[9+8*Length]=0x5f;
	}
	for(int j=2;j<(10+8*Length);j++)//�����У��
		{
				if( (j==7) || (j==(Length*8+8)) || (j==(Length*8+9)) )//i��2��ʼ�ӣ���ʾ֡ͷ������У�飬i==7ΪУ��λ i==size-1Ϊ֡β
				{
					continue;
				}
				send_buf[7]+=send_buf[j];
		}
	for(int i=0;i<sizeof(send_buf);i++)
	{
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  
		//�ȴ�������ɡ�   ��� USART_FLAG_TC �Ƿ���1��    //���⺯�� P359 ����
		USART_SendData(USART2,send_buf[i]);
	}
}
