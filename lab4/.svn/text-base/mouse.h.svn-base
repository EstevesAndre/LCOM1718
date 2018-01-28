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
 * Verifies if outpuf buffer is full (returning 1 if it is),
 * writes write_to_mouse command to command register,
 * writes the command to the outpuf buffer,
 * read the command the mouse sent to the outpuf buffer,
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
 * Sets stream mode by writting SET_STREAM_MODE and ENABLE_DATA_REPORT commands
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
 * @brief Issues the command to the KBC
 *
 * @param command command to be sent
 *
 * @return 0 upon success and non-zero otherwise
 */

unsigned int kbc_issue_command(unsigned long cmd);



/**
 * @brief Read KBC command byte
 *
 * @return command byte
 */

unsigned long mouse_read_command_byte();



/**
 * @brief Disable Mouse Interrupts by writing the corresponding command_byte to the KBC
 *
 * @param command_byte initial command byte
 *
 * @return 0 upon success and non-zero otherwise
 */

int mouse_disable_interrupts(unsigned long command_byte);



/**
 * @brief Enable Mouse Interrupts by writing the initial command_byte to the KBC
 *
 * @param command_byte initial command byte
 *
 * @return 0 upon success and non-zero otherwise
 */

int mouse_enable_interrupts(unsigned long command_byte);



/**
 * @brief Sets mouse on remote mode
 *
 * Sets mouse on remote mode by issuing DISABLE_DATA_REPORT and SET_REMOTE_MODE commands
 *
 * @return 0 upon success and non-zero otherwise
 */

int mouse_remote_mode();



/**
 * @brief Checks if there is a packet to read and reads it
 *
 * Reads packets by issuing a READ_DATA command and reading 3 times the OUTBUFFER
 *
 * @return 0 upon success and non-zero otherwise
 */

int mouse_remote_handler();



/**
 * @brief Re-sets mouse on stream mode
 *
 * re-sets mouse on remote mode by issuing DISABLE_DATA_REPORT and SET_STREAM_MODE commands
 *
 * @return 0 upon success and non-zero otherwise
 */

int mouse_reset_stream_mode();
