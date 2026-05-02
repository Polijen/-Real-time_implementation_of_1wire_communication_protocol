/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwow/lwow.h"
#include "lwow/devices/lwow_device_ds18x20.h"
#include <stdio.h>
#include <stdarg.h>
#include "usbd_cdc_if.h"
#include "FreeRTOS.h"
#include "semphr.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */
lwow_t ow_handle;

extern const lwow_ll_drv_t lwow_ll_drv_stm32_hal;

SemaphoreHandle_t smphrHandle;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2S3_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART3_UART_Init(void);
void StartDefaultTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
# define LOG_BUFFER_SIZE 128
 uint8_t buf[LOG_BUFFER_SIZE];
void vPrintSerial ( const char* format , ...){
	va_list ap;
	va_start(ap , format );
	size_t len = vsnprintf(( char *) buf , sizeof (buf) , format , ap );
	va_end(ap );
	if ( len > sizeof(buf) - 1)len = sizeof(buf) - 1;
	CDC_Transmit_FS(buf , len );
/* Send string to serial interface */
	}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2S3_Init();
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  //lwow_t ow_handle;          // structura centrala de gestionare a magistralei

  //lwow_init(&ow_handle, &huart3); //leaga bibliotecele LwOW cu perifericul USART3

  // 1. Creăm semaforul binar necesar pentru blocarea/deblocarea task-ului
  smphrHandle = osSemaphoreNew(1, 0, NULL);

  // 2. Inițializăm LwOW pasând și driver-ul hardware STM32 (lwow_ll_drv_stm32)
  lwow_init(&ow_handle, &lwow_ll_drv_stm32_hal, &huart3);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //uint32_t cnt = 0;
  //float temperatura_celsius = 0;
  //char buffer_mesaj[64]; // Am adăugat  pentru a aloca memorie pentru un text lung

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //vPrintSerial("Iteration:%d\n", ++cnt);
	  //HAL_Delay(1000) ;
	  /*
      if (lwow_ds18x20_start(&ow_handle, NULL) == 1) {
          osDelay(750);

          if (lwow_ds18x20_read_raw(&ow_handle, NULL, &temperatura_celsius) == 1) {
              int lungime = sprintf(buffer_mesaj, "Temperatura: %.2f C\r\n", temperatura_celsius);
              CDC_Transmit_FS((uint8_t*)buffer_mesaj, lungime);

          }
      }
      osDelay(1000); */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S3_Init(void)
{

  /* USER CODE BEGIN I2S3_Init 0 */

  /* USER CODE END I2S3_Init 0 */

  /* USER CODE BEGIN I2S3_Init 1 */

  /* USER CODE END I2S3_Init 1 */
  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_96K;
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S3_Init 2 */

  /* USER CODE END I2S3_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* Rutina apelată de subsistemul abstract de operare HAL pe fond de hardware cand fluxul de la memorie spre periferic a atins numărătoarea zerotronică (0) */

/* Required external linkage declarations in main.c */
/*
extern void lwow_ll_usart_tx_isr(UART_HandleTypeDef *huart);
extern void lwow_ll_usart_rx_isr(UART_HandleTypeDef *huart);
*/
/* Implementation of the HAL callbacks */
/*
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) { // Ensure the callback targets the correct peripheral
        lwow_ll_usart_tx_isr(huart);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        lwow_ll_usart_rx_isr(huart);
    }
}
*/
/*
// Aceste funcții sunt apelate automat de HAL când DMA-ul își termină treaba
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        // Apelează funcția de notificare TX din LwOW LL (verifică denumirea exactă în lwow_ll_stm32_hal.c)
        // Exemplu generic (depinde de versiunea LwOW folosită):
        lwow_ll_usart_tx_isr(huart);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        // Apelează funcția de notificare RX din LwOW LL
        lwow_ll_usart_rx_isr(huart);
    }
}
*/
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 5 */

  lwow_rom_t target_sensor;
  /* Buffer for formatting Virtual COM port transmissions */
     char transmission_buffer[90];
     osDelay(pdMS_TO_TICKS(3000));
     /* 1. Hardware Driver Initialization - Correctly passing driver by reference */
     while (lwow_init(&ow_handle, &lwow_ll_drv_stm32_hal, &huart3)!= lwowOK) {
             char *err_init = "Fatal: LwOW UART Initialization Failed. Retrying...\r\n";
             while (CDC_Transmit_FS((uint8_t*)err_init, strlen(err_init)) == USBD_BUSY) {
                 osDelay(pdMS_TO_TICKS(1));
             }
             osDelay(pdMS_TO_TICKS(1000)); /* Retry every 1 second instead of suspending */
     }

     /* 2. Topology Discovery - Utilizing the modernized stateful search API */
     lwow_search_reset(&ow_handle);
         while (lwow_search(&ow_handle, &target_sensor)!= lwowOK) {
             char *err_search = "Error: No DS18B20 Detected. Check wiring/pull-up. Retrying...\r\n";
             while (CDC_Transmit_FS((uint8_t*)err_search, strlen(err_search)) == USBD_BUSY) {
                 osDelay(pdMS_TO_TICKS(1));
             }
             osDelay(pdMS_TO_TICKS(1000)); /* Retry every 1 second instead of suspending */
             lwow_search_reset(&ow_handle);
         }
     //t<rget sensor found
     char *msg_found = "Success: DS18B20 Detected! Starting telemetry...\r\n";
             while (CDC_Transmit_FS((uint8_t*)msg_found, strlen(msg_found)) == USBD_BUSY) {
                 osDelay(pdMS_TO_TICKS(1));
             }

     /* 3. Thread Telemetry Variables */
     float current_temp = 0;
     uint32_t iteration_counter = 0;

     /* 4. Deterministic, Non-Blocking Operational Loop */
     for(;;)
         {
             /* Broadcast the Convert T Command to the sensor network */
             if (lwow_ds18x20_start(&ow_handle, NULL) == 1) {

                 /* Yield the execution context for the duration of the ADC processing (750ms) */
                 osDelay(pdMS_TO_TICKS(750));

                 /* Secure thread-safe retrieval of the converted memory scratchpad */
                 if (lwow_ds18x20_read(&ow_handle, &target_sensor, &current_temp) == 1) {

                     iteration_counter++;

                     /* Serialize output including iteration counter */
                     int len = snprintf(transmission_buffer, sizeof(transmission_buffer),
                                        "[%lu] DS18B20 Temperature: %.2f C\r\n",
                                        iteration_counter, current_temp);

                     /* Dispatch the USB CDC data transfer safely handling USBD_BUSY */
                     while (CDC_Transmit_FS((uint8_t*)transmission_buffer, len) == USBD_BUSY) {
                         osDelay(pdMS_TO_TICKS(1));
                     }

                 } else {
                     char *err_read = "Warning: DS18B20 Read Failure or CRC Mismatch.\r\n";
                     while (CDC_Transmit_FS((uint8_t*)err_read, strlen(err_read)) == USBD_BUSY) {
                         osDelay(pdMS_TO_TICKS(1));
                     }
                 }
             } else {
                 /* Conversion start command failed; attempt a short recovery delay */
                 osDelay(pdMS_TO_TICKS(100));
             }

             /* Regulate the overall sampling rate loop */
             osDelay(pdMS_TO_TICKS(250));
         }


  /*
  // Instanțiem structurile de bază pentru 1-Wire
    lwow_t ow;
    lwow_rom_t rom_id; // Adresa unică pe 64-biți a senzorului
    lwow_rom_t target_rom_id;
    uint8_t devices_found = 0;
    float temperature = 0.0f;

    // 1. Inițializăm biblioteca LwOW (îi pasăm structura și un pointer către driver-ul low-level)
    // Parametrii pot varia ușor în funcție de versiunea LwOW instalată
    //lwow_init(&ow, lwow_ll_drv_stm32_hal, &huart3);
    if (lwow_init(&ow, &lwow_ll_drv_stm32_hal, &huart3)!= lwowOK) {
        // Implement robust fault handling for initialization failure
    }
    lwow_search_reset(&ow);
    // 2. Căutăm primul senzor de pe magistrală
    // Dacă returnează 1, a găsit cu succes un senzor și i-a salvat adresa în `rom_id`
    if (lwow_search(&ow, &target_rom_id) == lwowOK){
        devices_found = 1;
        vPrintSerial("Senzor DS18B20 gasit!\r\n");
    } else {
        vPrintSerial("Eroare: Niciun senzor detectat. Verifica rezistenta de pull-up!\r\n");
    }

    uint32_t cnt = 0;

    // Infinite loop
    for(;;)
    {
      cnt++;

      if (devices_found) {
          // 3. Trimitem comanda către senzor să înceapă conversia temperaturii
          // Transmisia se face complet în background via DMA
          if (lwow_ds18x20_start(&ow, &rom_id) == 1) {

              osDelay(750);
          	  // 4. Așteptăm ca DS18B20 să termine măsurătoarea (Max 750 ms pt 12-bit)
          	  // Aici CPU-ul se duce să rezolve alte task-uri (ex: animații ecran, pachete rețea)

              // 5. Citim valoarea (Datele vin din senzor tot via DMA)
              if (lwow_ds18x20_read(&ow, &rom_id, &temperature) == 1) {
                  vPrintSerial("Iteratia: %lu | Temp: %.2f C\r\n", cnt, temperature);
              } else {
                  vPrintSerial("Eroare la citirea memoriei Scratchpad!\r\n");
              }
          } else {
              vPrintSerial("Senzorul nu a raspuns la comanda de conversie.\r\n");
          }
      } else {
          vPrintSerial("Iteratia: %lu | Astept conectarea senzorului...\r\n", cnt);
      }

      // Delay general al buclei pentru cadență (ex. 1 citire la fiecare ~2 secunde)
      osDelay(1000);
    }

  uint32_t cnt = 0;
  float temperatura_celsius = 0;
  char buffer_mesaj[64]; // Am adăugat  pentru a aloca memorie pentru un text lung

  for(;;){
  	  vPrintSerial("Iteration:%d\n", ++cnt);
  	  HAL_Delay(1000) ;

      if (lwow_ds18x20_start(&ow_handle, NULL) == 1) {
          osDelay(750);

          if (lwow_ds18x20_read_raw(&ow_handle, NULL, &temperatura_celsius) == 1) {
              int lungime = sprintf(buffer_mesaj, "Temperatura: %.2f C\r\n", temperatura_celsius);
              CDC_Transmit_FS((uint8_t*)buffer_mesaj, lungime);

          }
      }
      osDelay(1000);*/



  /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
