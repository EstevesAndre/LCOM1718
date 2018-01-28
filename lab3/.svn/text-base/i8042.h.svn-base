#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard.
 */

#define BIT(n) (0x01<<(n))

#define KBD_IRQ	       		1   /**< @brief KBD IRQ line */
#define WAIT_KBC			20000 /**< @brief Wait time from KBD controller */

/* Make/Breakcode */

#define BREAKCODE_BIT		BIT(7) /**< @brief Release code bit */
#define TWO_BYTE			0xe0 /**< @brief Most significative byte when scancode is two byte long */

/* I/O port addresses */

#define OUT_BUF 		0x60 /**< @brief KEYBOARD output buffer */
#define STAT_REG		0x64 /**< @brief KEYBOARD status register */
#define KBC_CMD_REG		0x64 /**< @brief KEYBOARD command register */

/* Status register bits */

#define IBF				BIT(1) /**< @brief Input buffer status register bit */
#define OBF				BIT(0) /**< @brief Output buffer status register bit */
#define PAR_ERR			BIT(7) /**< @brief Parity error status register bit */
#define TO_ERR			(BIT(5) | BIT(6)) /**< @brief Time-out errors status register bits */

/* Command byte bits */

#define ENABLE_INTERRUPT	BIT(0) /**< @brief Enable Output-Buffer-Full Interrupt */

/* Scancodes */

#define ESC_RELEASE		0x81 /**< @brief ESC key release code */

/* Keyboard Controller Commands */

#define READ_CMD_BYTE	0x20 /**< @brief Read Keyboard Controller's Command Byte */
#define WRITE_CMD_BYTE	0x60 /**< @brief Read Keyboard Controller's Command Byte */

/* Timer 0 */
#define FREQ_TIMER_0 	60   /**< @brief Timer 0 frequency */
#define TIMER0_IRQ	    0    /**< @brief Timer 0 IRQ line */

/**@}*/

#endif /* _LCOM_I8042_H */
