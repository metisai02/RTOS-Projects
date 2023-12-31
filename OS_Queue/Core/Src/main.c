/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t dataRecei_1[50] = {0};
uint8_t dataRecei_2[50] = {0};
typedef enum
{
    OVERFLOW = 0,
    WAIT,
    TRANSMIT
} state_t;
state_t state = TRANSMIT;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;

// osThreadId defaultTaskHandle;
// osThreadId myTask02Handle;
// osThreadId myTask03Handle;
// osThreadId myTask04Handle;
QueueHandle_t myQueue01Handle;
osSemaphoreId myBinarySem01Handle;
osSemaphoreId myBinarySem02Handle;
/* USER CODE BEGIN PV */
TaskHandle_t sender1handler;
TaskHandle_t sender2handler;
TaskHandle_t receiverhandler;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
void Receiver(void const *argument);
void Sender1(void const *argument);
void Sender2(void const *argument);
void StartTask04(void const *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* Create the semaphores(s) */
    /* definition and creation of myBinarySem01 */
    osSemaphoreDef(myBinarySem01);
    myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

    /* definition and creation of myBinarySem02 */
    osSemaphoreDef(myBinarySem02);
    myBinarySem02Handle = osSemaphoreCreate(osSemaphore(myBinarySem02), 1);

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the queue(s) */
    /* definition and creation of myQueue01 */
    //  osMessageQDef(myQueue01, 10, uint16_t);
    //  myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), NULL);

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */
    //  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
    //  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
    //
    //  /* definition and creation of myTask02 */
    //  osThreadDef(myTask02, Sender1, osPriorityNormal, 0, 128);
    //  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);
    //
    //  /* definition and creation of myTask03 */
    //  osThreadDef(myTask03, Sender2, osPriorityNormal, 0, 128);
    //  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

    /* definition and creation of myTask04 */
    //  osThreadDef(myTask04, StartTask04, osPriorityBelowNormal, 0, 128);
    //  myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    xTaskCreate((void *)Sender1, "sender1", 128, NULL, osPriorityNormal, &sender1handler);
    xTaskCreate((void *)Sender2, "sender2", 128, NULL, osPriorityNormal, &sender1handler);
    xTaskCreate((void *)Receiver, "Receiver", 128, NULL, osPriorityBelowNormal, &receiverhandler);

    myQueue01Handle = xQueueCreate(30, sizeof(dataRecei_1));

    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, dataRecei_1, sizeof(dataRecei_1));
    __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, dataRecei_2, sizeof(dataRecei_2));
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
    xSemaphoreTake(myBinarySem01Handle, 1000);
    xSemaphoreTake(myBinarySem02Handle, 1000);
    /* USER CODE END RTOS_THREADS */

    /* Start scheduler */
    osKernelStart();
    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
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

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */
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
    huart3.Init.BaudRate = 1200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
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
    /* DMA1_Channel5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    /* DMA1_Channel6_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
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
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    /*Configure GPIO pin : PC13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    if (huart->Instance == USART1)
        xSemaphoreGiveFromISR(myBinarySem01Handle, &xHigherPriorityTaskWoken);
    else if (huart->Instance == USART2)
        xSemaphoreGiveFromISR(myBinarySem02Handle, &xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void Receiver(void const *argument)
{
    /* USER CODE BEGIN 5 */
    uint32_t timeStart;
    uint8_t count;
    uint8_t data[50];
    /* Infinite loop */
    for (;;)
    {
        switch (state)
        {
        case OVERFLOW: // Queue get overflow
            timeStart = HAL_GetTick();
            vTaskSuspend(sender1handler);
            vTaskSuspend(sender2handler);
            state = WAIT;
            break;
        case WAIT: // wait 20s to send data out
            if (HAL_GetTick() - timeStart > 1000)
            {
                HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
                timeStart = HAL_GetTick();
                count++;
                if (count == 20)
                    state = TRANSMIT;
            }
            break;
        case TRANSMIT: // Runing
            count = 0;
            vTaskResume(sender1handler);
            vTaskResume(sender2handler);
            HAL_UARTEx_ReceiveToIdle_DMA(&huart1, dataRecei_1, sizeof(dataRecei_1));
            __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

            HAL_UARTEx_ReceiveToIdle_DMA(&huart2, dataRecei_2, sizeof(dataRecei_2));
            __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
            break;
        }

        if (xQueueReceive(myQueue01Handle, data, 0) == pdTRUE)
            HAL_UART_Transmit(&huart3, data, 50, 1000);

        osDelay(1);
    }
    /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
 * @brief The function implements the Sender1 Task.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask02 */
void Sender1(void const *argument)
{
    /* USER CODE BEGIN StartTask02 */
    /* Infinite loop */
    for (;;)
    {
        if (xSemaphoreTake(myBinarySem01Handle, 2000) == pdTRUE)
        {
            if (uxQueueSpacesAvailable(myQueue01Handle) != 0)
            {
                __HAL_DMA_DISABLE(&hdma_usart1_rx);
                xQueueSend(myQueue01Handle, dataRecei_1, 0);
                HAL_UARTEx_ReceiveToIdle_DMA(&huart1, dataRecei_1, sizeof(dataRecei_1));
                __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
            }
            else
            {
                __HAL_DMA_DISABLE(&hdma_usart1_rx);
                state = OVERFLOW;
            }
        }
        else
        {
        }

        osDelay(10);
    }
    /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
 * @brief The function implements the Sender2 Task.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask03 */
void Sender2(void const *argument)
{
    /* USER CODE BEGIN StartTask03 */
    /* Infinite loop */
    for (;;)
    {
        if (xSemaphoreTake(myBinarySem02Handle, 2000) == pdTRUE)
        {
            if (uxQueueSpacesAvailable(myQueue01Handle) != 0)
            {
                __HAL_DMA_DISABLE(&hdma_usart2_rx);
                xQueueSend(myQueue01Handle, dataRecei_2, 0);
                HAL_UARTEx_ReceiveToIdle_DMA(&huart2, dataRecei_2, sizeof(dataRecei_2));
                __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
            }
            else
            {
                __HAL_DMA_DISABLE(&hdma_usart2_rx);
                state = OVERFLOW;
            }
        }
        else
        {
        }
        osDelay(10);
    }
    /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
 * @brief Function implementing the myTask04 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask04 */
// void StartTask04(void const * argument)
//{
//   /* USER CODE BEGIN StartTask04 */
//	uint8_t data[50];
//   /* Infinite loop */
//   for(;;)
//   {
//	if(xQueueReceive(myQueue01Handle, data, 2000) == pdTRUE)
//	{
//		HAL_UART_Transmit(&huart3, data, 50, 1000);
//	}
//	osDelay(10);
//   }
//   /* USER CODE END StartTask04 */
// }

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
