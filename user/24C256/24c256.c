#include "24c256.h"



unsigned char IDcard_data;
/***********************************************************************
* 名称 : I2C_delay()
* 功能 : 小延时
* 输入 : 无
* 输出 : 无
***********************************************************************/
void I2C_delay(void)
{	
    u16 i=6; //这里可以优化速度，经测试最低到5还能写入
    while(i)
    {
        i--;
    }
}

/***********************************************************************
* 名称 : IIC_Init()
* 功能 : 初始化
* 输入 : 无
* 输出 : 无
***********************************************************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// ----------IIC 引脚定义---------------
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/***********************************************************************
* 名称 : I2C_Start()
* 功能 : 产生一个起始信号
* 输入 : 无
* 输出 : 无
***********************************************************************/
unsigned char I2C_Start(void)
{
    SDA_H;
		I2C_delay();
    SCL_H;
    I2C_delay();
//    if(!SDA_read) return 0;	//SDA线为低电平则总线忙,退出
    SDA_L;
    I2C_delay();
//    if(SDA_read)  return 0;	//SDA线为高电平则总线出错,退出
    SCL_L;
    I2C_delay();
    return 1;
}


/***********************************************************************
* 名称 : I2C_Stop()
* 功能 : 产生一个停止信号
* 输入 : 无
* 输出 : 无
***********************************************************************/
void I2C_Stop(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SDA_H;
    I2C_delay();
}

/***********************************************************************
* 名称 : I2C_Ack()
* 功能 : 产生一个应答信号
* 输入 : 无
* 输出 : 无
***********************************************************************/
void I2C_Ack(void)
{	
    SDA_L;
    I2C_delay();
	
    SCL_H;
    I2C_delay();
		SCL_L;
    I2C_delay();
	
//    SDA_H;
//    I2C_delay();
}

/***********************************************************************
* 名称 : I2C_NoAck()
* 功能 : 产生一个非应答信号
* 输入 : 无
* 输出 : 无
***********************************************************************/
void I2C_NoAck(void)
{	
	  I2C_delay();
	
    SDA_H;
    I2C_delay();

//		SCL_L;
//    I2C_delay();
    SCL_H;
    I2C_delay();
	
    SCL_L;
    I2C_delay();
}

/***********************************************************************
* 名称 : I2C_WaitAck()
* 功能 : 读一个应答信号
* 输入 : 无
* 输出 : 返回为TRUE有ACK,返回FALSE无ACK
***********************************************************************/
unsigned char I2C_WaitAck(void) 	
{
    SCL_L;
    I2C_delay();
    SDA_H;			
    I2C_delay();
    SCL_H;
    I2C_delay();
    if(SDA_read)
    {
        SCL_L;
        return 0;
    }
    SCL_L;
		I2C_delay();
    return 1;
}

/***********************************************************************
* 名称 : I2C_SendByte()
* 功能 : 发送一个字节
* 输入 : 无
* 输出 : 无
***********************************************************************/
void I2C_SendByte(u8 SendByte) //数据从高位到低位
{
    u8 i=8;
    while(i--)
    {       
        I2C_delay();
        if(SendByte&0x80) SDA_H;
        else SDA_L;
        SendByte <<= 1;
        I2C_delay();
        SCL_H;
        I2C_delay();
				SCL_L;
    }
    I2C_delay();
}

/***********************************************************************
* 名称 : I2C_ReceiveByte()
* 功能 : 接收一个字节
* 输入 : 无
* 输出 : 接收到的数据
***********************************************************************/
unsigned char I2C_ReceiveByte(void)  //数据从高位到低位
{
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
        ReceiveByte<<=1;  
				I2C_delay();     
        SCL_H;
        I2C_delay();	
        if(SDA_read)
        {
            ReceiveByte|=0x01;
        }
				I2C_delay();
				SCL_L;			
    }
    I2C_delay();
    return ReceiveByte;
}

/***********************************************************************
* 名称 : I2C_WriteByte()
* 功能 : 向EEPROM写一个字节
* 输入 : SendByte要发送的数据，WriteAddress要写入的地址，DeviceAddress从器件地址
* 输出 : TRUE正确，FALSE发送失败
***********************************************************************/
//unsigned char I2C_WriteByte(u8 SendByte, u16 WriteAddress, u8 DeviceAddress)
//{		
//    if(!I2C_Start())return 0;
//    I2C_SendByte(((WriteAddress & 0x0700) >>7) | DeviceAddress & 0xFFFE);//设置高起始地址+器件地址
//    if(!I2C_WaitAck()){I2C_Stop(); return 0;}
//    I2C_SendByte((u8)(WriteAddress & 0x00FF));   //设置低起始地址
//    I2C_WaitAck();	
//    I2C_SendByte(SendByte);
//    I2C_WaitAck();
//    I2C_Stop();
//	//注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
//    //Systick_Delay_1ms(10);
//	delay_ms(10);
//    return 1;
//}
unsigned char I2C_BufferWrite_Onebyte(u8 *pBuffer, u16 WriteAddress)
{
	static unsigned char WriteTimes=0;
	
write1:	
	WriteTimes++;
  I2C_Start();
	I2C_SendByte(0xa0);//设置高起始地址+器件地址
  if(!I2C_WaitAck())
	{
		if(WriteTimes<200)goto write1;//?? 
		else return 0;
	}
  I2C_SendByte(WriteAddress>>8);   //设置高起始地址
  if(!I2C_WaitAck())
	{
		if(WriteTimes<200)goto write1;//?? 
		else return 0;
	}
  I2C_SendByte(WriteAddress & 0x00FF);   //设置低起始地址
  if(!I2C_WaitAck())
	{
		if(WriteTimes<200)goto write1;//?? 
		else return 0;
	}
	I2C_SendByte(0xff - *pBuffer);
	if(!I2C_WaitAck())
	{
		if(WriteTimes<200)goto write1;//?? 
		 else return 0;
	}
//	pBuffer++;
//	WriteAddress++;
	I2C_Stop();
	
	//注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
	//Systick_Delay_1ms(10);
	DelayTimes_ms=10;
	while (DelayTimes_ms);
	return 1;
}
/***********************************************************************
* 名称 : I2C_BufferWrite()
* 功能 : 向EEPROM页写一串字节，不能跨页写，一页在8个字节以内
* 输入 : * pBuffer要发送的数据，length发送数据的个数，
         WriteAddress要写入的地址，DeviceAddress从器件地址
* 输出 : TRUE正确，FALSE发送失败
***********************************************************************/
unsigned char I2C_BufferWrite(u8 *pBuffer, u16 length, u16 WriteAddress)
{
	static unsigned char WriteTimes=0;
	static unsigned int length_temp=0;
	static unsigned int  WriteAddress_temp;
	length_temp=length;
	WriteAddress_temp=WriteAddress;
write1:	
	if(length>=length_temp)WriteAddress_temp=WriteAddress_temp+(length-length_temp);
	WriteTimes++;
  I2C_Start();
	I2C_SendByte(0xa0);//设置高起始地址+器件地址
  if(!I2C_WaitAck())
	{
		if(WriteTimes<200)goto write1;//?? 
		else return 0;
	}
  I2C_SendByte(WriteAddress_temp>>8);   //设置高起始地址
  if(!I2C_WaitAck())
	{
		if(WriteTimes<200)goto write1;//?? 
		else return 0;
	}
  I2C_SendByte(WriteAddress_temp & 0x00FF);   //设置低起始地址
  if(!I2C_WaitAck())
	{
		if(WriteTimes<200)goto write1;//?? 
		else return 0;
	}
	while(length_temp)
	{
		I2C_SendByte(0xff - *pBuffer);
		if(!I2C_WaitAck())
		{
//			if(WriteTimes<200)goto write1;//?? 
//			 else return 0;
		}
		pBuffer++;
		length_temp--;
	}
	I2C_Stop();
	
	//注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
	//Systick_Delay_1ms(10);
	DelayTimes_ms=10;
	while (DelayTimes_ms);
	return 1;

//	unsigned int i;
//	
//	for(i=0;i<length;i++)
//	{
//		I2C_BufferWrite_Onebyte(pBuffer+i, WriteAddress+i);
//	}

//	return 1;

}

/***********************************************************************
* 名称 : I2C_PageWrite()
* 功能 : 向EEPROM页写一串字节，连续页写
* 输入 : * pBuffer要发送的数据，length发送数据的个数，
         WriteAddress要写入的地址，DeviceAddress从器件地址
         24C256   page 64byte
				 24C512   page 128byte******************************************
         24C1024  page 256byte
* 输出 : TRUE正确，FALSE发送失败
***********************************************************************/
void I2C_PageWrite(u8* pBuffer, u16 length, u16 WriteAddress)
{
	unsigned int I2C_PageSize=64;
	unsigned char NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	Addr  = WriteAddress % I2C_PageSize;      //写入地址是开始页的第几位
	count = I2C_PageSize - Addr;					    //在开始页要写入的个数(从写入地址写到写满页的个数)
	NumOfPage   =  length / I2C_PageSize;     //要写入的页数
	NumOfSingle =  length % I2C_PageSize;     //不足一页的个数

	if(Addr == 0)         //写入地址是页的开始
	{
		if(NumOfPage == 0)  //数据小于一页
		{
			I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress);   //写少于一页的数据
			DelayTimes_ms=12;
			while (DelayTimes_ms);
		}
		else		            //数据大于等于一页
		{
			while(NumOfPage)//要写入的页数
			{
				I2C_BufferWrite(pBuffer,I2C_PageSize,WriteAddress);//写一页的数据
				WriteAddress +=  I2C_PageSize;
				pBuffer      +=  I2C_PageSize;
				NumOfPage--;
				DelayTimes_ms=12;
				while (DelayTimes_ms);
			}
			if(NumOfSingle!=0)//剩余数据小于一页
			{
				I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress); //写少于一页的数据
				DelayTimes_ms=12;
				while (DelayTimes_ms);
			}
		}
	}
	else                  //写入地址不是页的开始
	{
		if(NumOfPage== 0)   //数据小于一页
		{
			if(NumOfSingle<=count)I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress);   //写入数据不会超过页边界
			else                                                                       //写入数据会超过页边界
			{
				I2C_BufferWrite(pBuffer,count,WriteAddress);      //将开始的地址写满一页  count=从开始页要写入的个数(从写入地址写到写满页的个数)
				WriteAddress += count;
				pBuffer      += count;
				DelayTimes_ms=12;
				while (DelayTimes_ms);
				I2C_BufferWrite(pBuffer,NumOfSingle-count,WriteAddress);   //写跨页的剩余数据
			}
			DelayTimes_ms=12;
			while (DelayTimes_ms);
		}
		else                //数据大于等于一页
		{
			length       -= count;
			NumOfPage     = length / I2C_PageSize;  //重新计算要写入的页数
			NumOfSingle   = length % I2C_PageSize;  //重新计算不足一页的个数	

			if(count != 0)
			{
				I2C_BufferWrite(pBuffer,count,WriteAddress);      //将开始的空间写满一页
				WriteAddress += count;
				pBuffer      += count;
				DelayTimes_ms=12;
				while (DelayTimes_ms);
			}

			while(NumOfPage--)  //要写入的页数
			{
				I2C_BufferWrite(pBuffer,I2C_PageSize,WriteAddress);//写一页的数据
				WriteAddress +=  I2C_PageSize;
				pBuffer      +=  I2C_PageSize;
				DelayTimes_ms=12;
				while (DelayTimes_ms);
			}
			if(NumOfSingle != 0)//剩余数据小于一页
			{
				I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress); //写少于一页的数据
				DelayTimes_ms=12;
				while (DelayTimes_ms);
			}
		}
	}

//I2C_BufferWrite(pBuffer, length, WriteAddress);
}

/***********************************************************************
* 名称 : I2C_PageWrite()
* 功能 : 读取EEPROM数据
* 输入 : *pBuffer接收到的数据地址指针，length接收数据的个数，
         WriteAddress要读数据的地址，DeviceAddress从器件地址
* 输出 : TRUE读正确，FALSE读取失败
* 使用 ：I2C_ReadByte(&i, 1, 0x0002, 0xa0);  &i为变量i的地址
***********************************************************************/
unsigned char I2C_ReadByte(u8* pBuffer,   u16 length,     u16 ReadAddress)
{	
		static unsigned char ReadTimes=0;
	
read1:  
		ReadTimes++;	
		I2C_Start();
		I2C_SendByte(0xa0);//设置高起始地址+器件地址
		if(!I2C_WaitAck())
		{
			if(ReadTimes<200)goto read1;//?? 
			else return 0;
		}
    I2C_SendByte(ReadAddress>>8);                           //设置高起始地址
		if(!I2C_WaitAck())
		{
			if(ReadTimes<200)goto read1;//?? 
			else return 0;
		}
    I2C_SendByte(ReadAddress & 0x00FF);                           //设置低起始地址
		if(!I2C_WaitAck())
		{
			if(ReadTimes<200)goto read1;//?? 
			else return 0;
		}

    I2C_Start();
    I2C_SendByte(0xa1);
		if(!I2C_WaitAck())
		{
			if(ReadTimes<200)goto read1;//?? 
			else return 0;
		}
    while(length)
    {
		*pBuffer = 0xff-I2C_ReceiveByte();
		if(length == 1)I2C_NoAck();
		else I2C_Ack();
		pBuffer++;
		length--;
    }
    I2C_Stop();
//	delay_ms(2);
    return 1;
}






