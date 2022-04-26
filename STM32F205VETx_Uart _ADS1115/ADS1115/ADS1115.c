#include "ads1115.h"

//#ifdef ADS1115

ADS1115_InitTypeDefine ADS1115_InitType;
ADS1115_InitTypeDefine ADS1115_1;

float ADS1115_CHn_Vol[4];
/**
 * �ɹ��ⲿ���õ�ȫ�ֱ�������¼��ADS1115������ԭʼ16λ���ݡ�����void ADS1115_RefreshAllChannel( )��������ˢ�����������
 * ͨ�������ADS1115_USE_FILTER�����Խ�ADS1115����ѯ�������ݾ��������˲��󣬱��浽ADS1115_RawData[]�С�
 * ͨ��float ADS1115_RawDataToVoltage(int16_t rawData)�������Խ�ADS1115_RawData[]����ɶ�Ӧ�ĵ�ѹֵ��
 */
int16_t ADS1115_RawData[4] = {0};


/**
 * @brief ���оƬ���ƶ˿ڳ�ʼ���������ó�ʼ״̬
 */
void ADS1115_Init()
{
	

}


/**
 * @brief Configuration of ADS1115, single-shot
 */
void ADS1115_UserConfig_SingleConver(ADS1115_InitTypeDefine* ADS1115_InitStruct)
{
	ADS1115_InitStruct->COMP_LAT = ADS1115_COMP_LAT_0;
	ADS1115_InitStruct->COMP_MODE = ADS1115_COMP_MODE_0;
	ADS1115_InitStruct->COMP_POL = ADS1115_COMP_POL_0;
	ADS1115_InitStruct->DataRate = ADS1115_DataRate_475;
	ADS1115_InitStruct->MODE = ADS1115_MODE_SingleConver;
	ADS1115_InitStruct->MUX = ADS1115_MUX_Channel_0;
	ADS1115_InitStruct->OS = ADS1115_OS_SingleConverStart;
	ADS1115_InitStruct->PGA = ADS1115_PGA_4096;
	ADS1115_Config(ADS1115_InitStruct);
}


/**
 * @brief Configuration of ADS1115, continuous conversion
 */
void ADS1115_UserConfig_ContinuConver()
{
	ADS1115_InitType.COMP_LAT = ADS1115_COMP_LAT_0;
	ADS1115_InitType.COMP_MODE = ADS1115_COMP_MODE_0;
	ADS1115_InitType.COMP_POL = ADS1115_COMP_POL_0;
	ADS1115_InitType.DataRate = ADS1115_DataRate_475;
	ADS1115_InitType.MODE = ADS1115_MODE_ContinuConver;
	ADS1115_InitType.MUX = ADS1115_MUX_Channel_0;
	ADS1115_InitType.OS = ADS1115_OS_OperationalStatus;
	ADS1115_InitType.PGA = ADS1115_PGA_4096;

	ADS1115_Config(&ADS1115_InitType);
}


/**
 * @brief ����ADS1115
 * @param ADS1115_InitStruct: ��������ADS1115�Ľṹ�����ָ��
 * @return ���ý��
 * 		@arg: fail
 * 		@arg: success
 */
uint8_t ADS1115_Config(ADS1115_InitTypeDefine *ADS1115_InitStruct)
{
	uint16_t Config;
	uint8_t Writebuff[3];

	Config = ADS1115_InitStruct->OS + ADS1115_InitStruct->MUX + ADS1115_InitStruct->PGA + ADS1115_InitStruct->MODE
			+ ADS1115_InitStruct->DataRate + ADS1115_InitStruct->COMP_MODE + ADS1115_InitStruct->COMP_POL
			+ ADS1115_InitStruct->COMP_LAT + ADS1115_InitStruct->COMP_QUE;
  Writebuff[0] = ADS1115_Pointer_ConfigReg;
	Writebuff[1] = (unsigned char) ((Config >> 8) & 0xFF);
	Writebuff[2] = (unsigned char) (Config & 0xFF);
	HAL_I2C_Master_Transmit(&hi2c1, ADS1115_ADDRESS, Writebuff, 3, 100);
	return (1);
}



/**
 * @brief ��ȡADS1115��ǰͨ���µ�ԭʼ����
 * @param rawData: ����һ��int16_t���ͱ�����ָ�룬ADS1115��ԭʼ���ݽ����������������
 * @return ��ȡ���
 * 		@arg 0: fail
 * 		@arg 1: success
 */

uint8_t ADS1115_ReadRawData(int16_t *rawData)
{
	unsigned char Result[2];
	uint8_t Writebuff[1];
	Writebuff[0] = ADS1115_Pointer_ConverReg;
	
	HAL_I2C_Master_Transmit(&hi2c1, ADS1115_ADDRESS, Writebuff, 3, 100);
	
	
	HAL_I2C_Master_Receive(&hi2c1, ADS1115_ADDRESS , Result, 2, 100);



	*rawData = (int16_t) (((Result[0] << 8) & 0xFF00) | (Result[1] & 0xFF));

	return 1;
}


/**
 * @brief Switch the channel of ADS1115
 * @param channel
 */
void ADS1115_ScanChannel(uint8_t channel)
{
	switch (channel)
	{
	case 0:
		ADS1115_InitType.MUX = ADS1115_MUX_Channel_0;
		break;
	case 1:
		ADS1115_InitType.MUX = ADS1115_MUX_Channel_1;
		break;
	case 2:
		ADS1115_InitType.MUX = ADS1115_MUX_Channel_2;
		break;
	case 3:
		ADS1115_InitType.MUX = ADS1115_MUX_Channel_3;
		break;
	default:
		break;
	}

	ADS1115_Config(&ADS1115_InitType);
}


/**
 * @brief ����������ԭʼ��������ת��Ϊ��ѹ���ݣ�
 * 			����ADS1115_InitType�ṹ���а�����������Ϣ����
 * @param rawData: ��ת����ԭʼ����
 * @retval ���ؾ�������ĵ�ѹֵ
 */
float ADS1115_RawDataToVoltage(int16_t rawData)
{
	float voltage;

	switch (ADS1115_InitType.PGA)
	{
	case ADS1115_PGA_0256:
		voltage = rawData * 0.0078125;
		break;

	case ADS1115_PGA_0512:
		voltage = rawData * 0.015625;
		break;

	case ADS1115_PGA_1024:
		voltage = rawData * 0.03125;
		break;

	case ADS1115_PGA_2048:
		voltage = rawData * 0.0625;
		break;

	case ADS1115_PGA_4096:
		voltage = rawData * 0.125;
		break;

	case ADS1115_PGA_6144:
		voltage = rawData * 0.1875;
		break;

	default:
		voltage = 0;
		break;
	}

	return voltage;
}


/**
 * @brief ֱ�ӻ�ȡADS1115��ǰͨ���ĵ�ѹ����ֵ
 * @return ��ѹ����ֵ
 */
float ADS1115_GetVoltage()
{
	int16_t rawData;

	ADS1115_ReadRawData(&rawData);

	return ADS1115_RawDataToVoltage(rawData);
}


/**
 * @brief ��ȡ������ADC������ƽ����ѹֵ
 * @param num: ����ƽ��ֵ������
 * @retval ��ѹ������ƽ��ֵ
 */
float ADS1115_GetAverageVoltage(uint16_t num)
{
	int32_t sum = 0;
	int16_t rawData;

	if(num == 0)
	{
		return ADS1115_GetVoltage( );
	}

	for(uint16_t i =0; i< num;i++)
	{
		ADS1115_ReadRawData(&rawData);
		sum += rawData;
	}

	return ADS1115_RawDataToVoltage(sum/num);
}

/**
 * @brief ˢ��ADS1115ȫ��ͨ���Ĳ�������
 * 		����ADS1115ͨ���л�����Ҫ�ȴ��ϳ�ʱ�����ݲ��ܹ��ȶ���
 * 		�ڽ��ж�·���ݲɼ���ʱ���л�ͨ������ʱ�����ȴ��л���ɻ�ռ�ù����ϵͳʱ�䣬
 * 		�����Ҫ��һ����ʱ������ѯ�ɼ�ADS1115���ݣ�ÿ�βɼ���ɺ��л�����һ��ͨ��
 * 		����������ϵͳ������Ч�ʡ�
 *
 * 		���ô˺�������ˢ��ȫ�ֱ���ADS1115_RawData[4]��ֵ��
 *
 * 		Ӧ����һ����ʱ�������жϷ������������Եĵ��ô˺����������������С��200Hz
 */
void ADS1115_RefreshAllChannel()
{
	static uint8_t channel = 0;
	int16_t adcDataTemp = 0;

	//ͨ���л�ʱ�����в�ȷ�������ݶ����������Ҫ��ǰ1~2�ζ�������������
	ADS1115_ReadRawData(&adcDataTemp);
	ADS1115_ReadRawData(&adcDataTemp);

	//��ȡ���ݷ�����ȷ���򽫶���������д��ADS1115_RawData������
	if( ADS1115_ReadRawData(&adcDataTemp) !=0 )
	{
		ADS1115_RawData[channel] = adcDataTemp;
	}
	
	//ADS1115�ܹ�4��ͨ��
	channel++;
	
	if(channel>ADS1115_MAX_CHANNEL-1)
		channel = 0;

	//�����������л�����һͨ��
	ADS1115_ScanChannel(channel);
}


