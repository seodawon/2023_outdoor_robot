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
#include "crc.h"
#include "dma.h"
#include "rng.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "base_serial.h"
#include "ringbuffer.h"
#include "Serial.h"
#include "roboClaw.h"
#include "gps.h"
#include "bluetooth.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//앱인벤터에서 보낸 데이터를 받는 변수->bluetooth.c의 변수 값을 그대로 가져와서 써야하기 때문에 extern 선언
extern uint8_t rx3_data;
//위의 것과 같은 맥락
extern GPS_t GPS;
//앱인벤터에서 보낸 값 넣는 변수 형식 => 각도,거리#
extern float GPS_angle; //앱인벤터로 실시간 각도를 받는 변수
extern float GPS_distance;
float APP_angle; // 앱인벤터로 받은 변수를 그 각도만큼 돌리고 난 후에 다시 각도를 받기위한 변수
float APP_distance; // // 앱인벤터로 받은 변수를 그 거리만큼 이동한 후 다음 거를 받기위한 변수
extern float L_angle;
extern uint8_t point;
extern RoboClaw_HandleTypeDef hroboClaw1;
extern RoboClaw_HandleTypeDef hroboClaw2;
uint8_t check1 =0; //맨 처음 각도 값을 받기 위한 변수 셋팅 ;

uint8_t start=0;
uint8_t enc;
uint8_t encV[10];
uint32_t encM1 = 0;
uint8_t stop=0;
uint8_t count1 = 0;
uint8_t count2 = 0;
uint8_t stopM1[5] = {0};
uint8_t stopM2[5] = {0};
extern float Lidar_data;
extern uint16_t srf08_data;
extern uint8_t LCD_data;
//LCD_data  -> start 버튼을 받았을떄  모터 굴리기 시작

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

void  Uart_send(float a,float b){
	char buff[50];
	sprintf(buff,"angle : %f =>test, distance : %f\n",a,b);
	HAL_UART_Transmit(&huart6, (uint8_t*) buff, strlen(buff),HAL_MAX_DELAY);
}
void  Uart_send32(uint32_t a){
	char buff[50];
	sprintf(buff,"enc : %lu",(unsigned long)a);
	HAL_UART_Transmit(&huart6, (uint8_t*) buff, strlen(buff),HAL_MAX_DELAY);
}
void  Uart_send1(float a,float b){
	char buff[50];
	sprintf(buff,"angle : %f, distance : %f=>test\n",a,b);
	HAL_UART_Transmit(&huart6, (uint8_t*) buff, strlen(buff),HAL_MAX_DELAY);
}
void  Uart_send2(){
	char buff[50];
	sprintf(buff,"middle stop\n");
	HAL_UART_Transmit(&huart6, (uint8_t*) buff, strlen(buff),HAL_MAX_DELAY);
}
void  Uart_Number_send(){
	char buff[50];
	sprintf(buff,"stoping  \r\n");
	HAL_UART_Transmit(&huart6, (uint8_t*) buff, strlen(buff),HAL_MAX_DELAY);
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_UART5_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_CRC_Init();
  MX_RNG_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  //  MX_USART2_UART_Init(); 		GPS o
  //  MX_USART3_UART_Init(); 		bluetooth o
  //  MX_UART5_Init();				motor o
  //  MX_USART6_UART_Init();		serial o
  //  MX_I2C1_Init(); 				tof

  MOTOR_Init();																	// MOTOR default code
  GPS_Init();																	// GPS default code
  //bluetooth의 uart포트를 바꾸고 싶으면 'bluetooth.h'에서 BLUETOOTH_USART에 할당된 ()안의 값만 바꾸면
  HAL_UART_Receive_IT(BLUETOOTH_USART,&rx3_data, sizeof(rx3_data));
  HAL_UART_Receive_IT(&huart5, &enc, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //앱인벤터로 gps값을 보내는 함수로 계속 이함수를 사용해줘야 앱인벤터로 지속적으로 gps값이 들어갈 수 있음

//	  SIBAL_SendData();
	  GPS_SendData();
//	  B_Forward(10);
//////	  GETENCODERSM1M2(&hroboClaw1);
//	  //우리가 앱인벤터로 받은 값이 파싱이  잘 되었는지 보기위한 함수
//	  ResetEnc();
//	  B_turn(8);

	  if(point != 236){
//	  AppValue_SendData1();
//	  if(start ==2 || ((check == 1 || check1 == 1) ||(GPS_angle >-5 && GPS_angle<5))){
		  if(start !=2){
			 // 처음에 각도를 받았으니 check 0으로 다시 셋팅
//			  ResetEnc();
			  APP_angle = GPS_angle;
	//		  APP_distance =GPS_distance;
		  }
	//	  if((start ==2 || (check != 1||check1 != 1))&&(GPS_angle <-5  || GPS_angle>5)){
//		  if(start ==2 || (GPS_angle <-5  || GPS_angle>5)){

			  if(start !=2 && (GPS_angle <-5  || GPS_angle>5)){
	//	  		  check1=0;// 처음에 각도를 받았으니 check 0으로 다시 셋팅
	//	  		  APP_angle = GPS_angle;
				  APP_distance =GPS_distance;
			  }
			  if(start == 1){//LCD에서 출발 버튼을 눌렀을때->  일단은 앱인벤터로부터 gps 값을 받았을때
				  B_turn(APP_angle);
	//			  Uart_send(APP_angle, APP_distance);
				  if(check1 >0){ //
	//				  check=0;
//					  check1=0;
					  B_Forward(APP_distance);
	//				  Uart_send1(APP_angle, APP_distance);
				  }
				  if(point==1 ||point == 2){
					  start =2;
				  }
			  }else if(start == 2 ){// 코드가 실행되는 동안 point가 바뀌어서 실행이 안될수도??
				  if(point ==2){// 최종 목적지에 도착했을떄 실행
					  check1 =0;

					  ResetEnc();
//					  Uart_Number_send();

				  }
		//			  DriveM1M2withSpeedandPosition(&hroboClaw1, 0,0,0,0);            // Forward 1m distance
		//			  DriveM1M2withSpeedandPosition(&hroboClaw2, 0,0,0,0);
				  else if(point==1){//중간 마커 지점에 도착했을때 실행
						  //거리 엔코더
					  ResetEnc();
					  start =1;
					  check1 =0;
//					  check=0;
//					  Uart_send2();
			  }
		//				  B_turn(APP_angle);
		//
		//				  if(check ==1){
		//					  B_Forward(GPS_distance);
		//				  }
					  }
	  }else {
		  ResetEnc();
		  B_Forward(0);
	  }





//		  else if(check ==1){ //중간 마커지점에 도착하지도 않았는데 엔코더값을 채웠을때 실행
//			  	  check=0;
//				  B_turn(APP_angle);
//				  if(check ==1){
//			  B_Forward(GPS_distance);
//
//				  }
//		  }
	  // 나중에 lcd의 start 버튼을 눌렀을떄 start =1로 다시 가게 해서 루프 돌게해야겠다.

	  //각도값이 실시간으로 들어올떄 -> 3~-3사이의 각도가 들어오면 직진 => 해결
	  //check가 1일때 다음것을 실행한
	  //처음 각도까지는 잘 작동하는데 그다음 거리부터 문제가 생길수 있음
	  // 지속적으로 각도 값이 들어올떄 =>해결
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
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
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

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if(huart == &huart2) GPS_UART_CallBack();
//	else if(huart == &huart6){
//
//	}
	else if(huart == &huart3){
		 count1++;
	      APP_VALUE_PARSE_CallBack();
	      if(count1==1){
	      start=1;
	      }
//	      if(start !=2 && (GPS_angle <-5  || GPS_angle>5)){
//	      			 // 처음에 각도를 받았으니 check 0으로 다시 셋팅
//	      			  ResetEnc();
//	      			  APP_angle = GPS_angle;
//	      	//		  APP_distance =GPS_distance;
//	      		  }
//	      	//	  if((start ==2 || (check != 1||check1 != 1))&&(GPS_angle <-5  || GPS_angle>5)){
//	      //		  if(start ==2 || (GPS_angle <-5  || GPS_angle>5)){
//
//	      			  if(start !=2 && (GPS_angle <-5  || GPS_angle>5)){
//	      	//	  		  check1=0;// 처음에 각도를 받았으니 check 0으로 다시 셋팅
//	      	//	  		  APP_angle = GPS_angle;
//	      				  APP_distance =GPS_distance;
//	      			  }
	   }
//	else if(huart -> Instance == UART5){
//	      if(enc != 0xFF){
//	         encV[count1++] = enc;
//	         if(count1 == 9){
//	            encM1 = 0;
//	            count1 = 0;
//	            for(int i = 0; i < 4; i++){
//	               if(i < 4) encM1 |= (uint32_t) encV[i] << (8 * (i));
////	               else if(i >= 4 && i < 8) encM2 |= (uint32_t) encV[i] >> (8 * (i - 4));
//	            }
//	            Uart_send32(encM1);
//	         }
//	      }
//	      if (!(stopM1[count2] == encM1)) {
//	         stopM1[0] = encM1;
//
//	         count2 = 0;
//	      }
//	      else {
//
//	         stopM1[count2++] = encM1;
//
//	         if (count2 == 5) {
//	        	 ResetEnc();
////	        	 check = 1;
//	         }
//	      }
//	      HAL_UART_Receive_IT(&huart5, &enc, 1);
//	   }
	}

/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
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
