#ifndef __KBD_H
#define __KBD_H

#define BIT(n) (0x01<<(n))

#define TIMER0_IRQ	    	0    	/**< @brief Timer 0 IRQ line */
#define KBD_IRQ	       		1   /**< @brief KBD IRQ line */
#define WAIT_KBC			20000 /**< @brief Wait time from KBD controller */
#define FREQ_TIMER_0		60  	/**< @brief Timer 0 Frequency */

#define OUT_BUF 		0x60 /**< @brief KEYBOARD output buffer */
#define STAT_REG		0x64 /**< @brief KEYBOARD status register */
#define KBC_CMD_REG		0x64 /**< @brief KEYBOARD command register */

#define IBF				BIT(1) /**< @brief Input buffer status register bit */
#define OBF				BIT(0) /**< @brief Output buffer status register bit */
#define PAR_ERR			BIT(7) /**< @brief Parity error status register bit */
#define TO_ERR			(BIT(5) | BIT(6)) /**< @brief Time-out errors status register bits */

#define ESC_RELEASE		0x81 /**< @brief ESC key release code */

extern unsigned char scancode;
extern unsigned long global_timer_counter;

/** @defgroup kbd kbd
 * @{
 *
 * Reusable functions to work with the keyboard
 */

/**
 * @brief Subscribes, enables Keyboard interrupts and the standard MINIX 3 KBC interrupt handler will not be notified of the occurrence of KBC interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int kbd_subscribe_int();

/**
 * @brief Unsubscribes Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_unsubscribe_int();

/**
 * @brief Handles Keyboard interrupts, counting sys_inb() kernel calls
 *
 * Checks if 8042 input buffer is not empty, and then reads scancode
 *
 * If there was an error, or there was no scancode to read, scancode = 0
 *
 * Tries this 5 times, waiting between them
 *
 * Increments sys_inb kernel calls counter if LAB3 is defined
 */
void kbd_int_handler();


/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int(void );


/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int();


/**
 * @brief Timer 0 interrupt handler
 *
 * Increments timer counter
 */
void timer_int_handler();

#endif /* __KBD_H */
