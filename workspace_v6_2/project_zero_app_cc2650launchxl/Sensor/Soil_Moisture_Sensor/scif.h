/** \mainpage Driver Overview
  *
  * \section section_drv_info Driver Information
  * This Sensor Controller Interface driver has been generated by the Texas Instruments Sensor Controller
  * Studio tool:
  * - <b>Project name</b>:     Soil Moisture
  * - <b>Project file</b>:     D:/Ben/Documents/Texas Instruments/Sensor Controller Studio/projects/soilMoisture.scp
  * - <b>Code prefix</b>:      -
  * - <b>Operating system</b>: TI-RTOS
  * - <b>Tool version</b>:     1.4.0.42970
  * - <b>Tool patches</b>:     None
  * - <b>Target chip</b>:      CC2650, revision -, package QFN48 7x7 RGZ
  * - <b>Created</b>:          2017-02-11 12:11:02.853
  * - <b>Computer</b>:         DESKTOP-CH7LS9R
  * - <b>User</b>:             Ben
  *
  * No user-provided resource definitions were used to generate this driver.
  *
  * No user-provided procedure definitions were used to generate this driver.
  *
  * Do not edit the generated source code files other than temporarily for debug purposes. Any
  * modifications will be overwritten by the Sensor Controller Studio when generating new output.
  *
  * \section section_drv_modules Driver Modules
  * The driver is divided into three modules:
  * - \ref module_scif_generic_interface, providing the API for:
  *     - Initializing and uninitializing the driver
  *     - Task control (for starting, stopping and executing Sensor Controller tasks)
  *     - Task data exchange (for producing input data to and consume output data from Sensor Controller
  *       tasks)
  * - \ref module_scif_driver_setup, containing:
  *     - The AUX RAM image (Sensor Controller code and data)
  *     - I/O mapping information
  *     - Task data structure information
  *     - Driver setup data, to be used in the driver initialization
  *     - Project-specific functionality
  * - \ref module_scif_osal, for flexible OS support:
  *     - Interfaces with the selected operating system
  *
  * It is possible to use output from multiple Sensor Controller Studio projects in one application. Only
  * one driver setup may be active at a time, but it is possible to switch between these setups. When
  * using this option, there is one instance of the \ref module_scif_generic_interface and
  * \ref module_scif_osal modules, and multiple instances of the \ref module_scif_driver_setup module.
  * This requires that:
  * - The outputs must be generated using the same version of Sensor Controller Studio
  * - The outputs must use the same operating system
  * - The outputs must use different source code prefixes (inserted into all globals of the
  *   \ref module_scif_driver_setup)
  *
  *
  * \section section_project_info Project Description
  * No description entered
  *
  *
  * \subsection section_io_mapping I/O Mapping
  * Task I/O functions are mapped to the following pins:
  * - Soil Moisture Sensor:
  *     - <b>A: Soil Moisture output</b>: DIO25
  *     - <b>O: Soil Moisture Power</b>: DIO26
  *
  *
  * \section section_task_info Task Description(s)
  * This driver supports the following task(s):
  *
  *
  * \subsection section_task_desc_soil_moisture_sensor Soil Moisture Sensor
  * No description entered
  *
  */




/** \addtogroup module_scif_driver_setup Driver Setup
  *
  * \section section_driver_setup_overview Overview
  *
  * This driver setup instance has been generated for:
  * - <b>Project name</b>:     Soil Moisture
  * - <b>Code prefix</b>:      -
  *
  * The driver setup module contains the generated output from the Sensor Controller Studio project:
  * - Location of task control and scheduling data structures in AUX RAM
  * - The AUX RAM image, and the size the image
  * - Task data structure information (location, size and buffer count)
  * - I/O pin mapping translation table
  * - Task resource initialization and uninitialization functions
  *
  * @{
  */
#ifndef SCIF_H
#define SCIF_H

#include <stdint.h>
#include <stdbool.h>
#include "../../Sensor/Soil_Moisture_Sensor/scif_framework.h"
#include "../../Sensor/Soil_Moisture_Sensor/scif_osal_tirtos.h"


/// Target chip name
#define SCIF_TARGET_CHIP_NAME_CC2650
/// Target chip package
#define SCIF_TARGET_CHIP_PACKAGE_QFN48_7X7_RGZ

/// Number of tasks implemented by this driver
#define SCIF_TASK_COUNT 1

/// Soil Moisture Sensor: Task ID
#define SCIF_SOIL_MOISTURE_SENSOR_TASK_ID 0


/// Soil Moisture Sensor: Bin count
#define SCIF_SOIL_MOISTURE_SENSOR_BIN_COUNT 5
/// Soil Moisture Sensor: Thresholds value
#define SCIF_SOIL_MOISTURE_SENSOR_THRESHOLD_COUNT 6
/// Soil Moisture Sensor I/O mapping: Soil Moisture output
#define SCIF_SOIL_MOISTURE_SENSOR_DIO_A_S_M_OUTPUT 25
/// Soil Moisture Sensor I/O mapping: Soil Moisture Power
#define SCIF_SOIL_MOISTURE_SENSOR_DIO_O_S_M_POWER 26


// All shared data structures in AUX RAM need to be packed
#pragma pack(push, 2)


/// Soil Moisture Sensor: Task configuration structure
typedef struct {
    uint16_t hysteresis;        ///< Hysteresis
    uint16_t pBinThresholds[6]; ///< Thresholds for each bin

} SCIF_SOIL_MOISTURE_SENSOR_CFG_T;


/// Soil Moisture Sensor: Task output data structure
typedef struct {
    uint16_t bin; ///< Current bin value

} SCIF_SOIL_MOISTURE_SENSOR_OUTPUT_T;


/// Soil Moisture Sensor: Task state structure
typedef struct {
    uint16_t adcValue;    ///< Last ADC value (for debugging and display only)
    uint16_t forceOutput; ///< Force output the next time the task executes

} SCIF_SOIL_MOISTURE_SENSOR_STATE_T;


/// Sensor Controller task data (configuration, input buffer(s), output buffer(s) and internal state)
typedef struct {
    struct {
        SCIF_SOIL_MOISTURE_SENSOR_CFG_T cfg;
        SCIF_SOIL_MOISTURE_SENSOR_OUTPUT_T output;
        SCIF_SOIL_MOISTURE_SENSOR_STATE_T state;
    } soilMoistureSensor;
} SCIF_TASK_DATA_T;

/// Sensor Controller task generic control (located in AUX RAM)
#define scifTaskData    (*((volatile SCIF_TASK_DATA_T*) 0x400E00E8))


// Initialized internal driver data, to be used in the call to \ref scifInit()
extern const SCIF_DATA_T scifDriverSetup;


// Restore previous struct packing setting
#pragma pack(pop)


// AUX I/O re-initialization functions
void scifReinitTaskIo(uint32_t bvTaskIds);


// RTC-based tick generation control
void scifStartRtcTicks(uint32_t tickStart, uint32_t tickPeriod);
void scifStartRtcTicksNow(uint32_t tickPeriod);
void scifStopRtcTicks(void);


#endif
//@}


// Generated by DESKTOP-CH7LS9R at 2017-02-11 12:11:02.853
