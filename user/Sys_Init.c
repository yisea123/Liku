
#include "Sys_Init.h"

void Sys_Init(void)
{		

	/************ϵͳ��ʼ��*******************************************************/
	/*����*/
	delay_init();	    		//��ʱ������ʼ��	  
	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	Beep_Init();					//��������ʼ��
	KEY_Init();						//������ʼ��
	GPIO_OutPut_Init();
	GPIO_InPut_Init();
	PWM_Init();
	IIC_Init();
	USART3_Init(115200);		//����3��ʼ�� ������9600
	USART2_Init(115200);		//����2��������ʼ��
	NVIC_Configuration(); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	TIM3_Int_Init(9,7199);//10Khz�ļ���Ƶ�ʣ�������1000Ϊ1ms 	
	IWDG_Init(6,3000);//20����ι��һ��
	//WIFI_Init();
	Reset_Flag = 0;
	for(int i=0;i<20;i++)
	{
		for(int j =0;j<100;j++)
		{
			Uart3[j].Buffer[j] = 0;			
		}
		Uart3[i].Enter_mode = Rec_Data;
	}
	
  All_I2C_Read();
	
	Change_X_MPP.X_MPP_Data = Change_X_MPP.X_MPP_Data;
	Change_X_MPP.X_MPP_Data = Change_X_MPP.X_MPP_Data;
	
	Change_X_Zero_Pluse.X_Zero_Pluse_Data = Change_X_Zero_Pluse.X_Zero_Pluse_Data;
	Change_X_Zero_Pluse.X_Zero_Pluse_Data = Change_X_Zero_Pluse.X_Zero_Pluse_Data;
	
	Change_First_Goods_Height.First_Goods_Height_Data = Change_First_Goods_Height.First_Goods_Height_Data;
  Change_First_Goods_Height.First_Goods_Height_Data = Change_First_Goods_Height.First_Goods_Height_Data;	
	
	Change_Z_MPP.Z_MPP_Data = Change_Z_MPP.Z_MPP_Data;
	Change_Z_MPP.Z_MPP_Data = Change_Z_MPP.Z_MPP_Data;
	
	Change_Plat_Axis_Z.Plat_Axis_Z_Data = Change_Plat_Axis_Z.Plat_Axis_Z_Data;
	Change_Plat_Axis_Z.Plat_Axis_Z_Data = Change_Plat_Axis_Z.Plat_Axis_Z_Data;
	
	Change_Plat_Axis_X.Plat_Axis_X_Data = Change_Plat_Axis_X.Plat_Axis_X_Data;
	Change_Plat_Axis_X.Plat_Axis_X_Data = Change_Plat_Axis_X.Plat_Axis_X_Data;
	
	Change_Y_RightZero_Pluse.Y_RightZero_Pluse_Data = Change_Y_RightZero_Pluse.Y_RightZero_Pluse_Data;
	Change_Y_RightZero_Pluse.Y_RightZero_Pluse_Data = Change_Y_RightZero_Pluse.Y_RightZero_Pluse_Data;
	
	Change_Y_LeftZero_Pluse.Y_LeftZero_Pluse_Data = Change_Y_LeftZero_Pluse.Y_LeftZero_Pluse_Data;
	Change_Y_LeftZero_Pluse.Y_LeftZero_Pluse_Data = Change_Y_LeftZero_Pluse.Y_LeftZero_Pluse_Data;
	
	Change_Y_LeftTrayPluse.Y_LeftTrayPluse_Data = Change_Y_LeftTrayPluse.Y_LeftTrayPluse_Data;
	Change_Y_LeftTrayPluse.Y_LeftTrayPluse_Data = Change_Y_LeftTrayPluse.Y_LeftTrayPluse_Data;
	
	Change_Y_LeftLim_MorePluse.Y_LeftLim_MorePluse_Data = Change_Y_LeftLim_MorePluse.Y_LeftLim_MorePluse_Data;
	Change_Y_LeftLim_MorePluse.Y_LeftLim_MorePluse_Data = Change_Y_LeftLim_MorePluse.Y_LeftLim_MorePluse_Data;
	
	Change_Y_LeftLim_LessPluse.Y_LeftLim_LessPluse_Data = Change_Y_LeftLim_LessPluse.Y_LeftLim_LessPluse_Data;
	Change_Y_LeftLim_LessPluse.Y_LeftLim_LessPluse_Data = Change_Y_LeftLim_LessPluse.Y_LeftLim_LessPluse_Data;
	
	Change_Y_RightLim_MorePluse.Y_RightLim_MorePluse_Data = Change_Y_RightLim_MorePluse.Y_RightLim_MorePluse_Data;
	Change_Y_RightLim_MorePluse.Y_RightLim_MorePluse_Data = Change_Y_RightLim_MorePluse.Y_RightLim_MorePluse_Data;
	
	Change_Y_RightLim_LessPluse.Y_RightLim_LessPluse_Data = Change_Y_RightLim_LessPluse.Y_RightLim_LessPluse_Data;
	Change_Y_RightLim_LessPluse.Y_RightLim_LessPluse_Data = Change_Y_RightLim_LessPluse.Y_RightLim_LessPluse_Data;
	
	Change_AxisZ_UpDown_Pluse.AxisZ_UpDown_Pluse_Data = Change_AxisZ_UpDown_Pluse.AxisZ_UpDown_Pluse_Data;
	Change_AxisZ_UpDown_Pluse.AxisZ_UpDown_Pluse_Data = Change_AxisZ_UpDown_Pluse.AxisZ_UpDown_Pluse_Data;
	
	Change_AxisZ_UpDownMore_Pluse.AxisZ_UpDownMore_Pluse_Data = Change_AxisZ_UpDownMore_Pluse.AxisZ_UpDownMore_Pluse_Data;
	Change_AxisZ_UpDownMore_Pluse.AxisZ_UpDownMore_Pluse_Data = Change_AxisZ_UpDownMore_Pluse.AxisZ_UpDownMore_Pluse_Data;
	Beep(50,50,1);
	
}


