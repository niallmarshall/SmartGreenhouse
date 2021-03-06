/**************************************************************************/
/*!
    @file     MCP342X.h
    @brief    This is a driver for the MCP472X series of Analog-to-Digital
     converters. Based on the Microchip datasheet for MCP3426 and the Arduino
     library by C. Schnarel.
    @author   C. Schnarel, I. Mitchell
    @license  BSD and GNU
    @copyright
    Copyright (c) 2013 C. Schnarel
    Copyright (c) 2019 I. Mitchell

    @section  History
    2013-Dec-24  - First release, C. Schnarel
    2019-Apr-15  - Second release, I. Mitchell
*/
/**************************************************************************/

#ifndef _MCP342X_H_
#define _MCP342X_H_

/********************************************************/
/* header includes */
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>

#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/kernel.h>
#else
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <wiringPiI2C.h>
#include <math.h>
#endif

/** @brief I2C Address of device.
* MCP3421, MCP3425 & MCP3426 are factory programed for any of 0x68 thru 0x6F. */
#define MCP342X_DEFAULT_ADDRESS	0x68

//@{
/** @brief Address and controls definition.
* MCP3422, MCP3423, MCP3424, MCP3427 & MCP3428 addresses are controlled by address lines A0 and A1
 each address line can be low (GND), high (VCC) or floating (FLT). */
#define MCP342X_A0GND_A1GND		0x68
#define MCP342X_A0GND_A1FLT		0x69
#define MCP342X_A0GND_A1VCC		0x6A
#define MCP342X_A0FLT_A1GND		0x6B
#define MCP342X_A0VCC_A1GND		0x6C
#define MCP342X_A0VCC_A1FLT		0x6D
#define MCP342X_A0VCC_A1VCC		0x6E
#define MCP342X_A0FLT_A1VCC		0x6F
//@}

//@{
/** @brief Conversion mode definition. */
#define MCP342X_MODE_ONESHOT	0x00
#define MCP342X_MODE_CONTINUOUS	0x10
//@}

//@{
/** @brief Channel definition. */
/** MCP3421 & MCP3425 have only the one channel and ignore this parameter
* MCP3422, MCP3423, MCP3426 & MCP3427 have two channels and treat 3 & 4 as repeats of 1 & 2, respectively,
* MCP3424 & MCP3428 have all four channels */
#define	MCP342X_CHANNEL_1	0x00
#define	MCP342X_CHANNEL_2	0x20
#define	MCP342X_CHANNEL_3	0x40
#define	MCP342X_CHANNEL_4	0x60
#define	MCP342X_CHANNEL_MASK	0x60
//@}

//@{
/** @brief Sample size definition - also affects the sampling rate.
* 12-bit has a max sample rate of 240sps
* 14-bit has a max sample rate of  60sps
* 16-bit has a max sample rate of  15sps
* 18-bit has a max sample rate of   3.75sps (MCP3421, MCP3422, MCP3423, MCP3424 only) */
#define MCP342X_SIZE_12BIT	0x00
#define MCP342X_SIZE_14BIT	0x04
#define MCP342X_SIZE_16BIT	0x08
#define MCP342X_SIZE_18BIT	0x0C
#define MCP342X_SIZE_MASK  	0x0C
//@}

//@{
/** @brief Programmable gain definition. */
#define MCP342X_GAIN_1X	0x00
#define MCP342X_GAIN_2X	0x01
#define MCP342X_GAIN_4X	0x02
#define MCP342X_GAIN_8X	0x03
#define MCP342X_GAIN_MASK 0x03
//@}

/** @brief /RDY bit definition. */
#define MCP342X_RDY	0x80

/*! @class MCP342X
* @brief I2C sensor device class.
*/
class MCP342X {
    public:
	/*!
	@brief Default I2C device address constructor.
	*/
        MCP342X();
	/*!
	* @brief Specific I2C device address constructor.
	* @param[in] address : I2C slave address
	*/
        MCP342X(uint8_t address);

	/*!
 	* @brief Test connection to the MCP3426X ADC.
 	*/
        bool testConnection(void);

	/*!
        * @brief Set the configuration bits for the ADC.
	*/
        uint8_t configure(void);

        /*!
	* @brief Starts a conversion.
	* @param configData : Structure instance to hold the configured data
	*/
        bool startConversion(uint8_t configData);

        /*!
	* @brief Read the ADC result.
	* @param data : Structure instance to hold the data
	*/
        uint8_t getResult(uint8_t *data);

        /*!
	* @brief Non-blocking read the ADC result.
	* @param data : Structure instance to hold the data
	*/
        uint8_t checkforResult(uint8_t *data);

    private:
	/*!
	* @brief Address of the device.
	*/
        uint8_t devAddr;
	/*!
	* @brief Data configured according to settings.
	*/
        uint8_t configData;
	//float	stepSizeTbl[];
};

#endif /* _MCP342X_H_ */
