# _HAL-STM32F401-UART
This project uses STM32CubeIDE and it's a program created to practice my C habilities during the course 'Mastering Microcontroller: Timers, PWM, CAN, Low Power (MCU2)'. I am using a NUCLEO-F401RE board.

**HAL FW version -> 1.27.1.**

## Creating the project (MX Cube)

Inside the STM32CubeIDE v1.10.1 (and with an open Workspace for this course), I go to *File -> New project -> STM32 Project -> Board Selector*. Then, I add the board model and click Next.

![image](https://user-images.githubusercontent.com/58916022/210432085-ca5a464b-476e-4b72-bf6e-c8bd9d2f3b63.png)

And then I add the name of the project and confirm its location. I use the following configurations:

![image](https://user-images.githubusercontent.com/58916022/210432472-97ca1c66-6103-4148-bcc2-3097798b7c11.png)

Then I confirm to initialize the peripherals in their default mode.

![image](https://user-images.githubusercontent.com/58916022/210432512-3777c7b6-1e06-4a5a-8d3f-c4f61857985e.png)

And I confirm the option to open the Device Configuration Tool pespective.

![image](https://user-images.githubusercontent.com/58916022/210432563-6469041f-c312-41ac-96df-5ec084995fb9.png)

Now Inside the MX Cube, we have (note that UART2 were configured already).

![image](https://user-images.githubusercontent.com/58916022/210433545-20da0f57-f137-47ab-b8a6-3480a49697e3.png)

Checking the schematic (19168 - sch c4.pdf), we can confirm that in the NUCLEO board the UART2 pins are PA2 (as TX) and PA3 (as RX).

![image](https://user-images.githubusercontent.com/58916022/210433930-1bca937c-81bd-46a1-a7f8-f3637f403371.png)

Also, SB61, SB62 and SB63 are disconnected. 

The schematic/Manual/MX says the the HSE comes from External XTAL source. The connection is made by PH0 and PH1 pins, but in schematic they are N/A (DNP). So, HSI (High Speed Internal RC oscillator will be used).

![image](https://user-images.githubusercontent.com/58916022/210435312-c4015301-5b59-4ee4-b3d0-5e29c41bd65d.png)

We have also some peripherals such the LD2 (green LED) and B1 (blue push button).

For now we have the following clock configuration: 

![image](https://user-images.githubusercontent.com/58916022/210435532-e9759813-dbe9-4f69-9fbf-82488eb490ea.png)

In the Project Manager tab we can configure important things. Since we are creating a project already inside the STM32CubeIDE, the 'Toolchain/IDE' (found in the first part of the configuration, called *Project*) is already selected and cannot be changed. An important thing to consider (that also can be part of the project name or added to the header file of user code/user application) is the HAL version that the code is being generated. New versions are delivered from STMiconeletronics, and older versions can have problems to work with new versions of the code. In our case, we are using the version 1.27.1. That information is presented at the field 'Firmware Package Name and Version'.

![image](https://user-images.githubusercontent.com/58916022/210436547-1ec08c32-a5bf-4016-bd65-46ca4c0843ac.png)

In the Project Manager tab, but now in *Code Generator*, we add the following configuration. As a good practice, I ticked the field 'Generate peripheral initialization as a pair of .c/.h file per peripheral'. This way, the peripheral codes won't be in the main.c file.

![image](https://user-images.githubusercontent.com/58916022/210437177-51353213-8f72-4de0-8f27-ef0dfe246209.png)

In the Project Manager tab, but now in *Advanced Settings* I just tick the option to 'Do not generate function call'. -> **S told me that he leaves unticked only the TIM17 unticked, because he uses it to count time. I need to do a further investigation.** also **Last timer always to count time. For this uC TIM11 is the last timer. I decided to activate just to check**.

![image](https://user-images.githubusercontent.com/58916022/210438248-4fef3a6c-5872-48ba-8bba-dd825c18db25.png)

After generating the code I had:

![image](https://user-images.githubusercontent.com/58916022/210438463-a753dde8-40fa-4af7-8371-5426903a9f53.png)

## Understanding a little bit of the structure

## Understanding a little bit of the codes

### HAL_MspInit()

HAL_MspInit() (@file stm32f4xx_hal_msp.c) must initialize the low level processor specifics. The API comes with those information:

![image](https://user-images.githubusercontent.com/58916022/210556222-e88e8989-25dc-4ce9-816d-c3d8c6a9c37c.png)

On the course, the teacher told that HAL_MspInit() has to (all information found on Generic User Guide of Cortex™-M4 Devices document, DUI 0553B): 

- 1. Set up the priority grouping of the arm cortex mx processor

![image](https://user-images.githubusercontent.com/58916022/210556965-f39d7f34-f81d-4de1-9c38-ee29006c00be.png)

The function is found on stm32f4xx_hal_cortex.c.

![image](https://user-images.githubusercontent.com/58916022/210557232-622ee0ec-69ff-4e9e-b422-4756ff5e4fb1.png)

- 2. Enable the required system exceptions of the arm mx processor

![image](https://user-images.githubusercontent.com/58916022/210555514-92777793-a9d9-4be6-8a93-55325025f09c.png)

```c
// Setting example
  SCB->SHCSR |= 0x7 << 16; //System Control Block, set bits 16, 17 and 18 
```

- 3. Configure the priority for the system exceptions

There is no need of setting priority for systick once that HAL_Init() (@file main.c) -> HAL_InitTick(TICK_INT_PRIORITY) (@file stm32f4xx_hal.c)

```c
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0); // for every system exception
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0); 	 // I set the priority
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0); 	 // macros are in stm32f410xx.h header file
  ```

### MX_USART2_UART_Init(void) - [High-Level Peripheral Init]

MX_USART2_UART_Init(void) (@filw usart.c) must be initialize inside the main.c code. Since the time of its initialization may depend on user application, the user must call its function. This API does the **high-level initialization**.

![image](https://user-images.githubusercontent.com/58916022/210567018-fa9679ad-e63b-4eab-bb5c-7808a5412e4e.png)

Note that I added this code line between one USER CODE BEGIN and a USER CODE END. The reason is because if I need to change anything on *.ioc* file and regenerate the codes using STM32CubeMX, the USER CODES won't be lost! 

Since I am using an API that is inside a different source file, I also need to include its header file.

![image](https://user-images.githubusercontent.com/58916022/210569040-0e4a6303-caf8-40a6-9356-ffdc84cad5cf.png)

MX_USART2_UART_Init() is written already by the settings made in STM32CubeMX

![image](https://user-images.githubusercontent.com/58916022/210566517-20e63647-b5b9-4362-9ef0-4780fc23e62b.png)

### HAL_UART_MspInit(UART_HandleTypeDef \*huart) - [Low-Level Peripheral Init]

At the end of the  MX_USART2_UART_Init() (@file usart.c) API, the HAL_UART_Init() (@file stm32fxx_hal_uart.c) is called. Inside the HAL_UART_Init(), the  HAL_UART_MspInit(huart) is called. The function is definied as __weak, so it must be implemented by the user on stm32f4xx_hal_msp.c file.

![image](https://user-images.githubusercontent.com/58916022/210579277-64f6f1b6-dac8-481e-a56c-2f5153dc5985.png)

We need to enable the peripheral clock. 

Then we need to configure the peripheral pins of UART:
- UART_TX: PA2;
- UART_RX: PA3;
- UART_RTS: not applied;
- UART_CTS: not applied.

For the alternate function, we need to configure PA2 and PA3 as AF07 according to the microcontroller datasheet.

![image](https://user-images.githubusercontent.com/58916022/210582326-81bfafac-62b2-440d-8654-d05c0c4c8c5a.png)

Note: The available macros for GPIO are on file stm32f4xx_hal_gpio_ex.h, at the section of selected microcontroller.

![image](https://user-images.githubusercontent.com/58916022/210602254-f7ced2d3-4314-49f2-a07c-60cf7e6f4a94.png)

Inside stm32f4xx_hal_cortex.c we can find the HAL_NVIC_EnableIRQ. The available IRQn numbers are on stm32f401xe.h file. In our case, USART2_IRQn.

```c
/* USER CODE BEGIN 1 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart){
	// Low level inits
	GPIO_InitTypeDef gpio_uart;

	// 1. Enable the clock for the USART2 peripheral
	__HAL_RCC_USART2_CLK_ENABLE();

	// 2. Do the pin muxing configurations
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AD_PP;
	gpio_uart.Pull = PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2; //UART2_TX
	HAL_GPIO_Init(GPIOA,&gpio_uart); // GPIO doesn't have handle struct

	gpio_uart.Pin = GPIO_PIN_3;	//UART2_RX
	HAL_GPIO_Init(GPIOA,&gpio_uart); // GPIO doesn't have handle struct

	// 3. Enable the IRQn
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}
/* USER CODE END 1 */
```
Well, in reallity I don't need to add this function, since in the function HAL_UART_Init() (@file stm32f4xx_hal_uart.c) already call another function with name HAL_UART_MspInit() (@file usart.c). This function does all the low-level initialization:

![image](https://user-images.githubusercontent.com/58916022/210612413-97e0ab02-c532-4f3a-98e4-b99f3bc599e8.png)

Important note: **This API already call for the clock enables of GPIOA and USART2**.

## Peripheral Data Handling API Flavors

Basically there are 3 ways that STM32Cube can do:

- Non Interrupt based (polling or also called blocking mode);
- Interrupt based;
- **DMA base** (uses DMA and interrupts).

## Serial terminal

I used the [Tera Term](https://ttssh2.osdn.jp/index.html.en) terminal. Configured it to work with same serial configuration from exercise.

![image](https://user-images.githubusercontent.com/58916022/210619175-446630c2-abfe-4acc-b7cf-caf9b9e456fe.png)

It wasn't showing the whole text, that is because of the systick interrupt of 1ms.

![image](https://user-images.githubusercontent.com/58916022/210620251-41a3ad71-7671-4023-b333-04a4e1eeb27a.png)

So, let's implement the systick handler interrupt. I leaved the same way the instructor of the course did and still didn't work.

![image](https://user-images.githubusercontent.com/58916022/210622318-22492f17-f5e3-4128-91d7-254de67d825b.png)

Basically on Q&a:

FastBit Embedded: "if system tick interrupt and UART interrupt happens at the same time according to the priority thy will run on the cpu." also "Lets say you have to wait for some event. what if the event never occurs ? how long should the api wait before returning timeout error ? how do you implement such logic ? you need a timing base. you need some background ticking and based on that timeout will be decided. and systick timer interrupt maintains that global tick value.".

Well, to fix, just use the right function to callculate the string size (strlen) and also add the #include <string.h>.

![image](https://user-images.githubusercontent.com/58916022/210628736-a92e4aa5-1e51-45d5-b227-6fdb26fccaf1.png)


