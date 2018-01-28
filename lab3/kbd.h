#ifndef __KBD_H
#define __KBD_H

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
int kbd_subscribe_scan();

/**
 * @brief Unsubscribes Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_unsubscribe_scan();

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
void kbd_scan_handler();

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int();

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

/**
 * @brief Reads keyboard control command byte
 *
 * Increments sys_inb kernel calls counter if LAB3 is defined
 *
 * @returns keyboard control command byte
 */
unsigned long kbd_read_command_byte();

/**
 * @brief Write keyboard control command byte, enabling keyboard interrupts
 *
 * @param cmd byte command byte read on kbd_test_poll start
 *
 * @returns 0 upon success
 */
int kbd_write_command_byte(unsigned long cmd_byte);

/**
 * @brief issues a command to keyboard controller
 *
 * Checks if 8042 input buffer is empty, and then issues the command
 *
 * Tries this 5 times, waiting between them, returns 1 in case it fails 5 times
 *
 * Increments sys_inb kernel calls counter if LAB3 is defined
 *
 * @param cmd command to be issued
 *
 * @return 0 upon success, 1 otherwise
 */
int kbd_issue_command(unsigned long cmd);

/**
 * @brief Handles Keyboard interrupts in assembly
 *
 * Checks if 8042 input buffer is not empty, and then reads scancode
 *
 * If there was an error, or there was no scancode to be read, scancode = 0
 */
void kbd_scan_handler_asm();

#endif /* __KBD_H */
