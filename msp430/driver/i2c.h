/**
 * @file driver/i2c.h
 *
 * @author Richard Treichl
 * @date 07.02.2015
 *
 * @brief I2C implemtation for MSP430G2553, interrupt based.
 * @n This implemtation based on examples from Texas Instruments slaa382.
 * @n For more information see link below.
 * @see http://www.ti.com/lit/an/slaa382/slaa382.zip
 *
 * @todo Find a better way for continue i2c_write_arr() after transmittion on repeated start condition.
 * @n Implement error detection, for example wrong configuration for prescale or detect non existing slave.
 *
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <driver/timer.h>

#define VARIADIC_IMPLEMENTATION 0

#if VARIADIC_IMPLEMENTATION

#warning "Compiling with VARIADIC feature to safe storage, deaktivate this option..."

#else

#define I2C_MAX_ARGS 10
#warning "Compiling without VARIADIC feature I2C_MAX_ARGS has to be choose carefully..."

#endif

/**
 * @enum I2C_CRTL_CMD
 * @brief Control command for sendig a STOP condtion or repeated start.
 */

enum I2C_CRTL_CMD {
	STOP = 0,		///< If this one is set a stop condtion will be send after trasmition or reception.
	REPT			///<  If this one is set no stop condtion is send the I2C stays low on SCL and wait for a new start condtion.
};

/**
 * @enum I2C_CRTL_STATS
 * @brief Give back a status of I2C module.
 */

enum I2C_CRTL_STATS {
	IDLE = 0,		///< Indicates that I2C module is not performing a command.
	TRANSMIT,		///< Indicates that I2C module is performing a transmittion command.
	RECEIVE,		///< Indicates that I2C module is performing a reception command.
	ERROR			///< Indicates that I2C has occured an error.
};

#define I2C_BIG_ENDIAN -1	///< Indecates a big endian system for slave
#define I2C_LITTLE_ENDIAN 1	///< Indecates a little endian system for slave

/**
 *  @brief Setup I2C with a defined clock based on given SMCKL.
 *  @param smclk_freq Frequency of SMCLK clock in Hz
 *  @param i2c_freq Frequency for I2C clock in Hz
 *  @return error
*/

uint8_t i2c_init (uint16_t smclk_freq, uint16_t i2c_freq);

/**
 *  @brief Transmits I2C commands with a variable length to a I2C slave.
 *  @param addr Slave Address in 7-bit format.
 *  @param rept_start Command for stop or repeated start.
 *  @param n_args Number of command bytes.
 *  @param ... I2C command bytes to send.
 *  @return error
*/

uint8_t i2c_write_var (uint8_t addr, enum I2C_CRTL_CMD rept_start, uint8_t n_args, ...);

/**
 *  @brief Transmits I2C command array to a I2C slave.
 *  @param addr Slave Address in 7-bit format.
 *  @param rept_start Command for stop or repeated start.
 *  @param n_size Number of transmitting bytes.
 *  @param *TxData Pointer to I2C command bytes to send.
 *  @return error
*/

uint8_t i2c_write_arr (uint8_t addr,enum I2C_CRTL_CMD rept_start, uint8_t n_size, const void *TxData);

/**
 *  @brief Receives I2C commands until a NACK is received or received RxBytes.
 *  @param addr Slave Address in 7-bit format.
 *  @param rept_start Command for stop or repeated start.
 *  @param endian of slave system.
 *  @param RxBytes Number of receiving bytes.
 *  @param *RxData Pointer where received I2C commands are be writtn.
 *  @return error
*/

uint8_t i2c_read (uint8_t addr, enum I2C_CRTL_CMD rept_start, int8_t endian, uint8_t RXBytes, const void *RxData);

/**
 *  @return Actuall state of I2C module.
*/

uint8_t i2c_get_status();

#endif /* I2C_H_ */
