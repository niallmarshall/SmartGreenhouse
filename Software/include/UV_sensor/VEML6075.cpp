/**************************************************************************/
/*!

    @file     VEML6075.cpp
    @brief    Class for the operation of the Vishay VEML6075 UVA/UVB I2C sensor.
      Based on the Vishay VEML6075 application notes and Adafruit Industries
    @author   I. Mitchell
		@license  GNU
    @copyright
    Copyright (c) 2019 I. Mitchell

    @section  History
    2019-Apr-15  - First release, I. Mitchell
*/

#include <stdio.h>
#include <cstdlib>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "VEML6075.h"


/**************************************************************************/
/*!
    @brief Sets up the wiringPi I2C comms to the VEML6075 device.
*/
/**************************************************************************/
static int setupI2C_veml6075 = wiringPiSetup();
/**************************************************************************/
/*!
    @brief Sets up the wiringPi I2C comms to the VEML6075 device.
    @param[in] VEML6075_ADDR : VEML6075 slave address
*/
/**************************************************************************/
static int fd_uv = wiringPiI2CSetup(VEML6075_ADDR);


/**************************************************************************/
/*!
 * @brief Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
/**************************************************************************/
bool UV_sensor::testConnection() {
  if(fd_uv < 0)
  {
    printf("ERROR: UV sensor could not be found \n");
    return 0;
  } else
  {
    return 1;
    }
}

void UV_sensor::uvConfigure(void) {
  wiringPiI2CWriteReg16(fd_uv,VEML6075_CONF_REG,VEML6075_CONF_UV_AF_AUTO);
  wiringPiI2CWriteReg16(fd_uv,VEML6075_CONF_REG,VEML6075_CONF_UV_TRIG_NO);
  wiringPiI2CWriteReg16(fd_uv,VEML6075_CONF_REG,VEML6075_CONF_UV_IT_100MS);

  wiringPiI2CWriteReg16(fd_uv, VEML6075_CONF_REG, VEML6075_CONF_SD_ON); //shutdown to save
  wiringPiI2CWriteReg16(fd_uv, VEML6075_CONF_REG, VEML6075_CONF_SD_OFF); //power up
}

/**************************************************************************/
/*!
    @brief Constructor initialises default configuration value.
*/
/**************************************************************************/
UV_sensor::UV_sensor() {
  setCoefficients(VEML6075_DEFAULT_UVA_A_COEFF, VEML6075_DEFAULT_UVA_B_COEFF,
		  VEML6075_DEFAULT_UVB_C_COEFF, VEML6075_DEFAULT_UVB_D_COEFF,
		  VEML6075_DEFAULT_UVA_RESPONSE, VEML6075_DEFAULT_UVB_RESPONSE);
}

/**************************************************************************/
/*!
    @brief Set the UVI calculation coefficients.
    @param UVA_A : The UVA visible coefficient
    @param UVA_B : The UVA IR coefficient
    @param UVB_C : The UVB visible coefficient
    @param UVB_D : The UVB IR coefficient
    @param UVA_ : Response the UVA responsivity
    @param UVB_ :Response the UVB responsivity
*/
/**************************************************************************/
void UV_sensor::setCoefficients(float UVA_A, float UVA_B, float UVB_C, float UVB_D,
					float UVA_response, float UVB_response) {
  _uva_a = UVA_A;
  _uva_b = UVA_B;
  _uvb_c = UVB_C;
  _uvb_d = UVB_D;
  _uva_resp = UVA_response;
  _uvb_resp = UVB_response;
}

/**************************************************************************/
/*!
    @brief Perform a reading and calculate UV value.
*/
/**************************************************************************/
float UV_sensor::takeReading() {

  float uva = wiringPiI2CReadReg16(fd_uv, VEML6075_UVA_DATA_REG);
  float uvb = wiringPiI2CReadReg16(fd_uv, VEML6075_UVB_DATA_REG);
  float uvcomp1 = wiringPiI2CReadReg16(fd_uv, VEML6075_UVCOMP1_DATA_REG);
  float uvcomp2 = wiringPiI2CReadReg16(fd_uv, VEML6075_UVCOMP2_DATA_REG);

  // Equation 1 & 2 in App note, without 'golden sample' calibration
  _uva_calc = abs(uva - (_uva_a * uvcomp1) - (_uva_b * uvcomp2));
  _uvb_calc = abs(uvb - (_uvb_c * uvcomp1) - (_uvb_d * uvcomp2));
  _uvi_calc = (((_uva_calc * _uva_resp) + (_uvb_calc * _uvb_resp)) / 2) * 10;
}

/**************************************************************************/
/*!
    @brief  Read the calibrated UVA band reading.
    @return the UVA reading in unitless counts
*/
/*************************************************************************/
float UV_sensor::readUVA(void) {
  takeReading();
  return _uva_calc;
}

/**************************************************************************/
/*!
    @brief  Read the calibrated UVB band reading.
    @return the UVB reading in unitless counts
*/
/*************************************************************************/
float UV_sensor::readUVB(void) {
  takeReading();
  return _uvb_calc;
}

/**************************************************************************/
/*!
    @brief  read and calculate the approximate UV Index reading.
    @return the UV Index as a floating point
*/
/**************************************************************************/
float UV_sensor::readUVI(void) {
  takeReading();
  return _uvi_calc;
}
