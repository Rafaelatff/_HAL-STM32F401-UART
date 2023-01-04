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
