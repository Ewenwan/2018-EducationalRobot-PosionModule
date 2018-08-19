## Arduino nano系列学习笔记

### 1. 软件安装及教程资源链接

[Windows软件下载](https://coding.net/u/coloz/p/arduino-installer/git/raw/master/1.8.5/arduino-1.8.5-windows.exe) 

[Mac os软件下载](https://coding.net/u/coloz/p/arduino-installer/git/raw/master/1.8.5/arduino-1.8.5-macosx.zip) 

[驱动安装方法](https://www.arduino.cn/thread-1008-1-1.html)

[Arduino教程汇总](https://www.arduino.cn/thread-1066-1-1.html)

[Arduino官网](http://www.arduino.cc/)

![引脚图](..\img\ardunio_nano.jpg) 

### 2. Arduino教程

#### 2.1 Blink——Arduino的Hello World！ 

> Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO. it is attached to digital pin 13, on MKR1000 on pin 6. **LED_BUILTIN** is set to the correct LED pin independent of which board is used.

```c
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
```

- `setup()函数用于初始化，只在开始执行一次；loop()函数循环执行，不需要main函数。`

#### 2.2 Arduino语言

1. **Arduino使用C/C++编写程序**，虽然C++兼容C语言，但这是两种语言，C语言是一种面向过程的编程语言，C++是一种面向对象的编程语言。早期的Arduino核心库使用C语言编写，后来引进了面向对象的思想，目前最新的Arduino核心库采用C与C++混合编写而成。  

2. 传统开发方式中，你需要厘清每个**寄存器**的意义及之间的关系，然后通过配置多个寄存器来达到目的。  而在Arduino中，使用了清楚明了的**API**替代繁杂的寄存器配置过程，如以下代码： `pinMode(13,OUTPUT);digitalwrite(13,HIGH)`，`pinMode(13,OUTPUT)`即是设置引脚的模式，这里设定了13脚为输出模式；而`digitalWrite(13,HIGH) `是让13脚输出高电平数字信号。 

3. 如果你使用过C/C++语言，你会发现Arduino的程序结构与传统的C/C++结构的不同——Arduino程序中没有main函数。  其实并不是Arduino没有main函数，而是main函数的定义隐藏在了Arduino的核心库文件中。Arduino开发一般不直接操作main函数，而是使用`setup()`和`loop()`这个两个函 数。

   Arduino控制器通电或复位后，**即会开始执行`setup() `函数中的程序，该部分只会执行一次**。 通常我们会在`setup() `函数中完成Arduino的初始化设置，如**配置I/O口状态，初始化串口等操作。** 

   在`setup()` 函数中的程序执行完后，Arduino会接着执行`loop()` 函数中的程序。而`loop()`函数是一个死循环，其中的程序会不断的重复运行。 通常我们会在`loop() `函数中完成程序的主要功能，如**驱动各种模块，采集数据**等。

4. C/C++编程基础（补充）

   ```c
   常量定义：const 类型 常量名 = 常量值
   布尔型变量，即boolean。它的值只有两个：false（假）和true（真）。boolean会占用1个字节的内存空间。
   ```

#### 2.3 常用电子原件和拓展模块

- 面包板：进行各种电子实验所设计的工具。众多电子元器件都可根据需要，随意插入或拔出，免去了焊接，节省了电路的组装时间 。

  ![](C:\Users\liuyang\Documents\GitHub\2018-EducationalRobot-PosionModule\img\breadboard.png)

###  3. Arduino编程基础

#### 3.1 按键实验

> 关于硬件接口，所有接口均可输出数字量，包括模拟量输出接口A；D3-D13即对应pin3-pin13.

- 按键实验描述：通过按键控制LED灯点亮与熄灭，电路设计可以分为下拉和输出上拉。使用了`digitalRead(pin)`

- 硬件连接

  ![](..\img\arled.png)

  D2设置为输入上拉模式，内部连接到Vcc并含有上拉电阻（10k，内部），当S1打开时，D2输入为高电平，S1闭合时，D2输入为低电平。

- 程序设计

  `setup()`函数初始化设置引脚输出模式，`pinMode(pin,MODE)`设置D13为`OUTPUT`，2为`INPUT_PULLUP`

  ```c
  int buttonPin = 2;
  int ledPin = 13;
  boolean ledState=false;  // 记录LED状态
  boolean buttonState=true;  // 记录按键状态
   
  void setup() 
  {
  // 初始化I/O口
    pinMode(buttonPin, INPUT_PULLUP);   //设置模式为输入上拉，悬空为高，闭合为低
    pinMode(ledPin,OUTPUT);
  }
   
  void loop() 
  {
  // 等待按键按下
  while(digitalRead(buttonPin)==HIGH){}  //当按键不闭合buttonPin为高，不执行下一段
   
    // 当按键按下时，点亮或熄灭LED
    if(ledState==true)
    {
      digitalWrite(ledPin,LOW);
      ledState=!ledState;
    }
    else
    {
      digitalWrite(ledPin,HIGH);
      ledState=!ledState;
    }
    delay(500);//延时500ms,程序运行很快，一次按键可能执行了多次loop()
  }
  ```

#### 3.2 模拟信号实验

> Arduino **模拟输入功能**有10位精度，即**可以将0～5V的电压信号转换为0～1023的整数形式表示**。 模拟输入功能需要使用analogRead() 函数。 
>
> `int value = analogRead(pin)` 
>
> 与模拟输入功能对应的是**模拟输出功能**，我们使用`analogWrite() `函数实现这个功能。但该函数并不是输出真正意义上的模拟值，而是以一种特殊的方式来达到输出近似模拟值的效果，这种方式叫做脉冲宽度调制（PWM，Pulse Width Modulation）。 在Arduino UNO中，提供PWM功能的引脚为**3、5、6、9、10、11**。 
>
> `analogWrite(pin,value) `
>
> 参数pin是指定要输出PWM波的引脚，参数value指定是PWM的脉冲宽度，范围为**0～255**。 **在analogWrite() 和analogRead() 函数内部，已经完成了引脚的初始化，因此不用在Setup() 函数中进行初始化操作。**       

