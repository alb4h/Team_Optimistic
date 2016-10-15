#ifndef UARTHANDLER_H_INCLUDED
#define UARTHANDLER_H_INCLUDED

#include "decisionHandler.c"

/*
	Message send structure is
	<start byte 0xFA>
	<short message count>
	<short intake pot val>
	<short left quad val>
	<short right quad val>
	<short isWorking>

	Message recieve structure is
	<start byte 0xFA>
	<short message count>
	<short estimated x>
	<short estimated y>
	<short estimated theta>
	<short x coordinate demand>
	<short y coordinate demand>
	<short pick up object>
*/

//Message length (excluding start byte)
#define MSG_LENGTH 7

//Current message
short msg[MSG_LENGTH];
#define MSG_COUNT     0
#define MSG_EST_X     1
#define MSG_EST_Y     2
#define MSG_EST_THETA 3
#define MSG_X_COORD   4
#define MSG_Y_COORD   5
#define MSG_PICKUP    6

//Message write semaphore, always get lock before reading or writing
TSemaphore msgSem;

//Total message count
static short msgCount;

void initUART()
{
	msgCount = 0;
	semaphoreInitialize(msgSem);
	setBaudRate(UART1, baudRate9600);
}

//Sends a new message
void sendCurrentData()
{
	//Send start byte
	sendChar(UART1, 0xFA);

	//Send msg header
	sendChar(UART1, msgCount++);

	//Send analog data
	sendChar(UART1, (short)SensorValue[intakePot]);

	//Send digital data
	sendChar(UART1, (short)SensorValue[leftQuad]);
	sendChar(UART1, (short)SensorValue[rightQuad]);

	//Send isWorking flag
	sendChar(UART1, isWorking);
}

//Reads a new message
task readBuffer()
{
	unsigned int index = 0;
	short msgFlagHolder;

	while (true)
	{
		//Load start byte into temp variable
		if ((msgFlagHolder = getChar(UART1)) == 0xFA)
		{
			//Get lock for message array
			semaphoreLock(msgSem);

			//If we have lock
			if (bDoesTaskOwnSemaphore(msgSem))
			{
				//If start flag is seen, read in rest of message
				for (index = 0; index < MSG_LENGTH; index++)
				{
					//0xFF represents empty, so ignore it
					if ((msg[index] = getChar(UART1)) == 0xFF)
					{
						index--;
					}
				}
			}

			//If we still have lock
			if (bDoesTaskOwnSemaphore(msgSem))
			{
				//Write is done, release lock
				semaphoreUnlock(msgSem);
			}
		}

		//Task wait
		wait1Msec(1);
	}
}

#endif //UARTHANDLER_H_INCLUDED
