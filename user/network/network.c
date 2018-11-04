#include "network.h"

volatile struct_UART3 Uart3[20];
bool Reset_Flag ;//������λ��־
bool Set_flag;					//�������ñ�־ ������1����ģ������ΪSTAģʽ 
u8 Current_Position_Buff[8] ;//���ص�ǰ������Ϣ
u8 Cache_Cnt = 0;

void Return_SendMesge(void)//��λ����ϴ���Ϣ
{
	if(XYZ_To_Zero.Over_Flag == 1)
	{
		XYZ_To_Zero.Over_Flag = 0;
		Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,Fifth_Stage,0);//XZ�˶��������������ر���
	}
}
void USART3_Init(u32 bound)		//USART3 ����WIFI
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//USART3ʱ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
	
	USART_DeInit(USART3);  //��λ����3
	//USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��ʼ��PB10
	//USART3_RX   PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);								//��ʼ��PB11
		
	//USART3��ʼ��
	USART_InitStructure.USART_BaudRate = bound;	//һ��Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������
	RFID_ON();
	
	/* Enable the USART1 */

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART3, USART_FLAG_TXE);     /* �巢����ɱ�־��Transmission Complete flag */
	USART_Cmd(USART3, ENABLE);	
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�


  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	Uart3[Cache_Cnt].RecAdd = 0;

}

void send_char(char *buf,u8 set_flg)
{
	Uart3[Cache_Cnt].Enter_mode = set_flg;
	if(Uart3[Cache_Cnt].Enter_mode == Link_Wifi)
	{
		Set_flag = 1;
		Uart3[Cache_Cnt].RecAdd = 0;
	}
	for(int i =0;i<strlen(buf);i++)
	{
		while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);  
		USART_SendData(USART3,buf[i]);
		if((i+1)%4096==0)
		{
			DelayTimes_ms=10;
			while (DelayTimes_ms);
		}
	}
}

void WIFI_Init(void)
{
		/********WIFIģ���ʼ��***********/
		char buf0[] = {'+','+','+','\0'};
		char buf1[] = {'A','T','+','C','W','M','O','D','E','_','D','E','F','=','1',
									'\r','\n','\0'};
		char buf2[] = {'A','T','+','C','W','J','A','P','_','D','E','F','=','"','J','E','S','O','O','"',',',
										'"','j','e','s','o','o','5','6','6','"',
										'\r','\n','\0'};
		char buf3[] = {'A','T','+','C','I','P','S','T','A','R','T','=','"','T','C','P','"',',','"','1','9','2','.','1','6','8','.','1','.','1','4','4','"',
										',','6','0','0','0',
										'\r','\n','\0'};
		char buf4[] = {'A','T','+','C','I','P','M','O','D','E','=','1',
										'\r','\n','\0'};
		char buf5[] = {'A','T','+','C','I','P','S','T','A','_','D','E','F','=',
									'"','1','9','2','.','1','6','8','.','1','.','8','8','"',',',
									'"','1','9','2','.','1','6','8','.','1','.','1','"',',',
									'"','2','5','5','.','2','5','5','.','2','5','5','.','0','"',
									'\r','\n','\0'};
		char buf6[] = {'A','T','+','C','I','P','S','E','N','D',
									'\r','\n','\0'};
		send_char(buf0,None);//+++	
		DelayTimes_ms = 1000;
		while(DelayTimes_ms);
		send_char(buf1,None);//AT+CWMODE_DEF=1
		DelayTimes_ms = 1000;
		while(DelayTimes_ms);
		send_char(buf2,Link_Wifi);//AT+CWJAP_DEF="JS-LIKU","jesoo123"		
		while(Set_flag);
		send_char(buf3,Link_Wifi);//AT+CIPSTART="TCP","192.168.0.144",6000
		//while(Set_flag);
		DelayTimes_ms = 1000;
										DelayTimes_ms = 1000;
									DelayTimes_ms = 1000;
									DelayTimes_ms = 1000;
		while(DelayTimes_ms);
		send_char(buf4,None);//AT+CIPMODE=1
		DelayTimes_ms = 1000;
		while(DelayTimes_ms);
		send_char(buf5,None);//AT+CIPSTA_DEF="192.168.1.1","192.168.1.116","255.255.255.0"
		DelayTimes_ms = 1000;
		while(DelayTimes_ms);
		send_char(buf6,None);//AT+CIPSEND
		DelayTimes_ms = 1000;
		while(DelayTimes_ms);
		Uart3[Cache_Cnt].Enter_mode =Rec_Data;
		Uart3[Cache_Cnt].RecAdd = 0;
		Uart3[0].Enter_mode = Rec_Data;
		Uart3[1].Enter_mode = Rec_Data;
		Uart3[2].Enter_mode = Rec_Data;
		Uart3[3].Enter_mode = Rec_Data;
		Uart3[4].Enter_mode = Rec_Data;
		for(int i=0;i<100;i++)
		{
			Uart3[Cache_Cnt].Buffer[i] = 0;
		}
}

/************************************************
* �������ƣ�u8 WIFI_Set()
* ����������WIFI��������
* ���ؽ����0�ɹ� 1ʧ��
*************************************************/


/************************************************
* �������ƣ�void Uart_Send(u8 Length,u8 cmd,u8 net_num,u8 Port_cfg,u8 comdata,u8 Buf[])
* �������������ڷ������磬���ݳ��ȣ�ָ����ص�ַ��֡��Ϣ��ת��λָ���չ�ֽ�
* ���ؽ������
*************************************************/
void Uart_Send(u8 Length,u8 cmd,u8 net_num,u8 Port_cfg,u8 comdata,u8 Buf[])
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
		while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);  
		//�ȴ�������ɡ�   ��� USART_FLAG_TC �Ƿ���1��    //���⺯�� P359 ����
		USART_SendData(USART3,send_buf[i]);
	}
}

void Uart_Analysis()
{
	static u8 CheckAdd=0;
	if(Cache_Cnt)
		{
			if((test_add>0)&&(Uart3[Cache_Cnt-1].Buffer[COMEND]!=0x06))//��ǰ�����˶����ҳ���ָֹͣ���ⲻ������������
			{
				return ;
			}
			for(int i=2;i<(10+(Uart3[Cache_Cnt-1].Buffer[Rec_Length]<<3));i++)
			{
					if( (i==7) || (i==((Uart3[Cache_Cnt-1].Buffer[Rec_Length]<<3)+9)) || (i==((Uart3[Cache_Cnt-1].Buffer[Rec_Length]<<3)+8)) )//i��2��ʼ�ӣ���ʾ֡ͷ������У�飬i==7ΪУ��λ i==size-1Ϊ֡β
					{
						continue;
					}
					CheckAdd+=Uart3[Cache_Cnt-1].Buffer[i];//��У�����
			}
				if(CheckAdd == Uart3[Cache_Cnt-1].Buffer[CHECKADD]&&
					Uart3[Cache_Cnt-1].Buffer[Uart_Head_1]==0xAA&&
					Uart3[Cache_Cnt-1].Buffer[Uart_Head_2]==0x55&&
					Uart3[Cache_Cnt-1].Buffer[NETNUM]==Local_Ip
					)//�ж�֡ͷ��֡β����У�飬���ص�ַ
					{					
						CheckAdd = 0;
						Uart3[Cache_Cnt-1].RecAdd = 0;			
					}
					else//��У���������
					{
						Uart3[Cache_Cnt-1].RecAdd=0;
						memset(&Uart3[Cache_Cnt-1].Buffer,0,sizeof(Uart3[Cache_Cnt-1].Buffer));//�������
						Beep(50,50,5);
						return;
					}
			Action.Comend  = Uart3[Cache_Cnt-1].Buffer[COMEND];//ָ�
			Action.command = Uart3[Cache_Cnt-1].Buffer[COMDATA];//ת��λָ��
			Action.Target_X = (u16)(Uart3[Cache_Cnt-1].Buffer[8]<<8)+ Uart3[Cache_Cnt-1].Buffer[9];//Ŀ��X������
			Action.Target_Z = (u16)(Uart3[Cache_Cnt-1].Buffer[10]<<8)+ Uart3[Cache_Cnt-1].Buffer[11];//Ŀ��
			Action.tray_position = Uart3[Cache_Cnt-1].Buffer[12];//��������
			Action.PlatX_Axis = (u16)(Uart3[Cache_Cnt-1].Buffer[13]<<8)+Uart3[Cache_Cnt-1].Buffer[14];//�м�X������
			Action.PlatZ_Axis = (u16)(Uart3[Cache_Cnt-1].Buffer[15]<<8)+Uart3[Cache_Cnt-1].Buffer[16];//�м�Z������
			memset(&Uart3[Cache_Cnt-1].Buffer,0,sizeof(Uart3[Cache_Cnt-1].Buffer));//�������
//					for(int i=0;i<26;i++)//�������
//					{
//						Uart3[Cache_Cnt-1].Buffer[i]=0;
//					}
			Cache_Cnt--;
		switch(Action.Comend)
		{
			case CMD_REPLY://����Ӧ��֡ 00
			{
//				if(NetRecBuff[1].ComData == Tx_Buffer[3])//���Ӧ�������ָ������Ϊ��ǰ���ͻ����ָ�����ΪӦ��ɹ�
//				{
//					NetSendingFlag = 0;
//				}
			}break;
			/*****  01  *****/
			case CMD_READVER://��ȡ�汾�� 01
			{		
//					Net_Send(NetRecBuff[1].PROT_CFG,CMD_READVER,TERMINAL_VERSION,0,0);
			}break;
			/*****  02  *****/
			case CMD_QUERYSTATE://��ѯ��ǰX�ᣬZ������λ�ã��Լ������ٶȵ�λ
			{
				Current_Position_Buff[0] = ((Axis_X.Coordinate*X_MPP)+0.5)/256;//X����
				Current_Position_Buff[1] = (u8)(((Axis_X.Coordinate*X_MPP)+0.5)/1)%256;//X����
				Current_Position_Buff[2] = ((Axis_Z.Coordinate*Z_MPP)+0.5)/256;//Z����
				Current_Position_Buff[3] =	(u8)(((Axis_Z.Coordinate*Z_MPP)+0.5)/1)%256;//Z����
				if(Axis_Y.YCoordinate<0)	
				Current_Position_Buff[4] = 1;
				else
				Current_Position_Buff[4] = 0;
				Current_Position_Buff[5] = (u8)(((Axis_Y.YCoordinate*Y_MPP)+0.5)/1)%256;
				if(Action.Step_Grade!=1000)
				{
					Current_Position_Buff[6] = Action.Step_Grade+1;//���ز�����λ
				}
				else
				{
					Current_Position_Buff[6] = 0xf0;//���ز�����λ
				}
				Current_Position_Buff[7] = Action.Speed_Grade;//�����ٶȵ�λ
				Uart_Send(1,CMD_QUERYSTATE,Local_Ip,Prot_Cfg,0,Current_Position_Buff);//��������
				Beep(50,50,1);
			}break;
			/*****  03  *****/
			case CMD_CTRL_XZ://����X��Z�ᵽָ��λ��	03
			{
				switch (Action.command)
				{
					case Get_Left_Box://ץ����
					{
						if((Axis_X.InCtrl_Flag==0) && (Axis_Z.InCtrl_Flag==0)&&(Axis_Y.InCtrl_Flag==0))//�����ڿ��в��ܿ���
						{
						if(Proximity_1 == 1||Proximity_2 == 1||Proximity_3 == 1)//����ǰ�ж������Ƿ�λ����λ������ִ�ж���
						{
							Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,0xff,0);//��ǰ����δ������λ��������Ϣ
							return ;
						}
							Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,First_Stage,0);//������Ϣ�ɹ�������
							Motor_CtrlOver_Flag = 1;
							Action.Get_Box_Start_Flag = 1;
							Action.Get_Box_Stage = 1;
							Action.XZRun_Stage = 1;
							Action.XZRUN_Status_Change = 1;
							Action.Tray_PutBox_Stage = 1;
							Action.Tray_PutBox_Status_Change = 1;
							Action.Tray_GetBox_Stage = 1;
							Action.Tray_GetBox_Status_Change = 1;
							Action.Target_Z = Action.Target_Z-AxisZ_Float_Disance;
							Action.end_flag = 0;
							Beep(50,50,1);
						}
					}break;
					case Get_Right_Box://ץ����
					{
						if((Axis_X.InCtrl_Flag==0) && (Axis_Z.InCtrl_Flag==0) &&(Axis_Y.InCtrl_Flag==0))//�����ڿ��в��ܿ���
						{
						if(Proximity_1 == 1||Proximity_2 == 1||Proximity_3 == 1)//����ǰ�ж������Ƿ�λ����λ������ִ�ж���
						{
							Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,0xff,0);//��ǰ����δ������λ��������Ϣ
							return ;
						}
							Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,First_Stage,0);//������Ϣ�ɹ�������
							Motor_CtrlOver_Flag = 1;
							Action.Get_Box_Start_Flag = 1;
							Action.Get_Box_Stage = 1;
							Action.XZRun_Stage = 1;
							Action.XZRUN_Status_Change = 1;
							Action.Tray_PutBox_Stage = 1;
							Action.Tray_PutBox_Status_Change = 1;
							Action.Tray_GetBox_Stage = 1;
							Action.Tray_GetBox_Status_Change = 1;
							Action.Target_Z = Action.Target_Z-AxisZ_Float_Disance;
							Action.end_flag = 0;
						Beep(50,50,1);
						}
					}break;
					case Return_Left_Box://������
					{
						if((Axis_X.InCtrl_Flag==0) && (Axis_Z.InCtrl_Flag==0) &&(Axis_Y.InCtrl_Flag==0))//�����ڿ��в��ܿ���
						{
							if(Proximity_1 == 1||Proximity_2 == 1||Proximity_3 == 1)//����ǰ�ж������Ƿ�λ����λ������ִ�ж���
							{
								Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,0xff,0);//��ǰ����δ������λ��������Ϣ
								return ;
							}
							Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,First_Stage,0);//������Ϣ�����ر���
							Motor_CtrlOver_Flag = 1;
							Action.Put_Box_Start_Flag = 1;
							Action.Put_Box_Stage = 1;
							Action.XZRun_Stage = 1;
							Action.XZRUN_Status_Change = 1;
							Action.Tray_PutBox_Stage = 1;
							Action.Tray_PutBox_Status_Change = 1;
							Action.Tray_GetBox_Stage = 1;
							Action.Tray_GetBox_Status_Change = 1;
							Action.end_flag = 0;
							Action.Target_Z = Action.Target_Z+AxisZ_Float_Disance;
							Beep(50,50,1);
						}
					}break;
					case Return_Right_Box://������
					{
						if((Axis_X.InCtrl_Flag==0) && (Axis_Z.InCtrl_Flag==0) &&(Axis_Y.InCtrl_Flag==0))//�����ڿ��в��ܿ���
						{
							if(Proximity_1 == 1||Proximity_2 == 1||Proximity_3 == 1)//����ǰ�ж������Ƿ�λ����λ������ִ�ж���
							{
								Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,0xff,0);//��ǰ����δ������λ��������Ϣ
								return ;
							}
							Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,First_Stage,0);//������Ϣ�����ر���
							Motor_CtrlOver_Flag = 1;
							Action.Put_Box_Start_Flag = 1;
							Action.Put_Box_Stage = 1;
							Action.XZRun_Stage = 1;
							Action.XZRUN_Status_Change = 1;
							Action.Tray_PutBox_Stage = 1;
							Action.Tray_PutBox_Status_Change = 1;
							Action.Tray_GetBox_Stage = 1;
							Action.Tray_GetBox_Status_Change = 1;
							Action.end_flag = 0;
							Action.Target_Z = Action.Target_Z+AxisZ_Float_Disance;
							Beep(50,50,1);
						}
					}break;
					case Reset://��λ
					{
						if((Axis_X.InCtrl_Flag==0) && (Axis_Z.InCtrl_Flag==0) && (Axis_Y.InCtrl_Flag==0))//�����ڿ��в��ܿ���
						{
							Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,Successed,0);//������Ϣ�����ر���
							XYZ_To_Zero.Start_Flag = 1;
							Beep(50,50,1);
						}
					}break;
					case Arrive://����
					{
						if((Axis_X.InCtrl_Flag==0) && (Axis_Z.InCtrl_Flag==0) && (Axis_Y.InCtrl_Flag==0))//�����ڿ��в��ܿ���
						{
							if(Proximity_1 == 1||Proximity_2 == 1||Proximity_3 == 1)//����ǰ�ж������Ƿ�λ����λ������ִ�ж���
							{
								Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,0xff,0);//��ǰ����δ������λ��������Ϣ
								return ;
							}
							Motor_CtrlOver_Flag = 1;
							Action.Get_Box_Start_Flag = 1;
							Action.Get_Box_Stage = 1;
							Action.end_flag = 0;
							Action.XZRun_Stage = 1;
							Action.XZRUN_Status_Change = 1;
//							Action.tray_position = Uart3.Buffer[12];
							Beep(50,50,1);
						}
					}break;
				}
				
			}break;
			/*****  04  *****/
			case CMD_INDVICTRL_XYZ://��������X,Y,Z��
			{
				Action.Move_Alone = 1;//��λ�������Ʊ�־λ
				Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,Successed,0);//���ؽ�����Ϣ�ɹ�
				switch(Action.command&0X30)
				{
					/**************X�ᶯ��*****************/
					case X :
					{
						if(Proximity_1 == 1||Proximity_2 == 1||Proximity_3 == 1)//����ǰ�ж������Ƿ�λ����λ������ִ�ж���
						{
							Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,0xff,0);//��ǰ����δ������λ��������Ϣ
							return ;
						}
						switch(Action.command&0X01)
						{
							case X_BACK://X�����
							{	
								Axis_Move_Alone(AXIS_X , X_DIR_BACK);
							}break;
							case X_FRONT://X��ǰ��
							{
								Axis_Move_Alone(AXIS_X , X_DIR_FRONT);
							}break;
						}
					}break;
					/**************Y�ᶯ��*****************/
					case Y :
					{
						switch(Action.command&0X01)
						{
							case Y_LEFT://Y��������
							{
								Axis_Move_Alone(AXIS_Y , Y_DIR_LEFT);
							}break;
							case Y_RIGHT://Y��������
							{
								Axis_Move_Alone(AXIS_Y , Y_DIR_RIGHT);
							}break;
						}
					}break;
					/**************Z�ᶯ��*****************/
					case Z:
					{
						if(Proximity_1 == 1||Proximity_2 == 1||Proximity_3 == 1)//����ǰ�ж������Ƿ�λ����λ������ִ�ж���
						{
							Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,0xff,0);//��ǰ����δ������λ��������Ϣ
							return ;
						}
						switch(Action.command&0X01)
						{
							case Z_BACK://Z���½�
							{
								Axis_Move_Alone(AXIS_Z , Z_DIR_BACK);
							}break;
							case Z_FRONT://Z������
							{
								Axis_Move_Alone(AXIS_Z , Z_DIR_FRONT);
							}break;
						}
					}break;
				}
				Beep(50,50,1);
			}break;
			/*****  05  *****/
			case CMD_SPEED_STEP://���ò��������ٶȵ�λ
			{
				Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,Successed,0);//�������óɹ���Ϣ
				u8 temp_para;
				temp_para = Action.command>>4;
				if(temp_para>0)
				{
					temp_para = 0x10;
				}
				else
				{
					temp_para = 0x00;
				}
				switch(temp_para&0x10)
				{
					case Speed_Stage://�ٶȵ�λ
					{
						switch(Action.command&0x0f)
						{
							case Casual_Fre_1://�ٶȵ�λ1
							{
								Action.Speed_Grade = 0;
							}break;
							case Casual_Fre_2://�ٶȵ�λ2
							{
								Action.Speed_Grade = 1;
							}break;
							case Casual_Fre_3://�ٶȵ�λ3
							{
								Action.Speed_Grade = 2;
							}break;
							case Casual_Fre_4://�ٶȵ�λ4
							{
								Action.Speed_Grade = 3;
							}break;
							case Casual_Fre_5://�ٶȵ�λ5
							{
								Action.Speed_Grade = 4;
							}break;
							case Casual_Fre_6://�ٶȵ�λ6
							{
								Action.Speed_Grade = 5;
							}break;
							case Casual_Fre_7://�ٶȵ�λ7
							{
								Action.Speed_Grade = 6;
							}break;
							default:
							{
								Action.Speed_Grade = 0;
							}break;
						
						}
					}break;
					case Step_Stage://������λ
					{
						switch(Action.command&0xf0)
						{
							case Casual_Ste_1://����1
							{
								Action.Step_Grade = 0;
							}break;
							case Casual_Ste_2://����2
							{
								Action.Step_Grade = 1;
							}break;
							case Casual_Ste_3://����3
							{
								Action.Step_Grade = 2;
							}break;
							case Casual_Ste_4://����4
							{
								Action.Step_Grade = 3;
							}break;
							case Casual_Ste_5://����5
							{
								Action.Step_Grade = 4;
							}break;
							case Casual_Ste_6://����6
							{
								Action.Step_Grade = 5;
							}break;
							case Casual_Ste_7://����7
							{
								Action.Step_Grade = 6;
							}break;
							case Casual_Ste_None://�涯����
							{
								Action.Step_Grade = 1000;
							}break;
								default://����4
							{
								Action.Step_Grade = 0;
							}break;
						}
					}break;
				}
				Beep(50,50,1);
			}break;
			/*****  06  *****/
			case CMD_STOP://ָֹͣ���Լ�������λ���
			{
				Uart_Send(0,CMD_CTRL_XZ,Local_Ip,0x61,Successed,0);//���ؽ�����Ϣ�ɹ�
				if(Action.command == 0x00)//ָֹͣ��
				{
				Axis_X.InCtrl_Flag = 0;
				TIM_Cmd(TIM2, DISABLE);
				Axis_Y.InCtrl_Flag = 0;
				TIM_Cmd(TIM3, DISABLE);
				Axis_Z.InCtrl_Flag = 0;
				TIM_Cmd(TIM4, DISABLE);
				Action.Get_Box_Start_Flag = 0;
				Action.Put_Box_Start_Flag = 0;
				Action.Target_Z = 0;
				Action.Target_X = 0;
				Motor_CtrlOver_Flag = 1;
				Action.Get_Box_Stage = 1;
				Action.Put_Box_Stage = 1;
				Action.XZRun_Stage = 1;
				Action.XZRUN_Status_Change = 1;
				Action.Tray_PutBox_Stage = 1;
				Action.Tray_PutBox_Status_Change = 1;
				Action.Tray_GetBox_Stage = 1;
				Action.Tray_GetBox_Status_Change = 1;
				Action.end_flag = 0;
				/******************ֹͣ���ص�ǰ����*************/
				Current_Position_Buff[0] = ((Axis_X.Coordinate*X_MPP)+0.5)/256;
				Current_Position_Buff[1] = (u8)(((Axis_X.Coordinate*X_MPP)+0.5)/1)%256;
				Current_Position_Buff[2] = ((Axis_Z.Coordinate*Z_MPP)+0.5)/256;
				Current_Position_Buff[3] =	(u8)(((Axis_Z.Coordinate*Z_MPP)+0.5)/1)%256;
				if(Axis_Y.YCoordinate<0)
				{					
					Current_Position_Buff[4] = 1;
				}
				else
				{
					Current_Position_Buff[4] = 0;
				}
				Current_Position_Buff[5] = (u8)(((Axis_Y.YCoordinate*Y_MPP)+0.5)/1)%256;
				if(Action.Step_Grade!=1000)
				{
					Current_Position_Buff[6] = Action.Step_Grade+1;//���ز�����λ
				}
				else
				{
					Current_Position_Buff[6] = 0xf0;//���ز�����λ
				}
				Current_Position_Buff[7] = Action.Speed_Grade;//�����ٶȵ�λ
				Uart_Send(1,CMD_QUERYSTATE,Local_Ip,Prot_Cfg,0,Current_Position_Buff);
				}
				else//������λ
				{
					PAout(5) = 1;
					Reset_Flag = 1;
					delay_ms(1500);
					delay_ms(1500);
				}
				Beep(50,50,1);
			}break;
			case CMD_TRAYSET0://�м�Y��������
			{
				Uart_Send(0,CMD_TRAYSET0,Local_Ip,0x61,Successed,0);//���ؽ�����Ϣ�ɹ�
				Axis_Y.YCoordinate = 0;
				Beep(50,50,1);
			}break;
			
		}		
	}
}

		
void USART3_IRQHandler(void)	//����3�жϷ������
{	
	USART_ClearITPendingBit(USART3,USART_IT_RXNE); 
	/***********���Ľ���******************/
	if(Uart3[Cache_Cnt].Enter_mode ==Rec_Data)//����ͨѶЭ��
	{
		Uart3[Cache_Cnt].RxByte = USART_ReceiveData(USART3);	//���������ݴ�
		Uart3[Cache_Cnt].Buffer[Uart3[Cache_Cnt].RecAdd] = Uart3[Cache_Cnt].RxByte;
		Uart3[Cache_Cnt].RecAdd++;
		if((Uart3[Cache_Cnt].Buffer[Uart3[Cache_Cnt].Buffer[Rec_Length]*8+8]) == 0xA0&&(Uart3[Cache_Cnt].Buffer[Uart3[Cache_Cnt].Buffer[Rec_Length]*8+9]) == 0x5F)
		{
			Cache_Cnt++;//����һ�����ݻ���
			if(Cache_Cnt>19)
			{
				Cache_Cnt = 0;
			}
			Uart3[Cache_Cnt].RecAdd = 0;//��һ����������������0
		}
	}
	else if (Uart3[Cache_Cnt].Enter_mode ==Link_Wifi)//Link_Wifi
	{
		Uart3[Cache_Cnt].RxByte = USART_ReceiveData(USART3);	//���������ݴ�
		Uart3[Cache_Cnt].Buffer[Uart3[Cache_Cnt].RecAdd] = Uart3[Cache_Cnt].RxByte;
		Uart3[Cache_Cnt].RecAdd++;
		if(Uart3[Cache_Cnt].RecAdd>6&&
			 Uart3[Cache_Cnt].Buffer[Uart3[Cache_Cnt].RecAdd-1]=='K'&&
			 Uart3[Cache_Cnt].Buffer[Uart3[Cache_Cnt].RecAdd-2]=='O'
			)
		{
			Set_flag = 0;
		}
	}
	else//None��������
	{

	}

} 



