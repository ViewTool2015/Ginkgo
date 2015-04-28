/*  $Date: 2011/09/15 15:09:57 $
 *  Revision: 1.14 */
/*
* Copyright (C) 2011 Bosch Sensortec GmbH
*
* BMP180 pressure sensor API
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

/*! \file bmp180.c
    \brief This file contains all function implementations for the BMP180 API
    Details.*/

#include "bmp180.h"


struct bmp180_t *p_bmp180;          /**< pointer to BMP180 device area */

/** initialize BMP180

  This function initializes the BMP180 pressure sensor.
  The function automatically detects the sensor type and stores this for all
  future communication and calculation steps
  \param *bmp180_t pointer to bmp180 device data structure
  \return result of communication routines */

int bmp180_init(bmp180_t *bmp180)
{
   char comres = 0;
   unsigned char data;

   p_bmp180 = bmp180;                   /* assign BMP180 ptr */
   p_bmp180->sensortype = E_SENSOR_NOT_DETECTED;
   p_bmp180->dev_addr = BMP180_I2C_ADDR;   /* preset BMP180 I2C_addr */
   comres += p_bmp180->BMP180_BUS_READ_FUNC(p_bmp180->dev_addr,\
		BMP180_CHIP_ID__REG, &data, 1);  /* read Chip Id */

   p_bmp180->chip_id = BMP180_GET_BITSLICE(data, BMP180_CHIP_ID);
   p_bmp180->number_of_samples = 1;
   p_bmp180->oversampling_setting = 0;
   p_bmp180->sw_oss = 0;
	if (p_bmp180->chip_id == BMP180_CHIP_ID) {
		/* get bitslice */
		p_bmp180->sensortype = BOSCH_PRESSURE_BMP180;
		/* read Version reg */
		comres += p_bmp180->BMP180_BUS_READ_FUNC(p_bmp180->dev_addr,\
		BMP180_VERSION_REG, &data, 1);

		/* get ML Version */
		p_bmp180->ml_version = BMP180_GET_BITSLICE(data,\
		BMP180_ML_VERSION);
		/* get AL Version */
		p_bmp180->al_version = BMP180_GET_BITSLICE(data,\
		BMP180_AL_VERSION);
		bmp180_get_cal_param(); /*readout bmp180 calibparam structure*/
	}
   return comres;
}
/** read out parameters cal_param from BMP180 memory
   \return result of communication routines*/

int bmp180_get_cal_param(void)
{
   int comres;
   unsigned char data[22];
   comres = p_bmp180->BMP180_BUS_READ_FUNC(p_bmp180->dev_addr,\
	BMP180_PROM_START__ADDR, data, BMP180_PROM_DATA__LEN);

   /*parameters AC1-AC6*/
   p_bmp180->cal_param.ac1 =  (data[0] << 8) | data[1];
   p_bmp180->cal_param.ac2 =  (data[2] << 8) | data[3];
   p_bmp180->cal_param.ac3 =  (data[4] << 8) | data[5];
   p_bmp180->cal_param.ac4 =  (data[6] << 8) | data[7];
   p_bmp180->cal_param.ac5 =  (data[8] << 8) | data[9];
   p_bmp180->cal_param.ac6 = (data[10] << 8) | data[11];

   /*parameters B1,B2*/
   p_bmp180->cal_param.b1 =  (data[12] << 8) | data[13];
   p_bmp180->cal_param.b2 =  (data[14] << 8) | data[15];

   /*parameters MB,MC,MD*/
   p_bmp180->cal_param.mb =  (data[16] << 8) | data[17];
   p_bmp180->cal_param.mc =  (data[18] << 8) | data[19];
   p_bmp180->cal_param.md =  (data[20] << 8) | data[21];
   return comres;
}
/** calculate temperature from ut
  ut was read from the device via I2C and fed into the
  right calc path for BMP180
  \param ut parameter ut read from device
  \return temperature in steps of 0.1 deg celsius
  \see bmp180_read_ut()*/

short bmp180_get_temperature(unsigned long ut)
{
   short temperature;
   long x1, x2;
	if (p_bmp180->sensortype == BOSCH_PRESSURE_BMP180) {
		x1 = (((long) ut - (long) p_bmp180->cal_param.ac6) * \
		(long) p_bmp180->cal_param.ac5) >> 15;
		x2 = ((long) p_bmp180->cal_param.mc << 11) / \
		(x1 + p_bmp180->cal_param.md);
		p_bmp180->param_b5 = x1 + x2;
	}
   temperature = ((p_bmp180->param_b5 + 8) >> 4);  /* temperature in 0.1 deg C*/
   return temperature;
}
/** calculate pressure from up
  up was read from the device via I2C and fed into the
  right calc path for BMP180
  In case of BMP180 averaging is done through oversampling by the sensor IC

  \param ut parameter ut read from device
  \return temperature in steps of 1.0 Pa
  \see bmp180_read_up()*/

long bmp180_get_pressure(unsigned long up)
{
   long pressure, x1, x2, x3, b3, b6;
   unsigned long b4, b7;

   b6 = p_bmp180->param_b5 - 4000;
   /*****calculate B3************/
   x1 = (b6*b6) >> 12;
   x1 *= p_bmp180->cal_param.b2;
   x1 >>= 11;

   x2 = (p_bmp180->cal_param.ac2*b6);
   x2 >>= 11;

   x3 = x1 + x2;

   b3 = (((((long)p_bmp180->cal_param.ac1)*4 + x3) << \
		p_bmp180->oversampling_setting)+2) >> 2;

   /*****calculate B4************/
   x1 = (p_bmp180->cal_param.ac3 * b6) >> 13;
   x2 = (p_bmp180->cal_param.b1 * ((b6*b6) >> 12)) >> 16;
   x3 = ((x1 + x2) + 2) >> 2;
   b4 = (p_bmp180->cal_param.ac4 * (unsigned long) (x3 + 32768)) >> 15;

   b7 = ((unsigned long)(up - b3) * (50000>>p_bmp180->oversampling_setting));
	if (b7 < 0x80000000)
		pressure = (b7 << 1) / b4;
	else
		pressure = (b7 / b4) << 1;

   x1 = pressure >> 8;
   x1 *= x1;
   x1 = (x1 * PARAM_MG) >> 16;
   x2 = (pressure * PARAM_MH) >> 16;
   pressure += (x1 + x2 + PARAM_MI) >> 4;/* pressure in Pa*/
   return pressure;
}
/** read out ut for temperature conversion
   \return ut parameter that represents the uncompensated
    temperature sensors conversion value*/

unsigned short bmp180_get_ut()
{
   unsigned short ut;
   unsigned char data[2];
   unsigned char ctrl_reg_data;
   int wait_time;
   int comres;
	if (p_bmp180->chip_id == BMP180_CHIP_ID  /* get bitslice */) {
		ctrl_reg_data = BMP180_T_MEASURE;
		wait_time = BMP180_TEMP_CONVERSION_TIME;
	}
   comres = p_bmp180->BMP180_BUS_WRITE_FUNC(p_bmp180->dev_addr,\
		BMP180_CTRL_MEAS_REG, &ctrl_reg_data, 1);

   p_bmp180->delay_msec(wait_time);
   comres += p_bmp180->BMP180_BUS_READ_FUNC(p_bmp180->dev_addr,\
		BMP180_ADC_OUT_MSB_REG, data, 2);
   ut = (data[0] << 8) | data[1];
   return ut;
}
/** read out up for pressure conversion
  depending on the oversampling ratio setting up can be 16 to 19 bit
   \return up parameter that represents the uncompensated pressure value*/

unsigned long bmp180_get_up()
{
   int j;         /* j included for loop*/
   unsigned long up = 0;
   unsigned long sum = 0; /* get the calculated pressure data*/
   unsigned char data[3];
   unsigned char ctrl_reg_data;
   int comres = 0;
	if (p_bmp180->chip_id == BMP180_CHIP_ID && p_bmp180->sw_oss == 1 && \
		p_bmp180->oversampling_setting == 3) {
		for (j = 0 ; j < 3; j++) {
				/* 3 times getting pressure data*/
				ctrl_reg_data = BMP180_P_MEASURE + (p_bmp180->\
				oversampling_setting << 6);
				comres = p_bmp180->BMP180_BUS_WRITE_FUNC\
				(p_bmp180->dev_addr, BMP180_CTRL_MEAS_REG, \
				&ctrl_reg_data, 1);
				p_bmp180->delay_msec(2 + (3 << (p_bmp180->\
				oversampling_setting)));
				comres += p_bmp180->BMP180_BUS_READ_FUNC\
				(p_bmp180->dev_addr, BMP180_ADC_OUT_MSB_REG, \
				data, 3);
				sum = (((unsigned long) data[0] << 16) |\
				((unsigned long) data[1] << 8) |\
				(unsigned long) data[2]) >> \
				(8-p_bmp180->oversampling_setting);
				p_bmp180->number_of_samples = 1;
				up = up + sum;  /*add up with dummy var*/
	}
		up = up / 3;   /* averaging*/
	}
  else    {
	if (p_bmp180->chip_id == BMP180_CHIP_ID && p_bmp180->sw_oss == 0) {
		ctrl_reg_data = BMP180_P_MEASURE + (p_bmp180->\
		oversampling_setting << 6);
		comres = p_bmp180->BMP180_BUS_WRITE_FUNC(p_bmp180->dev_addr,\
		BMP180_CTRL_MEAS_REG, &ctrl_reg_data, 1);
		p_bmp180->delay_msec(2 + (3 << (p_bmp180->\
		oversampling_setting)));
		comres += p_bmp180->BMP180_BUS_READ_FUNC(p_bmp180->dev_addr,\
		BMP180_ADC_OUT_MSB_REG, data, 3);
		up = (((unsigned long) data[0] << 16) |\
		((unsigned long) data[1] << 8) |\
		(unsigned long) data[2]) >> (8-p_bmp180->oversampling_setting);
		p_bmp180->number_of_samples = 1;
	}
    }
return up;
}


