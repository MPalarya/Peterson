/*
 * sensors_api.c
 *
 *  Created on: Feb 9, 2017
 *      Author: Ben
 */

#include "../Sensor/sensors_api.h"

#include "Board.h"
#include "../Sensor/ex_include_tirtos.h"

/*********************************************************************
 * CONSTANTS
 */

// Task configuration
#define SENSOR_TASK_PRIORITY         4

#ifndef SENSOR_TASK_STACK_SIZE
#define SENSOR_TASK_STACK_SIZE       1024
#endif

/*********************************************************************
 * LOCAL VARIABLES
 */

// Semaphore variable
static Semaphore_Struct semSensorScTaskAlert;

// Task setup
Task_Struct sensorTask;
Char sensorTaskStack[SENSOR_TASK_STACK_SIZE];

/* Pin driver handles */
//static PIN_Handle ledPinHandle;


/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void Sensor_taskFxn(UArg a0, UArg a1)
{
	//uint_t light_val = 1;

	//Log_info0("HELLO! i'm a Sensor reading Task");

	// Initialize the Sensor Controller
	init_soil_moisture_sensor();

	// Main loop
	while (1)
	{
		//Log_info0("pending on Semaphore");
		Semaphore_pend(Semaphore_handle(&semSensorScTaskAlert), BIOS_WAIT_FOREVER);
		//Log_info0("reading...");

		//PIN_setOutputValue(ledPinHandle, Board_LED1, light_val);
		//Log_info2("Turning %s %s",
		//                (IArg)"\x1b[32mLED1\x1b[0m",
		//                (IArg)(light_val?"on":"off"));

		//light_val = 0;

		processTaskAlert_moisture();
		//processTaskAlert_light();
		//processTaskAlert_temp();

		//PIN_setOutputValue(ledPinHandle, Board_LED1, light_val);
		//Log_info2("Turning %s %s",
		//                (IArg)"\x1b[32mLED1\x1b[0m",
		//                (IArg)(light_val?"on":"off"));
		//light_val = 1;
	}

}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

// Semaphore used to wait for Sensor Controller task ALERT event
void sensor_semaphore_init(void)
{
    // Create the semaphore used to wait for Sensor Controller ALERT events
    Semaphore_Params semParams;
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    Semaphore_construct(&semSensorScTaskAlert, 0, &semParams);
}

void sensor_createTask(void)
{
  Task_Params taskParams;

  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = sensorTaskStack;
  taskParams.stackSize = SENSOR_TASK_STACK_SIZE;
  taskParams.priority = SENSOR_TASK_PRIORITY;
  Task_construct(&sensorTask, Sensor_taskFxn, &taskParams, NULL);
}

void semaphore_post_sensor(void)
{
	Semaphore_post(Semaphore_handle(&semSensorScTaskAlert));
} // semaphore_post_moisture


void init_sensors()
{
	sensor_semaphore_init();
	sensor_createTask();
}

void read_sensors()
{
	semaphore_post_sensor();
}


