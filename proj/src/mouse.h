#ifndef __MOUSE_H
#define __MOUSE_H

extern unsigned char array[3];
extern unsigned char array_pos;

//MACROS

#define BIT(n) (0x01<<(n))

#define MOUSE_IRQ	    	12   	/**< @brief Mouse IRQ line */
#define WAIT_KBC			20000 	/**< @brief Wait time from KBD controller */

/* Keyboard and Mouse controller ports */

#define STAT_REG			0x64 	/**< @brief KBC status register */
#define CMD_REG				0x64 	/**< @brief KBC command register */
#define OUT_BUF 			0x60 	/**< @brief KEYBOARD output buffer */

/* Status register bits */

#define IBF					BIT(1) 	/**< @brief Input buffer status register bit */
#define OBF					BIT(0) 	/**< @brief Output buffer status register bit */
#define PAR_ERR				BIT(7) 	/**< @brief Parity error status register bit */
#define AUX					BIT(5) 	/**< @brief Auxiliar Device status register bit */


/* KBC Mouse-Related Commands */

#define WRITE_TO_MOUSE		0xD4 	/**< @brief KBC command to write to mouse */
#define SET_STREAM_MODE 	0xEA	/**< @brief KBC command to set stream mode */
#define ENABLE_DATA_REPORT	0xF4	/**< @brief KBC command to enable data report (in stream mode only) */
#define DISABLE_DATA_REPORT 0xF5	/**< @brief KBC command to disable data report */
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

//Functions declaration


/**
 * @brief subscribe the mouse interrupt
 *
 * @return Return hook_id bit upon success and -1 otherwise
 */
int mouse_subscribe_int();


/**
 * @brief unsubscribe the mouse interrupt
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe_int();


/**
 * @brief issues command to the mouse
 *
 * Verifies if output buffer is full (returning 1 if it is),
 * writes write_to_mouse command to command register,
 * writes the command to the output buffer,
 * read the command the mouse sent to the output buffer,
 * if it is ACK returns 0
 * if it is NACK tries again
 * if it is ERROR returns 1
 *
 * @param command command to be sent
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_issue_command(unsigned long command);


/**
 * @brief Set stream mode
 *
 * Sets stream mode by writing SET_STREAM_MODE and ENABLE_DATA_REPORT commands
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_stream_mode();


/**
 * @brief handles mouse interrupt
 *
 * Reads byte sent by mouse to the OUTBUFF
 */
void mouse_int_handler();

/**
 * @brief Disables mouse data report
 *
 * Writes disable data report command to the KBC
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_disable_data_report();

#endif /* __MOUSE_H */
