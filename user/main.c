 
#include "Sys_Init.h"	
int main(void)
{
	/*******�ϵ��ʼ��*******/
	Sys_Init();		
	
	/***************************��ѭ��****************************/
	while(1)
	{
		if(Reset_Flag ==1)//������λ�������־λ
		{
			PAout(5) = 0;
			Reset_Flag=0;
		}
		Uart_Analysis();//���ڽ���
		Axis_Ultra_Limit();//XZ��������λ
		Tray_Limit_Contr();//������λ
		if(Timer_1ms_flag == 1)//1ms��ѯ��ִ����Ҫ��ʱ�Ĺ���   ��������/ģ�����
		{
			Timer_1ms_flag = 0;
			//Return_SendMesge();//��λ������Ϣ
			XYZ_BackZero();//���Ḵλ�׶���ѯ
			Key_Deal();//����ɨ��
			SURE_REC_UART2_Analyze();     //���������ڽ���
			Get_Box(Action.Target_X,Action.Target_Z);//ȡ��׶���ѯ
			Put_Box(Action.Target_X,Action.Target_Z);//����׶���ѯ
		}
	}
}


int fputc(int ch, FILE *f)
{
USART_SendData(USART3, (unsigned char) ch);
while (!(USART3->SR & USART_FLAG_TXE));

return (ch);

}

