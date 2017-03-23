#pragma config(UART_Usage, UART1, uartUserControl, baudRate115200, IOPins, None, None)
#pragma config(Sensor, in1,    intakePot,      sensorPotentiometer)
#pragma config(Sensor, in2,    liftPot,        sensorPotentiometer)
#pragma config(Sensor, dgtl1,  rightQuad,      sensorQuadEncoder)
#pragma config(Sensor, dgtl6,  leftQuad,       sensorQuadEncoder)
#pragma config(Sensor, dgtl10, stopBtn,        sensorTouch)
#pragma config(Sensor, dgtl11, testBtn,        sensorTouch)
#pragma config(Sensor, dgtl12, liftStopButton, sensorTouch)
#pragma config(Motor,  port1,           lidar,         tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           liftRO,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           liftLI,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           driveLFY,      tmotorVex393_MC29, openLoop, encoderPort, dgtl6)
#pragma config(Motor,  port5,           driveRB,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           driveLB,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           driveRFY,      tmotorVex393_MC29, openLoop, encoderPort, dgtl1)
#pragma config(Motor,  port8,           intakeY,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           liftLO,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          liftRI,        tmotorVex393_HBridge, openLoop, encoderPort, None)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define BCI_SEM_DEBUG_FULL_OFF
#define BCI_USE_UART
#define BCI_USE_TIMER
#define BCI_USE_POS_PID
#define BCI_USE_VEL_PID
#include "BCI\BCI.h"

//#define UARTHANDLER_DEBUG
//#define UARTHANDLER_DEBUG_READ
//#define MOVETOPOINT_DEBUG
//#define POINTMATH_DEBUG
#define MPC_DEBUG

#include "uartHandler.c"
#include "pointMath.c"
#include "basicMotorControl.c"
#include "drivingFunctions.c"
#include "turningFunctions.c"
#include "intakeAndLiftHandler.c"
#include "collisionHandler.c"
#include "motorControl.c"
#include "decisionHandler.c"
#include "skills.c"
#include "noWarning.c"

task monitorStop()
{
	while (true)
	{
		if (SensorValue[stopBtn])
		{
			stopAllTasks();
			for (int i = 0; i < 10; i++)
			{
				motor[i] = 0;
			}
		}

		EndTimeSlice();
	}
}

void testMotors(const tSensors btn, int power = 64)
{
	for (int i = 0; i < 10; i++)
	{
		while (true)
		{
			motor[i] = power;
			wait1Msec(250);
			motor[i] = 0;

			if (SensorValue[btn])
			{
				waitForZero(SensorValue[btn]);
				break;
			}

			wait1Msec(15);
		}
	}
}

task main()
{
	startTask(monitorStop);

	clearDebugStream();

	motor[lidar] = 55; //64 at 7.51V, 48 at 8.27V

	initUART();
	initSensors();

	//bool isUserControlled = false, isUserControlled_last = true;
	//int leftVal, rightVal;

	//startTask(testDrive);
	//startTask(testLift);
	intakeAndLiftTask_intakeState = INTAKE_REST;
  intakeAndLiftTask_liftState = LIFT_REST;
	startTask(intakeAndLiftTask);
	startTask(readBuffer);
	wait1Msec(250);

	while(!vexRT[Btn8D]){wait1Msec(15);}
	driveStraight(-550);
	intakeAndLiftTask_intakeState = INTAKE_OPEN;
	intakeAndLiftTask_liftState = LIFT_DOWN;
	startTask(commandRobot);

	while (true) { wait1Msec(15); }

	// while (true)
	// {
	// 	if (SensorValue[skillsBtn])
	// 	{
	// 		wait1Msec(400);
	// 		runSkills();
	//
	// 		//moveToPoint(609, 304 + ONE_TILE_MM, true);
	//
	// 		//intakeAndLiftTask_intakeState = INTAKE_ACUBE;
	// 		//intakeAndLiftTask_liftState = LIFT_DOWN;
	// 		//startTask(intakeAndLiftTask);
	// 		//waitForIntake(INTAKE_ACUBE);
	// 		//driveStraight(ONE_TILE_MM);
	// 		//intakeAndLiftTask_intakeState = INTAKE_CUBE;
	// 		//wait1Msec(250);
	// 		//intakeAndLiftTask_liftState = LIFT_UP;
	// 		//while (true)
	// 		//{
	// 		//	EndTimeSlice();
	// 		//}
	//
	// 		//turnToAbsAngle(0);
	//
	// 	  //turnToAbsAngle(190);
	// 	  //turnToAbsAngle(270);
	// 	}
	//
	// 	//Switch between driver control and autonomous
	// 	if (vexRT[JOY_BTN_RL])
	// 	{
	// 		isUserControlled = !isUserControlled;
	// 		waitForZero(vexRT[JOY_BTN_RL]);
	// 	}
	//
	// 	if (isUserControlled)
	// 	{
	// 		leftVal = vexRT[JOY_JOY_LV];
	// 		rightVal = vexRT[JOY_JOY_RV];
	// 		leftVal = abs(leftVal) < JOY_THRESHOLD ? 0 : leftVal;
	// 		rightVal = abs(rightVal) < JOY_THRESHOLD ? 0 : rightVal;
	//
	// 		setLeftMotors(leftVal);
	// 		setRightMotors(rightVal);
	//
	// 		setIntakeMotors(127 * vexRT[JOY_TRIG_LU] + -127 * vexRT[JOY_TRIG_LD]);
	// 		setLiftMotors(127 * vexRT[JOY_TRIG_RU] + -127 * vexRT[JOY_TRIG_RD]);
	// 	}
	// 	else
	// 	{
	// 		if (isUserControlled != isUserControlled_last)
	// 		{
	// 			startTask(commandRobot);
	// 		}
	// 	}
	//
	// 	isUserControlled_last = isUserControlled;
	//
	// 	if (vexRT[JOY_BTN_RU])
	// 	{
	// 		motor[lidar] += 1;
	// 		waitForZero(vexRT[JOY_BTN_RU]);
	// 	}
	// 	else if (vexRT[JOY_BTN_RD])
	// 	{
	// 		motor[lidar] -= 1;
	// 		waitForZero(vexRT[JOY_BTN_RD]);
	// 	}
	//
	// 	//Task wait
	// 	wait1Msec(15);
	// }
}
