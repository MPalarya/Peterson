/*
 * senors_api.h
 *
 *  Created on: Feb 9, 2017
 *      Author: Ben
 */

#ifndef SENSOR_SENSORS_API_H_
#define SENSOR_SENSORS_API_H_

//#include "../Sensor/Light_Sensor/light_sensor.h"
#include "../Sensor/Soil_Moisture_Sensor/soil_moisture.h"
//#include "../Sensor/Temp_Sensor/temp_sensor.h"

extern void read_sensors();
extern void init_sensors();

#endif /* SENSOR_SENSORS_API_H_ */
