/*  $Date: 2011/09/15 15:09:57 $
 *  Revision: 1.14 */
/*
* mainpage BMP180 Pressure Sensor API
* Copyright (C) 2011 Bosch Sensortec GmbH

* section intro_sec Introduction
* BMP180 digital Altimeter Programming Interface
* The BMP180 API enables quick access to Bosch Sensortec's digital altimeter.
* The only mandatory steps are:
* 1. linking the target application's communication functions to the API
  (\ref BMP180_WR_FUNC_PTR, \ref BMP180_RD_FUNC_PTR)
* 2. calling the bmp180_init() routine, which initializes all necessary
  data structures for using all functions*/

/*BMP180 pressure sensor API
*
* Usage:  Application Programming Interface for BMP180 configuration
*         and data read out

* Author:    venkatakrishnan.venkataraman@in.bosch.com

* Licensed under the Apache License, Version 2.0 (the "License"); you may not
  use this file except in compliance with the License and the following
  stipulations.The Apache License , Version 2.0 is applicable unless otherwise
  stated by the stipulations of the disclaimer below.

* You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

* Disclaimer

* Common:
* Bosch Sensortec products are developed for the consumer goods industry. They
* may only be used within the parameters of the respective valid product data
* sheet.Bosch Sensortec products are provided with the express understanding
* that there is no warranty of fitness for a particular purpose.They are not
* fit for use in life-sustaining, safety or security sensitive systems or any
* system or device that may lead to bodily harm or property damage if the
* system or device malfunctions. In addition,Bosch Sensortec products are not
* fit for use in products which interact with motor vehicle systems.The resale
* and/or use of products are at the purchaser's own risk and his own
* responsibility. The examination of fitness for the intended use is the sole
* responsibility of the Purchaser.
*
* The purchaser shall indemnify Bosch Sensortec from all third party claims,
* including any claims for incidental, or consequential damages, arising from
* any product use not covered by the parameters of the respective valid
* product data sheet or not approved by Bosch Sensortec and reimburse Bosch
* Sensortec for all costs in connection with such claims.
*
* The purchaser must monitor the market for the purchased products,
* particularly with regard to product safety and inform Bosch Sensortec
* without delay of all security relevant incidents.
*
* Engineering Samples are marked with an asterisk (*) or (e). Samples may vary
* from the valid technical specifications of the product series. They are
* therefore not intended or fit for resale to third parties or for use in end
* products. Their sole purpose is internal client testing. The testing of an
* engineering sample may in no way replace the testing of a product series.
* Bosch Sensortec assumes no liability for the use of engineering samples. By
* accepting the engineering samples, the Purchaser agrees to indemnify Bosch
* Sensortec from all claims arising from the use of engineering samples.
*
* Special:
* This software module (hereinafter called "Software") and any information on
* application-sheets (hereinafter called "Information") is provided free of
* charge for the sole purpose to support your application work. The Software
* and Information is subject to the following terms and conditions:
*
* The Software is specifically designed for the exclusive use for Bosch
* Sensortec products by personnel who have special experience and training. Do
* not use this Software if you do not have the proper experience or training.
*
* This Software package is provided `` as is `` and without any expressed or
* implied warranties, including without limitation, the implied warranties of
* merchantability and fitness for a particular purpose.
*
* Bosch Sensortec and their representatives and agents deny any liability for
* the functional impairment of this Software in terms of fitness, performance
* and safety. Bosch Sensortec and their representatives and agents shall not
* be liable for any direct or indirect damages or injury, except as
* otherwise stipulated in mandatory applicable law.
*
* The Information provided is believed to be accurate and reliable. Bosch
* Sensortec assumes no responsibility for the consequences of use of such
* Information nor for any infringement of patents or other rights of third
* parties which may result from its use. No license is granted by implication
* or otherwise under any patent or patent rights of Bosch. Specifications
* mentioned in the Information are subject to change without notice.
*
* It is not allowed to deliver the source code of the Software to any third
* party without permission of Bosch Sensortec.*/

/* file bmp180.h
 * brief Header file for all #define constants and function prototypes*/

#ifndef __BMP180_H__
#define __BMP180_H__

#define bmp180_calc_temperature(ut)\
		bmp180_get_temperature(ut)

#define bmp180_calc_pressure(up)\
		bmp180_get_pressure(up)

#define bmp180_read_ut()\
		bmp180_get_ut()

#define bmp180_read_up()\
		bmp180_get_up()

#define bmp180_read_reg(address, data, length)\
		bmp180_get_reg(address, data, length)

#define bmp180_write_reg(address, data, length)\
		bmp180_set_reg(address, data, length)

#define bmp180_read_cal_param()\
		bmp180_get_cal_param()

/* Define for used read and write macros
 * Define the calling convention of YOUR bus communication routine.
 * note This includes types of parameters. This example shows the
 * configuration for an SPI bus link.*/

/* defines the return parameter type of the BMP180_WR_FUNCTION */
#define BMP180_BUS_WR_RETURN_TYPE signed char

/* defines the calling parameter types of the BMP180_WR_FUNCTION */
#define BMP180_BUS_WR_PARAM_TYPES unsigned char, unsigned char,\
			unsigned char *, unsigned char

/* links the order of parameters defined in BMP180_BUS_WR_PARAM_TYPE
 * to function calls used inside the API */
#define BMP180_BUS_WR_PARAM_ORDER (device_addr, register_addr,\
			register_data, write_length)

/* never change this line  */
#define BMP180_BUS_WRITE_FUNC(device_addr, register_addr,\
			register_data, write_length)\
	bus_write(device_addr, register_addr, register_data, write_length)

/* defines the return parameter type of the BMP180_WR_FUNCTION */
#define BMP180_BUS_RD_RETURN_TYPE signed char

/* defines the calling parameter types of the BMP180_WR_FUNCTION */
#define BMP180_BUS_RD_PARAM_TYPES unsigned char, unsigned char,\
			unsigned char *, unsigned char

/* links the order of parameters defined in BMP180_BUS_WR_PARAM_TYPE
 * to function calls used inside the API*/
#define BMP180_BUS_RD_PARAM_ORDER (device_addr, register_addr,\
			register_data, read_length)

/* never change this line */
#define BMP180_BUS_READ_FUNC(device_addr, register_addr,\
			register_data, read_length)\
		bus_read(device_addr, register_addr, register_data, read_length)

/*        CHIP_TYPE CONSTANTS */
#define BMP180_CHIP_ID                      0x55
#define BOSCH_PRESSURE_BMP180   85

/*        BMP180 I2C Address */
#define BMP180_I2C_ADDR         (0xEE>>1)

/*        SMB380 API error codes */
#define E_BMP_NULL_PTR                      (char)  (-127)
#define E_BMP_COMM_RES                     (char) (-1)
#define E_BMP_OUT_OF_RANGE              (char) (-2)
#define E_SENSOR_NOT_DETECTED        (char) 0

/*     register definitions     */
#define BMP180_PROM_START__ADDR         0xaa
#define BMP180_PROM_DATA__LEN             22

#define BMP180_CHIP_ID_REG                      0xD0
#define BMP180_VERSION_REG                      0xD1

#define BMP180_CTRL_MEAS_REG            0xF4
#define BMP180_ADC_OUT_MSB_REG          0xF6
#define BMP180_ADC_OUT_LSB_REG          0xF7

#define BMP180_SOFT_RESET_REG           0xE0

#define BMP180_T_MEASURE        0x2E  /* temperature measurent */
#define BMP180_P_MEASURE        0x34  /* pressure measurement */

#define BMP180_TEMP_CONVERSION_TIME  5 /* TO be spec'd by GL or SB */

#define PARAM_MG      3038        /*calibration parameter */
#define PARAM_MH     -7357        /*calibration parameter */
#define PARAM_MI      3791        /*calibration parameter */

/* register write and read delays */

#define BMP180_MDELAY_DATA_TYPE unsigned int
#define BMP180_MDELAY_RETURN_TYPE  void
/* this structure holds all device specific calibration parameters */
struct bmp180_calibration_param_t{
   short ac1;
   short ac2;
   short ac3;
   unsigned short ac4;
   unsigned short ac5;
   unsigned short ac6;
   short b1;
   short b2;
   short mb;
   short mc;
   short md;
};
/* BMP180 image registers data structure */
struct bmp180_t {
   struct bmp180_calibration_param_t cal_param;
   unsigned char mode;
   unsigned char chip_id, ml_version, al_version;
   unsigned char dev_addr;
   unsigned char sensortype;

   long param_b5;
   int number_of_samples;
   short oversampling_setting;
   short sw_oss;
   BMP180_BUS_WR_RETURN_TYPE(*bus_write)(BMP180_BUS_WR_PARAM_TYPES);
   BMP180_BUS_RD_RETURN_TYPE(*bus_read)(BMP180_BUS_RD_PARAM_TYPES);
   BMP180_MDELAY_RETURN_TYPE(*delay_msec)(BMP180_MDELAY_DATA_TYPE);
};
/* bit slice positions in registers */

#define BMP180_CHIP_ID__POS             0
#define BMP180_CHIP_ID__MSK             0xFF
#define BMP180_CHIP_ID__LEN             8
#define BMP180_CHIP_ID__REG             BMP180_CHIP_ID_REG

#define BMP180_ML_VERSION__POS          0
#define BMP180_ML_VERSION__LEN          4
#define BMP180_ML_VERSION__MSK          0x0F
#define BMP180_ML_VERSION__REG          BMP180_VERSION_REG

#define BMP180_AL_VERSION__POS          4
#define BMP180_AL_VERSION__LEN          4
#define BMP180_AL_VERSION__MSK          0xF0
#define BMP180_AL_VERSION__REG          BMP180_VERSION_REG

/* DATA REGISTERS
 * LG/HG thresholds are in LSB and depend on RANGE setting
 * no range check on threshold calculation*/
#define BMP180_GET_BITSLICE(regvar, bitname)\
	((regvar & bitname##__MSK) >> bitname##__POS)

#define BMP180_SET_BITSLICE(regvar, bitname, val)\
	((regvar & ~bitname##__MSK) | ((val<<bitname##__POS)&bitname##__MSK))
/* General Setup Functions */
/* BMP180_init */

/*   input :      Pointer to bmp180_t
 *   output:  -
 *   return:  result of communication function
 *   notes : */

int bmp180_init(bmp180_t *bmp180);

short bmp180_get_temperature(unsigned long ut);

long bmp180_get_pressure(unsigned long up);

unsigned short bmp180_get_ut(void);
unsigned long  bmp180_get_up(void);

/* MISC RAW functions */

/* read: address, data-pointer, length */
char bmp180_get_reg(unsigned char , unsigned char *, unsigned char);

/* write: address, data-pointer, length */
char bmp180_set_reg(unsigned char , unsigned char*, unsigned char);

/* API internal helper functions */
int bmp180_get_cal_param(void);

#endif   /* __BMP180_H__ */
