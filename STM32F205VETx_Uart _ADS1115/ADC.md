![image-20220421143239000](C:\Users\hao\AppData\Roaming\Typora\typora-user-images\image-20220421143239000.png)

ADC主要功能

12位、10位、8位或6位可配置分辨率

转换结束、注入转换结束、模拟看门狗或溢出事件的情况下产生中断

单次和连续转换模式‘

自动从通道0到通道n的扫描模式数据一致性与内置数据一致性

通道可编程采样时间

具有配置极性的外部触发器选项，用于常规转换和注入转换

间断模式

双/三模式（在配备2个或更多ADC的设备上）

双/三ADC模式下的可配置DMA数据存储

双/三交织模式下转换之间的可配置延迟


ADC转换时间：0.5 us，APB2频率为60 MHZ

ADC电源要求：全速时为2.4 V至3.6 V，慢速时为1.8 V

ADC输入范围：VREF_  到 VREF+

在常规信道转换期间生成DMA请求

图28显示了ADC的框图

VREF-如果可用（取决于包装），必须与VSSA连接

![image-20220421144914003](C:\Users\hao\AppData\Roaming\Typora\typora-user-images\image-20220421144914003.png)