#include "roboClaw.h"
#include <stdio.h>

SERIAL_HandleTypeDef *hserial_uart5;
RoboClaw_HandleTypeDef hroboClaw1;
RoboClaw_HandleTypeDef hroboClaw2;
extern uint8_t check1;
extern float GPS_angle;
//uint8_t dCnt=0;
void MOTOR_Init()
{
	  hroboClaw1.hserial = hserial_uart5;
	  hroboClaw1.packetserial_address = 0x80;
	  roboClaw_init(&hroboClaw1);
	  ResetEncoders(&hroboClaw1);
	  hroboClaw2.hserial = hserial_uart5;
	  hroboClaw2.packetserial_address = 0x81;
	  roboClaw_init(&hroboClaw2);
	  ResetEncoders(&hroboClaw2);

//	  float Kp = 66.57174;  // Proportional coefficient
//	  float Ki = 1.84910;  // Integral coefficient
//	  float Kd = 554.73061;  // Derivative coefficient
//	  uint32_t QPPS = 45000;
//
//	  SetM1VelocityPID(&hroboClaw, 0, 0, 0, QPPS);
//	  SetM2VelocityPID(&hroboClaw, 0, 0, 0, QPPS);
//	  SetM1PositionPID(&hroboClaw, Kp, Ki, Kd, 43, 0, 0, 50000);
//	  SetM2PositionPID(&hroboClaw, Kp, Ki, Kd, 43, 0, 0, 50000);
//	  WriteNVM(&hroboClaw);
}
//
//void L_Forward()
//{
//
//	DriveM1M2withSpeedandPosition(&hroboClaw1, 43000, 3774, 43000, 3736);				// Forward 1m distance
//	DriveM1M2withSpeedandPosition(&hroboClaw2, 42550, 3770, 43000, 3774);				// Forward 1m distance
////	DriveM1M2withSpeedandPosition(&hroboClaw1, 42500, -3849, 43000, -3810);				// Backward 1m distance
////	DriveM1M2withSpeedandPosition(&hroboClaw2, 41950, -3845, 43000, -3849);				// Backward 1m distance
//}
//
//void L_Left(float angle)
//{
//	if (!(angle >= -3 && angle <= 3))
//		{
//			DriveM1M2withSpeedandPosition(&hroboClaw1, 42550, 4700 / 180 * angle, 40550, -4680 / 180 * angle);				// left 180
//			DriveM1M2withSpeedandPosition(&hroboClaw2, 42000, 4700 / 180 * angle, 40550, -4750 / 180 * angle);				// left 180
//		}
//
//}
//
//void L_Right(float angle)
//{
//	if (!(angle >= -3 && angle <= 3))
//		{
//			DriveM1M2withSpeedandPosition(&hroboClaw1, 43000, 4807 / 180 * angle, 43000, -4885 / 180 * angle);				// right 180
//			DriveM1M2withSpeedandPosition(&hroboClaw2, 42550, 4780 / 180 * angle, 43000, -4918 / 180 * angle);				// right 180
//		}
//}
//
//void L_turn(float angle)
//{
//	if (!(angle >= -3 && angle <= 3))
//		{
//			DriveM1M2withSpeedandPosition(&hroboClaw1, 43000, 4807 / 180 * angle, 43000, -4885 / 180 * angle);				// right 180
//			DriveM1M2withSpeedandPosition(&hroboClaw2, 42550, 4780 / 180 * angle, 43000, -4918 / 180 * angle);				// right 180
//		}
//}
//
void ResetEnc(){
	SetEncM1(&hroboClaw1,0);
	SetEncM2(&hroboClaw1,0);
	SetEncM1(&hroboClaw2,0);
	SetEncM2(&hroboClaw2,0);

}
void B_Forward(float B_distance)
{

	DriveM1M2withSpeedandPosition(&hroboClaw1, 3302, 3765*B_distance, 3300, 3765*B_distance);
	DriveM1M2withSpeedandPosition(&hroboClaw2, 3200, 3691*B_distance, 3218, 3710*B_distance);
//	DriveM1M2withSpeedandPosition(&hroboClaw1, 42500, -3849 * B_distance, 43000, -3810 * B_distance);				// Backward 1m distance
//	DriveM1M2withSpeedandPosition(&hroboClaw2, 41950, -3845 * B_distance, 43000, -3849 * B_distance);				// Backward 1m distance
}

void B_turn(float angle)
{

	if(GPS_angle > 5)
	{
		check1 =0;
		ResetEnc();
		DriveM1M2withSpeedandPosition(&hroboClaw1, 3302, 3765/138*angle, 3300, -3765/138*angle);
		DriveM1M2withSpeedandPosition(&hroboClaw2, 3200, 3696/138*angle, 3218, -3710/138*angle);				// right 180
	}
	else if (GPS_angle < -5)
	{
		check1 =0;
		ResetEnc();
		DriveM1M2withSpeedandPosition(&hroboClaw1, 3302, 3765/138*angle, 3300, -3765/138*angle);
		DriveM1M2withSpeedandPosition(&hroboClaw2, 3200, 3696/138*angle, 3218, -3710/138*angle);
	}else{
		//직진에 해당하는 각도가 들어왔을때 직진을 해라는 코드
		// 각도값이 -3 ~ 3사이의 값이 들어오면 직진해라
		check1++;
		if(check1==1){
			ResetEnc();
		}
	}
}
