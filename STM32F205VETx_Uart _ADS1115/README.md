# 功能

## ADC

### 采样

定时器1的CH3工作在生成PWM不输出的模式用于触发ADC1采样

定时器1的CH1工作在生成PWM输出的模式用于观察ADC触发信号产生时间，输出引脚为PE9。

 APB2的频率为120MHz，TIM1的预分频为0，计数值为12000

故TIM1输出 的PWM频率为10KHz,ADC的采样频率也为10kHz

采样ADC1_Filter_Num次后，DMA溢出进入HAL_ADC_ConvCpltCallback回调函数中，在回调函数中，翻转ADC_Intterupt引脚（PB12）的输出。PB12的输出频率为ADC采样频率的一半。

### 滤波

ADC的滤波和计算在double Get_ADC_Voltage(uint32_t *ADCBuf,ADC_CHANNEL CHANNEL);中，输入为需要读取的ADC的Buff数组（数据由DMA采样得到），和读取的通道。输出为滤波后的电压值单位为mV

函数对需要读取的通道和Vref的通道Buff数组进行冒泡排序，取排序后中间的ADC1_Filter_Num/5的数据进行平均。得到平均值后输出ADC1_Vref*(ADC_CHn_Sum&0xFFF)/(ADC1_Vref_Sum &0xFFF)；

其中ADC1_Vref的值在48脚以下封装在单片机内部存储，其他需要自己测试。



## 串口

使用DMA和空闲中断接收不定长的数据。

收到AT+GetADC1CH0

返回ADC1CH0 的值

收到AT+GetADC1CH1

返回ADC1CH1 的值



