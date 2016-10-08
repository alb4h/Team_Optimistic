#pragma config(UART_Usage, UART1, uartUserControl, baudRate9600, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(Sensor, in1,    intakePot,      sensorPotentiometer)
#pragma config(Sensor, dgtl1,  leftQuad,       sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightQuad,      sensorQuadEncoder)
#pragma config(Motor,  port1,            ,             tmotorVex393_HBridge, openLoop, reversed, driveLeft)
#pragma config(Motor,  port10,           ,             tmotorVex393_HBridge, openLoop, reversed, driveRight)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "Bulldog_Core_Includes.h"

//Message length (excluding start byte)
#define MSG_LENGTH 4
short msg[MSG_LENGTH];

task main()
{
	//Total message count
	short msgCount = 0;

	setBaudRate(UART1, baudRate9600);

	while (true)
	{
		/*
			Message structure is
			<start byte 0xFA>
			<short message count>
			<short intake pot val>
			<short left quad val>
			<short right quad val>
		*/

		//Send start byte
		sendChar(UART1, 0xFA);

		//Send msg header
		sendChar(UART1, msgCount++);

		//Send analog data
		sendChar(UART1, (short)SensorValue[intakePot]);

		//Send digital data
		sendChar(UART1, (short)SensorValue[leftQuad]);
		sendChar(UART1, (short)SensorValue[rightQuad]);

		//Task wait
		wait1Msec(15);
	}
}

task readBuffer()
{
	unsigned int index = 0;

	while (true)
	{
		//Start byte
		if ((msg[index] = getChar(UART1)) == 0xFA)
		{
			for (index = 1; index < MSG_LENGTH; index++)
			{
				if ((msg[index] = getChar(UART1)) == 0xFF)
				{
					index--;
				}
			}
			index = 0;
		}

		//Task wait
		wait1Msec(1);
	}
}
