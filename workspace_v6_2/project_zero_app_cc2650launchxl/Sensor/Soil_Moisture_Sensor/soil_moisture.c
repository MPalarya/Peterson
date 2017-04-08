/*
 * soil_moisture.c
 *
 *  Created on: Feb 8, 2017
 *      Author: Ben
 */


/*********************************************************************
 * INCLUDES
 */

#include "../../Sensor/ex_include_tirtos.h"
#include "../../Sensor/Soil_Moisture_Sensor/scif.h"

/*********************************************************************
 * CONSTANTS
 */
#define SOIL_MOISTURE_MAX_VAL         4096
/*********************************************************************
 * LOCAL FUNCTIONS
 */
#define BV(x)    (1 << (x))

/*********************************************************************
 * PUBLIC FUNCTIONS
 */


void scCtrlReadyCallback_moisture(void)
{

} // scCtrlReadyCallback_moisture Control Ready

void scTaskAlertCallback_moisture(void)
{
	// Wake up the OS task
	//Semaphore_post(Semaphore_handle(&semSensorScTaskAlert));
} // scTaskAlertCallback_moisture

uint8_t convert_to_digital(uint16_t adc_val)
{
	return (uint8_t)((adc_val/(float)SOIL_MOISTURE_MAX_VAL)*100);
}

void init_soil_moisture_sensor(void)
{
	// Initialize the Sensor Controller
		scifOsalInit();
		scifOsalRegisterCtrlReadyCallback(scCtrlReadyCallback_moisture);
		scifOsalRegisterTaskAlertCallback(scTaskAlertCallback_moisture);
		scifInit(&scifDriverSetup);

		// Set the Sensor Controller task tick interval to 1 second
		uint32_t rtc_Hz = 1;  // 1Hz RTC
		scifStartRtcTicksNow(0x00010000 / rtc_Hz);

		// Start Sensor Controller task
		scifStartTasksNbl(BV(SCIF_SOIL_MOISTURE_SENSOR_TASK_ID));

} // init_soil_moisture_sensor

void processTaskAlert_moisture(void)
{
	// Clear the ALERT interrupt source
	scifClearAlertIntSource();

	// Do SC Task processing here
	// Fetch 'state.adcValue' variable from SC

	uint16_t sm_adcValue = scifTaskData.soilMoistureSensor.state.adcValue;
	uint8_t sm_percent = convert_to_digital(sm_adcValue);

    //Log_info0("############~SOIL MOISTURE SENSOR~############");
    //Log_info1("soil Moisture value: %d", sm_adcValue);
    Log_info1("soil Moisture: %d %%", sm_percent);
    extern uint8_t globalInt;
    globalInt = sm_percent;
    // TODO: make global to use in setparameters
    //Log_info0("##############################################");

	// Acknowledge the ALERT event
	scifAckAlertEvents();

} // processTaskAlert Task Alert

