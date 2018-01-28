#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Mouse.
 */

#define BIT(n) (0x01<<(n))

#define MOUSE_IRQ	    	12   	/**< @brief Mouse IRQ line */
#define TIMER0_IRQ	    	0    	/**< @brief Timer 0 IRQ line */
#define WAIT_KBC			20000 	/**< @brief Wait time from KBD controller */
#define FREQ_TIMER_0		60  	/**< @brief Timer 0 Frequency */

/* Keyboard and Mouse controller ports */

#define STAT_REG			0x64 	/**< @brief KBC status register */
#define CMD_REG				0x64 	/**< @brief KBC command register */
#define OUT_BUF 			0x60 	/**< @brief KEYBOARD output buffer */

/* Status register bits */

#define IBF					BIT(1) 	/**< @brief Input buffer status register bit */
#define OBF					BIT(0) 	/**< @brief Output buffer status register bit */
#define PAR_ERR				BIT(7) 	/**< @brief Parity error status register bit */
#define TO_ERR				BIT(6) 	/**< @brief Time-out error status register bit */
#define AUX					BIT(5) 	/**< @brief Auxiliar Device status register bit */


/* KBC Mouse-Related Commands */

#define WRITE_TO_MOUSE		0xD4 	/**< @brief KBC command to write to mouse */
#define SET_STREAM_MODE 	0xEA	/**< @brief KBC command to set stream mode */
#define ENABLE_DATA_REPORT	0xF4	/**< @brief KBC command to enable data report (in stream mode only) */
#define DISABLE_DATA_REPORT 0xF5	/**< @brief KBC command to disable data report */
#define SET_REMOTE_MODE 	0xF0	/**< @brief KBC command to set remote mode */
#define READ_DATA			0xEB	/**< @brief KBC command to read data */

/* Mouse Commands */

#define ACK					0xFA 	/**< @brief Mouse command to check if everything is OK */
#define NACK				0xFE	/**< @brief Mouse command to check if received an invalid byte (re-send if there was any serial communication error) */
#define ERROR				0xFC	/**< @brief Mouse command to check if received a second consecutive invalid byte */

/* Mouse byte 1 bits */

#define YOV					BIT(7) 	/**< @brief Mouse command to Y position */
#define XOV					BIT(6) 	/**< @brief Mouse command to X position */
#define YSIGN				BIT(5) 	/**< @brief Mouse command to Y sign */
#define XSIGN				BIT(4) 	/**< @brief Mouse command to X sign  */
#define MB					BIT(2) 	/**< @brief Mouse command to Middle Button (MB) */
#define RB					BIT(1) 	/**< @brief Mouse command to Right Button (RB) */
#define LB					BIT(0) 	/**< @brief Mouse command to Left Button (LB) */

/* KBC Commands */

#define READ_CMD_BYTE		0x20 	/**< @brief Read Keyboard Controller's Command Byte */
#define WRITE_CMD_BYTE		0x60 	/**< @brief Write Keyboard Controller's Command Byte */

/* KBC Command Byte Bits */

#define MOUSE_INT			BIT(1)	/**< @brief KBC command byte to mouse_int */

/**@}*/

#endif /* _LCOM_I8042_H */
