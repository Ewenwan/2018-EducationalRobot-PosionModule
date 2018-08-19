## 基于Arduino超声波HCSR04模块测距

参考链接：[超声波类库应用](https://www.arduino.cn/thread-1003-1-1.html)

### 1. HCSR04模块

![超声波模块](C:/Users/liuyang/Documents/GitHub/2018-EducationalRobot-PosionModule/img/hcsr04.jpg)  

HC-SR04超声波测距模块可提供**2cm-400cm**的非接触式距离感测功能，测距精度可达高到**3mm**；模块包括**超声波发射器、接收器与控制电路**。像智能小车的测距以及转向，或是一些项目中，常常会用到。智能小车测距可以及时发现前方的障碍物，使智能小车可以及时转向，避开障碍物。

 `Vcc-GND 5V电源输入；Trig：触发信号输入；Echo:回响信号输出`

### 2. 工作原理

- 采用IO口TRIG触发测距，给最少10us的高电平信号。
- 模块自动发送8个40khz的方波，自动检测是否有信号返回。
- 有信号返回，通过IO口ECHO输出一个高电平，**高电平持续的时间就是超声波从发射到返回的时间**。
- $测试距离s=\frac{高电平时间t\times 声速（v=340m/s)}{2}$

![](C:/Users/liuyang/Documents/GitHub/2018-EducationalRobot-PosionModule/img/hcrsmode.png)

以上时序图表明你只需要提供一个 10uS以上脉冲触发信号，该模块内部将 发出8个 40kHz周期电平并检测回波。一旦检测到有回波信号则输出回响信号。 回响信号的脉冲宽度与所测的距离成正比。 由此通过发射信号到收到的回响信号时间间隔可以计算得到距离。

~~为了防止测试信号对回响信号的影响，建议测量周期$\ge60ms$ ,即频率小于16HZ。~~这句话存疑，根据最大测量距离3m计算，只需要保证周期>20m即可，实际测量最大距离大概为2m-3m。所以实际最大频率可以达到50Hz。

**由于回响电平持续时间为$us$级，对以上公式可以化简：$s=\frac{us\times 340\times100}{10^6\times 2} cm=0.017\times us\approx \frac{us}{58} cm(或者\frac{us}{59}cm)$** 

### 3. 基于Arduino的测距实现

#### 3.1 硬件连接

自定义数字输出端口，示例程序中`Vcc-GND:5V;  Trig:D2;  Echo:D3` 

#### 3.2 软件设计

- 利用了库函数

  ```c
  #include "SR04.h"    //库函数头文件
  #define TRIG_PIN 2
  #define ECHO_PIN 3
  SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);  //库函数声明
  long a;
  void setup() {
  Serial.begin(9600);   //设置I2C通信波特率
  Serial.println("Example written by Coloz From Arduino.CN");
  delay(1000);}
  void loop() {
  a=sr04.Distance();   //直接调用库函数
  Serial.print(a);
  Serial.println("cm");
  delay(1000);}
  ```

- 不用库直接驱动

  ```c
  const int TrigPin = 2; 
  const int EchoPin = 3; 
  float cm; 
  void setup() 
  { 
  Serial.begin(9600); //硬件库设置波特率为9600
  pinMode(TrigPin, OUTPUT); //设置引脚模式，Trig为输出，Echo为输入
  pinMode(EchoPin, INPUT); 
  } 
  void loop() 
  { 
  //发一个10ms的高脉冲去触发TrigPin 
  digitalWrite(TrigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(TrigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(TrigPin, LOW); 
   
  cm = pulseIn(EchoPin, HIGH) / 58.0; //算成厘米 
  cm = (int(cm * 100.0)) / 100.0; //保留两位小数 
  Serial.print(cm); 
  Serial.print("cm"); 
  Serial.println(); 
  delay(1000); 
  }
  ```
